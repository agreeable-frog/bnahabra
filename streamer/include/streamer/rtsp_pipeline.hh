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
    int id;
    ImageBuffer(unsigned char* data, size_t size, int frameId) : data(data), size(size), id(frameId) {
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
    void enable() {
        std::lock_guard<std::mutex> lock(mutex);
        enabled = true;
    }
    void disable() {
        std::lock_guard<std::mutex> lock(mutex);
        enabled = false;
    }
    void enqueue(std::shared_ptr<ImageBuffer> buffer) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!enabled) return;
        if (queue.size() < 2) queue.push(buffer);
        condVar.notify_one();
    }
    std::shared_ptr<ImageBuffer> dequeue() {
        std::unique_lock<std::mutex> lock(mutex);
        while (queue.empty()) {
            std::cout << "queue empty\n";
            condVar.wait(lock);
        }
        std::shared_ptr<ImageBuffer> val = queue.front();
        queue.pop();
        return val;
    }

private:
    std::queue<std::shared_ptr<ImageBuffer>> queue;
    mutable std::mutex mutex;
    std::condition_variable condVar;
    bool enabled = false;
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