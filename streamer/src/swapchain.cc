#include "streamer/swapchain.hh"

#include <iostream>

void Swapchain::present(const Image& image) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_queue.size() > 1) _queue.pop();
    _queue.push(image);
    _var.notify_one();
}

Image Swapchain::take() {
    std::unique_lock<std::mutex> lock(_mutex);
    while (_queue.empty()) {
        _var.wait(lock);
    }
    auto image = _queue.front();
    return image;
}