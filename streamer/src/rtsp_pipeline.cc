#include "streamer/rtsp_pipeline.hh"

#include <gst/rtsp-server/rtsp-server.h>
#include <gst/app/gstappsrc.h>

#define APPSRC_FRAMERATE 20

RtspPipeline::RtspPipeline(const std::string& address, const std::string& port,
                           const std::string& interface,
                           const std::string& mountPoint)
    : _address(address),
      _port(port),
      _interface(interface),
      _mountPoint(mountPoint) {
}

static void need_data(GstElement* appsrc, guint size, RtspPipeline* pipeline) {
    pipeline->setNeedData(true);
}

static void enough_data(GstElement* appsrc, RtspPipeline* pipeline) {
    pipeline->setNeedData(false);
}

static void prepared(GstRTSPMedia* media, RtspPipeline* pipeline) {
}

static void unprepared(GstRTSPMedia* media, RtspPipeline* pipeline) {
    pipeline->setRunning(false);
}

static void media_configure(GstRTSPMediaFactory* factory, GstRTSPMedia* media,
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
    pipeline->setRunning(true);
    pipeline->startFeedLoop(appsrc);
}

void RtspPipeline::start() {
    _runContext = g_main_context_new();
    _runLoop = g_main_loop_new(_runContext, false);
    GstRTSPServer* server;
    GstRTSPMountPoints* mounts;
    GstRTSPMediaFactory* factory;
    server = gst_rtsp_server_new();
    mounts = gst_rtsp_server_get_mount_points(server);
    factory = gst_rtsp_media_factory_new();
    gst_rtsp_media_factory_set_shared(factory, TRUE);
    gst_rtsp_media_factory_set_launch(
        factory,
        "( appsrc name=mysrc ! queue ! videoconvert ! video/x-raw,format=I420 ! "
        "x264enc speed-preset=ultrafast tune=zerolatency ! rtph264pay "
        "name=pay0 pt=96 )");

    g_signal_connect(factory, "media-configure", (GCallback)media_configure,
                     this);

    gst_rtsp_mount_points_add_factory(mounts, "/test", factory);

    gst_rtsp_server_attach(server, _runContext);
    _runThread = std::thread(&RtspPipeline::runLoop, this);
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

    while (_running) {
        if (_needData) {
            buffer = gst_buffer_new_and_alloc(960 * 540 * 3);
            gst_buffer_memset(buffer, 0, 0xff, 960 * 540 * 3);
            GST_BUFFER_PTS(buffer) = timestamp;
            GST_BUFFER_DURATION(buffer) =
                gst_util_uint64_scale_int(1, GST_SECOND, APPSRC_FRAMERATE);

            timestamp += GST_BUFFER_DURATION(buffer);
            g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);
            gst_buffer_unref(buffer);
        }
    }
}