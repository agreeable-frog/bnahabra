#include "streamer/rtsp_pipeline.hh"

#include <iostream>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <gst/app/gstappsrc.h>
#include <thread>

static gboolean read_data(GstElement* appsrc, guint unused,
                          gpointer userdata) {
    SafeQueue* queue = (SafeQueue*)userdata;
    GstFlowReturn ret;
    GstBuffer* buffer;

    std::shared_ptr<ImageBuffer> image = queue->dequeue();

    buffer = gst_buffer_new_and_alloc(960 * 540 * 3);
    gst_buffer_fill(buffer, 0, image->data, 960 * 540 * 3);
    // gst_buffer_memset(buffer, 0, 0xff, 960 * 540 * 3);

    g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);

    gst_buffer_unref(buffer);
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
    g_object_set(
        G_OBJECT(appsrc), "caps",
        gst_caps_new_simple("video/x-raw", "format", G_TYPE_STRING, "RGB",
                            "width", G_TYPE_INT, 960, "height", G_TYPE_INT, 540,
                            "framerate", GST_TYPE_FRACTION, 0, 1, NULL),
        NULL);
    g_object_set(G_OBJECT(appsrc), "is-live", TRUE, "stream-type", 0, "format",
                 GST_FORMAT_TIME, "do-timestamp", TRUE, NULL);
    g_signal_connect(appsrc, "need-data", G_CALLBACK(read_data), user_data);

    gst_object_unref(appsrc);
    gst_object_unref(element);
    std::cout << "configured ok\n";
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
        "( appsrc name=mysrc ! queue ! videoconvert ! video/x-raw,format=I420 ! "
        "queue ! x264enc ! rtph264pay name=pay0 pt=96 )");

    /* notify when our media is ready, This is called whenever someone asks for
     * the media and a new pipeline with our appsrc is created */
    g_signal_connect(factory, "media-configure", (GCallback)media_configure, &imageQueue);

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