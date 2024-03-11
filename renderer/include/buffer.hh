#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>
#include <string>
#include <vector>

namespace vertex {
    struct BindingDescriptor {
        GLuint binding;
        GLsizei stride;
        GLuint divisor;
    };

    struct AttributeDescriptor {
        GLuint location;
        GLint size;
        GLenum type;
        size_t offset;
    };
};

template <class T>
class Buffer : public std::vector<T> {
public:
    enum class Target {
        VERTEX = GL_ARRAY_BUFFER,
        INDEX = GL_ELEMENT_ARRAY_BUFFER
    };

    enum class Usage {
        STATIC = GL_STATIC_DRAW,
        STREAM = GL_STREAM_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW
    };
    Buffer(Target target, Usage usage);
    Buffer(const Buffer<T>& other);
    Buffer<T>& operator=(const Buffer<T>& other);
    ~Buffer();
    void bind() const;

private:
    Target _target;
    Usage _usage;
    GLuint _id;
    uint _pipelineUid = 0;

    void init();
};