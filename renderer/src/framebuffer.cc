#include "framebuffer.hh"

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
    return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void Framebuffer::bind() const {
    glBindBuffer(GL_FRAMEBUFFER, _id);
}

void Framebuffer::unbind() const {
    glBindBuffer(GL_FRAMEBUFFER, 0);
}

void Texture::init() {
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 900, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
}