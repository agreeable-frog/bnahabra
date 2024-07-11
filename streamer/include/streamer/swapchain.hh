#pragma once

#include <vector>
#include <queue>
#include <sys/types.h>
#include <mutex>
#include <atomic>
#include <condition_variable>

class Swapchain {
public:
    Swapchain(size_t width, size_t height, size_t depth)
        : _queue(),
          _width(width),
          _height(height),
          _depth(depth),
          _mutex(),
          _var() {
    }
    bool needsData() const;
    void present(u_char* data, size_t width, size_t height, int frameId);
    u_char* take();

private:
    std::queue<u_char*> _queue;
    size_t _width;
    size_t _height;
    size_t _depth;
    mutable std::mutex _mutex;
    std::condition_variable _var;
};