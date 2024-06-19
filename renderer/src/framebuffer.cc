#include "renderer/framebuffer.hh"

Framebuffer::Framebuffer(size_t width, size_t height, GLenum format)
    : _width(width), _height(height), _format(format) {
    glGenRenderbuffers(1, &_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, _format, _width, _height);
    glGenRenderbuffers(1, &_depthBuffer);
}