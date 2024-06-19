#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

class Framebuffer {
public:
    Framebuffer(size_t width, size_t height, GLenum _format);
    Framebuffer(const Framebuffer&);
    Framebuffer& operator=(const Framebuffer&);
    ~Framebuffer();
    void bind(GLenum bind);
private:
    size_t _width;
    size_t _height;
    GLenum _format;
    GLuint _renderBuffer;
    GLuint _depthBuffer;
    GLuint _fbo;
};