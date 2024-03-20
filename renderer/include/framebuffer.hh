#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

class Framebuffer {
public:
    Framebuffer();
    Framebuffer(const Framebuffer& other);
    Framebuffer& operator=(const Framebuffer& other);
    ~Framebuffer();
    GLuint getId() const {
        return _id;
    }
    bool isComplete() const;
    void bind() const;
    void unbind() const;
private:
    GLuint _id;
    void init();
};

class Texture {
public:
    Texture();
    Texture(const Texture& other);
    Texture& operator=(const Texture& other);
    ~Texture();
    GLuint getId() const {
        return _id;
    }
private:
    GLuint _id;
    void init();
};