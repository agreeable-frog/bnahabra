#pragma once

#include <vector>
#include <queue>
#include <sys/types.h>
#include <mutex>
#include <atomic>
#include <condition_variable>

struct Image {
    std::vector<u_char> data;
    size_t width;
    size_t height;
    size_t depth;
    uint id;
};

class Swapchain {
public:
    Swapchain() : _queue(), _mutex(), _var() {
    }
    void present(const Image& image);
    Image take();

private:
    std::queue<Image> _queue;
    mutable std::mutex _mutex;
    std::condition_variable _var;
};