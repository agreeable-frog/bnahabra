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

struct ImageBuffer {
    unsigned char* data;
    size_t size;
    ImageBuffer(unsigned char* data, size_t size) : data(data), size(size) {
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
        if (queue.size() > 1) return;
        queue.push(buffer);
        condVar.notify_one();
    }
    std::shared_ptr<ImageBuffer> dequeue() {
        std::unique_lock<std::mutex> lock(mutex);
        condVar.wait(lock, [&] { return !queue.empty(); });
        std::shared_ptr<ImageBuffer> val = queue.front();
        if (queue.size() >= 1) queue.pop();
        return val;
    }

private:
    std::queue<std::shared_ptr<ImageBuffer>> queue;
    mutable std::mutex mutex;
    std::condition_variable condVar;
};

class Executor {
public:
    Executor(int argc, char** argv);
    Executor(const Executor&) = delete;
    Executor& operator=(const Executor&) = delete;
    ~Executor();
    SafeQueue imageQueue;

private:
    GMainLoop* loop;
    GMainContext* context;
    std::thread thread;
    void workerLoop(int arc, char** argv);
};