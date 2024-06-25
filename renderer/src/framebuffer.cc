#include "renderer/framebuffer.hh"

void Framebuffer::init() {
    glGenRenderbuffers(1, &_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, _format, _width, _height);
    glGenRenderbuffers(1, &_depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _renderBuffer);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, _depthBuffer);
}

void Framebuffer::destroy() {
    glDeleteFramebuffers(1, &_fbo);
    glDeleteRenderbuffers(1, &_renderBuffer);
    glDeleteRenderbuffers(1, &_depthBuffer);
}

Framebuffer::Framebuffer(size_t width, size_t height, GLenum format)
    : _width(width), _height(height), _format(format) {
    init();
}

Framebuffer::Framebuffer(const Framebuffer& framebuffer) {
    _width = framebuffer._width;
    _height = framebuffer._height;
    _format = framebuffer._format;
    init();
    glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height,
                      GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

Framebuffer& Framebuffer::operator=(const Framebuffer& framebuffer) {
    destroy();
    _width = framebuffer._width;
    _height = framebuffer._height;
    _format = framebuffer._format;
    init();
    glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height,
                      GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    return *this;
}

std::vector<u_char> Framebuffer::read() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
    u_char* data = new u_char[_width * _height * 3];
    glReadPixels(0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, data);
    std::vector<u_char> out;
    out.insert(out.end(), &data[0], &data[_width * _height * 3]);
    delete data;
    for (size_t line = 0; line != _height / 2; ++line) {
        std::swap_ranges(out.begin() + 3 * _width * line,
                         out.begin() + 3 * _width * (line + 1),
                         out.begin() + 3 * _width * (_height - line - 1));
    }
    return out;
}

Framebuffer::~Framebuffer() {
    destroy();
}