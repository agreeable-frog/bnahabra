#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>
#include <sys/types.h>
#include <string>
#include <vector>

class TextureAtlas {
public:
    TextureAtlas();
    ~TextureAtlas();
    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas& operator=(const TextureAtlas&) = delete;
    bool addTexture(const std::string& path);
    bool addTexture(std::vector<u_char> data, size_t width, size_t height, GLenum _format);
    void bind() const;
    void unbind() const;
private:
    static GLuint _currentBound; // prevent useless rebinds
    GLuint _id;
    size_t _width;
    size_t _height;
    GLenum _format;
};

class AtlasedTexture {
public:
    AtlasedTexture();
private:
    std::weak_ptr<TextureAtlas> _atlas;
    size_t _width;
    size_t _height;
    size_t _atlasIndex;
};