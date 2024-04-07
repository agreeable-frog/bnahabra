#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>
#include <sys/types.h>
#include <string>
#include <vector>

class TextureAtlas;

class AtlasTexture {
public:
    AtlasTexture(const std::vector<u_char>& data, size_t width, size_t height,
                 GLenum format, std::weak_ptr<TextureAtlas> pAtlas,
                 size_t atlasIndex);
    const std::vector<u_char>& getData() const {
        return _data;
    }
    size_t getWidth() const {
        return _width;
    }
    size_t getHeight() const {
        return _height;
    }
    GLenum getFormat() const {
        return _format;
    }
    const std::weak_ptr<TextureAtlas>& getPAtlas() const {
        return _pAtlas;
    }
    size_t getAtlastIndex() const {
        return _atlasIndex;
    }

private:
    std::vector<u_char> _data;
    size_t _width;
    size_t _height;
    GLenum _format;
    std::weak_ptr<TextureAtlas> _pAtlas;
    size_t _atlasIndex;
};

class TextureAtlas : public std::enable_shared_from_this<TextureAtlas> {
public:
    TextureAtlas(GLenum format);
    ~TextureAtlas();
    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas& operator=(const TextureAtlas&) = delete;
    std::shared_ptr<AtlasTexture> addTexture(const std::string& path);
    std::shared_ptr<AtlasTexture> addTexture(const std::vector<u_char>& data,
                                             size_t width, size_t height,
                                             GLenum format);
    void build();
    void bind() const;
    void unbind() const;

private:
    bool _built = false;
    std::vector<std::shared_ptr<AtlasTexture>> _pTextures;
    static GLuint _boundId; // prevent useless rebinds
    GLuint _id;
    size_t _width;
    size_t _height;
    GLenum _format;
};