#pragma once

#include <sys/types.h>

class Texture {
public:
    Texture(uint width, uint height);
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
    ~Texture();
    void bind() const;
    void unbind() const;
    uint getId() const {
        return _id;
    }
    uint getWidth() const {
        return _width;
    }
    uint getHeight() const {
        return _height;
    }
private:
    uint _id;
    uint _width;
    uint _height;
    void init(uint width, uint height);
};

class Framebuffer {
public:
    Framebuffer();
    Framebuffer(const Framebuffer& other);
    Framebuffer& operator=(const Framebuffer& other);
    ~Framebuffer();
    uint getId() const {
        return _id;
    }
    bool isComplete() const;
    void bind() const;
    void unbind() const;
    void attachTexture(const Texture& texture);
private:
    uint _id;
    void init();
};