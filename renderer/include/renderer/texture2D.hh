#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <memory>

class Texture2D {
public:
    static std::shared_ptr<Texture2D> fromImage(const std::string& path);
    Texture2D(uint width, uint height, std::vector<u_char> data, GLenum format);
    Texture2D(const Texture2D& other) = delete;
    Texture2D& operator=(const Texture2D& other) = delete;
    ~Texture2D();
    void bind() const;
    void unbind() const;
    uint getId() const {
        return _id;
    }
    uint getWidth() const {
        return _width;
    }
    uint getHeight() const {
        return _height;
    }
    GLenum getFormat() const {
        return _format;
    }

private:
    GLuint _id;
    uint _width;
    uint _height;
    GLenum _format;
};