#include "renderer/framebuffer.hh"

#include <GL/glew.h>
#include <GL/gl.h>

void Framebuffer::init() {
    glGenFramebuffers(1, &_id);
}

Framebuffer::Framebuffer() {
    init();
}

Framebuffer::Framebuffer(const Framebuffer& other) {
    init();
}

Framebuffer& Framebuffer::operator=(const Framebuffer& other) {
    if (&other == this) {
        return *this;
    }
    glDeleteFramebuffers(1, &_id);
    init();
    return *this;
}

bool Framebuffer::isComplete() const {
    return (glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
            GL_FRAMEBUFFER_COMPLETE);
}

void Framebuffer::bind() const {
    glBindBuffer(GL_FRAMEBUFFER, _id);
}

void Framebuffer::unbind() const {
    glBindBuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::attachTexture(const Texture& texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture.getId(), 0);
}

void Texture::init(uint width, uint height) {
    _width = width;
    _height = height;
    glGenTextures(1, &_id);
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind();
}

Texture::Texture(uint width, uint height) {
    init(width, height);
}

Texture::Texture(const Texture& other) {
    init(other.getWidth(), other.getHeight());
}

Texture& Texture::operator=(const Texture& other) {
    if (&other == this) {
        return *this;
    }
    glDeleteFramebuffers(1, &_id);
    init(other.getWidth(), other.getHeight());
}

Texture::~Texture() {
    glDeleteFramebuffers(1, &_id);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}