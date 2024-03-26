#include "streamer/rtsp_pipeline.hh"

#include <iostream>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <gst/app/gstappsrc.h>
#include <thread>

#define APPSRC_FRAMERATE 20

static gboolean read_data(GstElement* appsrc, guint size, gpointer userdata) {
    static GstClockTime timestamp = 0;
    SafeQueue* queue = (SafeQueue*)userdata;
    GstFlowReturn ret;
    GstBuffer* buffer;

    std::shared_ptr<ImageBuffer> image = queue->dequeue();

    buffer = gst_buffer_new_and_alloc(960 * 540 * 3);
    gst_buffer_fill(buffer, 0, image->data, 960 * 540 * 3);
    GST_BUFFER_PTS(buffer) = timestamp;
    GST_BUFFER_DURATION(buffer) =
        gst_util_uint64_scale_int(1, GST_SECOND, APPSRC_FRAMERATE);

    timestamp += GST_BUFFER_DURATION(buffer);

    g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);
    if (ret != GST_FLOW_OK) {
        g_signal_emit_by_name(appsrc, "end-of-stream", &ret);
        return FALSE;
    }
    gst_buffer_unref(buffer);
    return TRUE;
}

static void new_state_callback(GstRTSPMedia* self, gint object,
                               gpointer user_data) {
    // std::cout << "new_state_callback\n";
}

static void new_stream_callback(GstRTSPMedia* self, GstRTSPStream* object,
                                gpointer user_data) {
    // std::cout << "new_stream_callback\n";
}

static void prepared_callback(GstRTSPMedia* self, gpointer user_data) {
    // std::cout << "prepared_callback\n";
}

static void removed_stream_callback(GstRTSPMedia* self, GstRTSPStream* object,
                                    gpointer user_data) {
    // std::cout << "removed_stream_callback\n";
}

static void target_state_callback(GstRTSPMedia* self, gint object,
                                  gpointer user_data) {
    // std::cout << "target_state_callback\n";
}

static void unprepared_callback(GstRTSPMedia* self, gpointer user_data) {
    // std::cout << "unprepared_callback\n";
}

/* called when a new media pipeline is constructed. We can query the
 * pipeline and configure our appsrc */
static void media_configure(GstRTSPMediaFactory* factory, GstRTSPMedia* media,
                            gpointer user_data) {
    GstElement *element, *appsrc;

    /* get the element used for providing the streams of the media */
    element = gst_rtsp_media_get_element(media);

    /* get our appsrc, we named it 'mysrc' with the name property */
    appsrc = gst_bin_get_by_name_recurse_up(GST_BIN(element), "mysrc");

    /* configure the caps of the video */
    g_object_set(G_OBJECT(appsrc), "caps",
                 gst_caps_new_simple(
                     "video/x-raw", "format", G_TYPE_STRING, "RGB", "width",
                     G_TYPE_INT, 960, "height", G_TYPE_INT, 540, "framerate",
                     GST_TYPE_FRACTION, APPSRC_FRAMERATE, 1, NULL),
                 NULL);
    g_object_set(G_OBJECT(appsrc), "is-live", TRUE, "stream-type", 0, "format",
                 GST_FORMAT_TIME, "do-timestamp", TRUE, NULL);
    g_signal_connect(appsrc, "need-data", G_CALLBACK(read_data), user_data);
    g_signal_connect(media, "new-state", G_CALLBACK(new_state_callback), 0);
    g_signal_connect(media, "new-stream", G_CALLBACK(new_stream_callback), 0);
    g_signal_connect(media, "prepared", G_CALLBACK(prepared_callback), 0);
    g_signal_connect(media, "removed-stream",
                     G_CALLBACK(removed_stream_callback), 0);
    g_signal_connect(media, "target-state", G_CALLBACK(target_state_callback),
                     0);
    g_signal_connect(media, "unprepared", G_CALLBACK(unprepared_callback), 0);

    gst_object_unref(appsrc);
    gst_object_unref(element);
    std::cout << "media configured ok\n";
}

void Executor::workerLoop(int argc, char** argv) {
    gst_init(&argc, &argv);
    g_main_context_push_thread_default(context);
    GstRTSPServer* server;
    GstRTSPMountPoints* mounts;
    GstRTSPMediaFactory* factory;

    /* create a server instance */
    server = gst_rtsp_server_new();

    /* get the mount points for this server, every server has a default object
     * that be used to map uri mount points to media factories */
    mounts = gst_rtsp_server_get_mount_points(server);

    /* make a media factory for a test stream. The default media factory can use
     * gst-launch syntax to create pipelines.
     * any launch line works as long as it contains elements named pay%d. Each
     * element with pay%d names will be a stream */
    factory = gst_rtsp_media_factory_new();
    gst_rtsp_media_factory_set_launch(
        factory,
        "( appsrc name=mysrc ! videoconvert ! video/x-raw,format=I420 ! "
        "x264enc speed-preset=ultrafast tune=zerolatency ! rtph264pay "
        "name=pay0 pt=96 )");

    /* notify when our media is ready, This is called whenever someone asks
                     a new pipeline with our appsrc is created */
    g_signal_connect(factory, "media-configure", (GCallback)media_configure,
                     &imageQueue);
    imageQueue.enable();

    /* attach the test factory to the /test url */
    gst_rtsp_mount_points_add_factory(mounts, "/test", factory);

    /* don't need the ref to the mounts anymore */
    g_object_unref(mounts);

    /* attach the server to the default maincontext */
    gst_rtsp_server_attach(server, context);

    /* start serving */
    g_print("stream ready at rtsp://127.0.0.1:8554/test\n");
    std::cout << "main loop start\n";
    g_main_loop_run(loop);
    g_main_context_pop_thread_default(context);
}

Executor::Executor(int argc, char** argv) {
    context = g_main_context_new();
    loop = g_main_loop_new(context, false);
    thread = std::thread(&Executor::workerLoop, this, argc, argv);
}

Executor::~Executor() {
    GSource* idleSource = g_idle_source_new();
    g_source_set_callback(idleSource, (GSourceFunc)g_main_loop_quit, loop, 0);
    g_source_attach(idleSource, context);
    g_source_unref(idleSource);
    if (thread.joinable()) {
        thread.join();
    }
    g_main_loop_unref(loop);
    g_main_context_unref(context);
}