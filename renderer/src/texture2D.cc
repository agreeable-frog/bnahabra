#include "renderer/texture2D.hh"

#include <GL/glew.h>
#include <GL/gl.h>
#include <stdexcept>

#include "stb_image.h"

Texture2D::Texture2D(uint width, uint height, std::vector<u_char> data,
                     GLenum format)
    : _width(width), _height(height), _format(format) {
    glGenTextures(1, &_id);
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format,
                 GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}

std::shared_ptr<Texture2D> Texture2D::fromImage(const std::string& path) {
    int width, height, nbChannels;
    u_char* data = stbi_load(path.c_str(), &width, &height, &nbChannels, 0);
    if (!data) {
        throw std::runtime_error("Can't laod image");
    }
    std::vector<u_char> vdata;
    vdata.insert(vdata.end(), &data[0], &data[width * height * nbChannels]);
    stbi_image_free(data);
    GLenum format;
    if (nbChannels == 1) {
        format = GL_R;
    } else if (nbChannels == 3) {
        format = GL_RGB;
    } else {
        format = GL_RGBA;
    }
    return std::make_shared<Texture2D>(width, height, vdata, format);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &_id);
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture2D::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}