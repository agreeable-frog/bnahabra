#include "streamer/swapchain.hh"

#include <iostream>

void Swapchain::present(const Image& image) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (image.width != _width || image.height != _height ||
        image.depth != _depth) {
        throw std::runtime_error(
            "Image given to swapchain doesn't fit! WTF are you doing?");
    }
    if (_queue.size() > 1) _queue.pop();
    _queue.push(image);
    if (_queue.size() > 1) {
        auto frontId = _queue.front().id;
        if (frontId > image.id) {
            std::cout
                << "Reordering in images id given to swapchain detected.\n";
        }
    }
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