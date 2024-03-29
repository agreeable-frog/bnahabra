#pragma once

#include <thread>
#include <gst/gst.h>
#include <memory>
#include <cstring>
#include <iostream>
#include <atomic>
#include <gst/rtsp-server/rtsp-server.h>

#include "streamer/swapchain.hh"

class RtspPipeline {
public:
    RtspPipeline(const std::string& address, const std::string& port,
                 const std::string& mountPoint, size_t width, size_t height);
    void start();
    void stop();
    Swapchain& getSwapchain() {
        return _swapchain;
    }

private:
    std::string _address;
    std::string _port;
    std::string _mountPoint;
    size_t _width;
    size_t _height;
    size_t _depth;

    GMainContext* _runContext;
    GMainLoop* _runLoop;
    std::thread _runThread;
    std::atomic<bool> _running;
    std::atomic<bool> _needData;
    void runLoop();

    std::thread _feedThread;
    Swapchain _swapchain;
    void startFeedLoop(GstElement* appsrc);
    void feedLoop(GstElement* appsrc);
    static void need_data(GstElement* appsrc, guint size,
                          RtspPipeline* pipeline);
    static void enough_data(GstElement* appsrc, RtspPipeline* pipeline);
    static void prepared(GstRTSPMedia* media, RtspPipeline* pipeline);
    static void unprepared(GstRTSPMedia* media, RtspPipeline* pipeline);
    static void media_configure(GstRTSPMediaFactory* factory,
                                GstRTSPMedia* media, RtspPipeline* pipeline);
};