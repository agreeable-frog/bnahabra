#include "streamer/rtsp_pipeline.hh"

#include <gst/rtsp-server/rtsp-server.h>
#include <gst/app/gstappsrc.h>

#define APPSRC_FRAMERATE 30

RtspPipeline::RtspPipeline(const std::string& address, const std::string& port,
                           const std::string& mountPoint, size_t width,
                           size_t height)
    : _address(address),
      _port(port),
      _mountPoint(mountPoint),
      _width(width),
      _height(height),
      _depth(3 * sizeof(u_char)),
      _swapchain(width, height, 3 * sizeof(u_char)) {
}

void RtspPipeline::need_data(GstElement* appsrc, guint size,
                             RtspPipeline* pipeline) {
    pipeline->_needData = true;
}

void RtspPipeline::enough_data(GstElement* appsrc, RtspPipeline* pipeline) {
    pipeline->_needData = false;
}

void RtspPipeline::prepared(GstRTSPMedia* media, RtspPipeline* pipeline) {
}

void RtspPipeline::unprepared(GstRTSPMedia* media, RtspPipeline* pipeline) {
    pipeline->_running = false;
}

void RtspPipeline::media_configure(GstRTSPMediaFactory* factory,
                                   GstRTSPMedia* media,
                                   RtspPipeline* pipeline) {
    GstElement* element;
    GstElement* appsrc;
    g_signal_connect(media, "unprepared", G_CALLBACK(unprepared), pipeline);
    g_signal_connect(media, "prepared", G_CALLBACK(prepared), pipeline);

    element = gst_rtsp_media_get_element(media);
    appsrc = gst_bin_get_by_name_recurse_up(GST_BIN(element), "mysrc");

    g_object_set(G_OBJECT(appsrc), "caps",
                 gst_caps_new_simple(
                     "video/x-raw", "format", G_TYPE_STRING, "RGB", "width",
                     G_TYPE_INT, 960, "height", G_TYPE_INT, 540, "framerate",
                     GST_TYPE_FRACTION, APPSRC_FRAMERATE, 1, NULL),
                 NULL);
    g_object_set(G_OBJECT(appsrc), "is-live", TRUE, "stream-type", 0, "format",
                 GST_FORMAT_TIME, "do-timestamp", TRUE, NULL);
    g_signal_connect(appsrc, "need-data", G_CALLBACK(need_data), pipeline);
    g_signal_connect(appsrc, "enough-data", G_CALLBACK(enough_data), pipeline);
    pipeline->_running = true;
    pipeline->startFeedLoop(appsrc);
}

void RtspPipeline::start() {
    _runContext = g_main_context_new();
    _runLoop = g_main_loop_new(_runContext, false);
    GstRTSPServer* server;
    GstRTSPMountPoints* mounts;
    GstRTSPMediaFactory* factory;
    server = gst_rtsp_server_new();
    gst_rtsp_server_set_address(server, _address.c_str());
    gst_rtsp_server_set_service(server, _port.c_str());
    mounts = gst_rtsp_server_get_mount_points(server);
    factory = gst_rtsp_media_factory_new();
    gst_rtsp_media_factory_set_shared(factory, TRUE);
    gst_rtsp_media_factory_set_launch(
        factory,
        "( appsrc name=mysrc ! queue ! "
        "videoconvert ! video/x-raw,format=I420 ! "
        "x264enc speed-preset=ultrafast tune=zerolatency ! "
        "rtph264pay name=pay0 pt=96 )");

    g_signal_connect(factory, "media-configure", (GCallback)media_configure,
                     this);

    gst_rtsp_mount_points_add_factory(mounts, ("/" + _mountPoint).c_str(),
                                      factory);

    gst_rtsp_server_attach(server, _runContext);
    _runThread = std::thread(&RtspPipeline::runLoop, this);
    std::cout << "RTSP stream available at " << _address << ":" << _port << "/"
              << _mountPoint << '\n';
}

void RtspPipeline::stop() {
    g_main_loop_quit(_runLoop);
    if (_runThread.joinable()) {
        _runThread.join();
    }
    if (_feedThread.joinable()) {
        _feedThread.join();
    }
}

void RtspPipeline::runLoop() {
    g_main_context_push_thread_default(_runContext);
    g_main_loop_run(_runLoop);
    g_main_context_pop_thread_default(_runContext);
    _running = false;
}

void RtspPipeline::startFeedLoop(GstElement* appsrc) {
    if (_feedThread.joinable()) {
        _feedThread.join();
    }
    _feedThread = std::thread(&RtspPipeline::feedLoop, this, appsrc);
}

void RtspPipeline::feedLoop(GstElement* appsrc) {
    GstClockTime timestamp = 0;
    GstFlowReturn ret;
    GstBuffer* buffer;
    Image image;

    while (_running) {
        if (_needData) {
            buffer = gst_buffer_new_and_alloc(_width * _height * _depth /
                                              sizeof(u_char));
            image = _swapchain.take();
            gst_buffer_fill(buffer, 0, image.data.data(), _width * _height * _depth / sizeof(u_char));
            GST_BUFFER_PTS(buffer) = timestamp;
            GST_BUFFER_DURATION(buffer) =
                gst_util_uint64_scale_int(1, GST_SECOND, APPSRC_FRAMERATE);

            timestamp += GST_BUFFER_DURATION(buffer);
            g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);
            gst_buffer_unref(buffer);
        }
    }
}