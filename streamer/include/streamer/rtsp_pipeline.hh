#pragma once

#include <thread>
#include <gst/gst.h>
#include <memory>
#include <cstring>
#include <iostream>
#include <atomic>

#include "streamer/swapchain.hh"

class RtspPipeline {
public:
    RtspPipeline(const std::string& address, const std::string& port,
                 const std::string& interface, const std::string& mountPoint);
    void start();
    void stop();
    void startFeedLoop(GstElement* appsrc);
    void setRunning(bool running) {
        _running = running;
    }
    const std::atomic<bool>& running() const {
        return _running;
    }
    void setNeedData(bool needData) {
        _needData = needData;
    }
    const std::atomic<bool>& needData() const {
        return _needData;
    }
    void setSwapchain(std::shared_ptr<Swapchain> swapchain) {
        _swapchain = swapchain;
    }


private:
    std::string _address;
    std::string _port;
    std::string _interface;
    std::string _mountPoint;

    GMainContext* _runContext;
    GMainLoop* _runLoop;
    std::thread _runThread;
    std::atomic<bool> _running;
    std::atomic<bool> _needData;
    void runLoop();

    std::thread _feedThread;
    std::shared_ptr<Swapchain> _swapchain;
    void feedLoop(GstElement* appsrc);
};