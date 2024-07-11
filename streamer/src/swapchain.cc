#include "streamer/swapchain.hh"

#include <iostream>

bool Swapchain::needsData() const {
    return _queue.size() <= 1;
}

void Swapchain::present(u_char* data, size_t width, size_t height, int frameId) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (width != _width || height != _height) {
        throw std::runtime_error(
            "Image given to swapchain doesn't fit! WTF are you doing?");
    }
    _queue.push(data);
    _var.notify_one();
}

u_char* Swapchain::take() {
    std::unique_lock<std::mutex> lock(_mutex);
    while (_queue.empty()) {
        _var.wait(lock);
    }
    auto image = _queue.front();
    _queue.pop();
    return image;
}