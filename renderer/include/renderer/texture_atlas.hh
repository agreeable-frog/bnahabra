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
    AtlasTexture(const std::string& path);
    AtlasTexture(std::vector<u_char> data, size_t width, size_t height,
                 GLenum format);

private:
    bool _bound = false;
    std::weak_ptr<TextureAtlas> _atlas;
    std::vector<u_char> data;
    size_t _width;
    size_t _height;
    GLenum _format;
    size_t _atlasIndex;
};

class TextureAtlas {
public:
    TextureAtlas();
    ~TextureAtlas();
    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas& operator=(const TextureAtlas&) = delete;
    bool addTexture(std::shared_ptr<AtlasTexture> pAtlasTexture);
    void build();
    void bind() const;
    void unbind() const;

private:
    bool _built = false;
    std::vector<std::shared_ptr<AtlasTexture>> _boundTextures;
    static GLuint _currentBound; // prevent useless rebinds
    GLuint _id;
    size_t _width;
    size_t _height;
    GLenum _format;
    size_t size;
};