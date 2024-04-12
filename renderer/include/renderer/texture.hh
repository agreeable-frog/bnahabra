#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <sys/types.h>

class Texture {
public:
    Texture(const std::vector<u_char>& data, size_t width, size_t height,
            GLenum format)
        : _data(data), _width(width), _height(height), _format(format) {
    }
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
    virtual void bind() = 0;
    virtual void unbind() = 0;

protected:
    std::vector<u_char> _data;
    size_t _width;
    size_t _height;
    GLenum _format;
};