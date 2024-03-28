#pragma once

#include <thread>
#include <gst/gst.h>
#include <vector>
#include <mutex>
#include <memory>
#include <queue>
#include <cstring>
#include <condition_variable>
#include <iostream>
#include <atomic>

struct ImageBuffer {
    unsigned char* data;
    size_t size;
    int id;
    ImageBuffer(unsigned char* data, size_t size, int frameId)
        : data(data), size(size), id(frameId) {
    }
    ImageBuffer(const ImageBuffer& other) {
        size = other.size;
        data = new unsigned char[size];
        std::memcpy(data, other.data, size);
    }
    ImageBuffer& operator=(const ImageBuffer& other) {
        if (&other == this) return *this;
        delete data;
        size = other.size;
        data = new unsigned char[size];
        std::memcpy(data, other.data, size);
        return *this;
    }
    ~ImageBuffer() {
        delete data;
    }
};

class SafeQueue {
public:
    void enqueue(std::shared_ptr<ImageBuffer> buffer) {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.size() > 1) {
            queue.pop();
        }
        queue.push(buffer);
        condVar.notify_one();
    }
    std::shared_ptr<ImageBuffer> dequeue() {
        std::unique_lock<std::mutex> lock(mutex);
        while (queue.empty()) {
            condVar.wait(lock);
        }
        std::shared_ptr<ImageBuffer> val = queue.front();
        return val;
    }

private:
    std::queue<std::shared_ptr<ImageBuffer>> queue;
    mutable std::mutex mutex;
    std::condition_variable condVar;
};

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
    SafeQueue _imageQueue;
    void feedLoop(GstElement* appsrc);
};