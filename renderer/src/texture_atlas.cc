#include "renderer/texture_atlas.hh"

#include <stdexcept>
#include <iostream>

#include "stb_image.h"

GLuint TextureAtlas::_boundId = 0;

AtlasTexture::AtlasTexture(const std::vector<u_char>& data, size_t width,
                           size_t height, GLenum format,
                           std::weak_ptr<TextureAtlas> pAtlas,
                           size_t atlasIndex)
    : _data(data),
      _width(width),
      _height(height),
      _format(format),
      _pAtlas(pAtlas),
      _atlasIndex(atlasIndex) {
}

TextureAtlas::TextureAtlas(GLenum format) : _format(format) {
    glGenTextures(1, &_id);
    if ((format != GL_R) && (format != GL_RGB) && (format != GL_RGBA))
        throw std::runtime_error("Atlas format not recognized.");
}

TextureAtlas::~TextureAtlas() {
    if (_boundId == _id) _boundId = 0;
    glDeleteTextures(1, &_id);
}

void TextureAtlas::bind() const {
    if (_boundId == _id) return;
    glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
    _boundId = _id;
}

void TextureAtlas::unbind() const {
    if (_boundId == 0) return;
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    _boundId = 0;
}

std::shared_ptr<AtlasTexture> TextureAtlas::addTexture(
    const std::string& path) {
    int width, height, nbChannels;
    u_char* data = stbi_load(path.c_str(), &width, &height, &nbChannels, 0);
    if (!data) {
        throw std::runtime_error("Could not load image file.");
    }
    GLenum format;
    switch (nbChannels) {
        case 1:
            format = GL_R;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            throw std::runtime_error(
                "Unrecognized pixel format in image file.");
    }
    std::vector<u_char> vdata;
    vdata.insert(vdata.end(), &data[0], &data[width * height * nbChannels]);
    stbi_image_free(data);
    return addTexture(vdata, width, height, format);
}

std::shared_ptr<AtlasTexture> TextureAtlas::addTexture(
    const std::vector<u_char>& data, size_t width, size_t height,
    GLenum format) {
    if (_built)
        throw std::runtime_error("Can't add texture to already built atlas.");
    if (format != _format) {
        throw std::runtime_error(
            "Image loaded in atlas has wrong pixel format.");
    }
    std::shared_ptr<AtlasTexture> pTexture = std::make_shared<AtlasTexture>(
        data, width, height, format, std::weak_ptr<TextureAtlas>(shared_from_this()), _pTextures.size());
    _width = std::max(_width, width);
    _height = std::max(_height, height);
    _pTextures.push_back(pTexture);
    return pTexture;
}

void TextureAtlas::build() {
    bind();
    GLenum internalFormat;
    switch (_format) {
        case GL_R:
            internalFormat = GL_R8;
            break;
        case GL_RGB:
            internalFormat = GL_RGB8;
            break;
        case GL_RGBA:
            internalFormat = GL_RGBA8;
            break;
        default:
            throw std::runtime_error("Atlas format not recognized.");
            break;
    }
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, internalFormat, _width, _height,
                   _pTextures.size());
    for (const auto& pTexture : _pTextures) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, pTexture->getAtlastIndex(),
                        pTexture->getWidth(), pTexture->getHeight(), 1, _format,
                        GL_UNSIGNED_BYTE, (void*)pTexture->getData().data());
    }
    _built = true;
}