#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <type_traits>

struct BindingDescriptor {
    GLsizei stride;
    GLuint divisor;
};

struct AttributeDescriptor {
    GLuint location;
    GLint size;
    GLenum type;
    size_t offset;
};

struct Vertex {
    virtual BindingDescriptor getBindingDescriptor() = 0;
    virtual std::vector<AttributeDescriptor> getAttributeDescriptors() = 0;
};

enum class Target { VERTEX = GL_ARRAY_BUFFER, INDEX = GL_ELEMENT_ARRAY_BUFFER };

enum class Usage {
    STATIC = GL_STATIC_DRAW,
    STREAM = GL_STREAM_DRAW,
    DYNAMIC = GL_DYNAMIC_DRAW
};

template <class T>
class Buffer : public std::vector<T> {
public:
    Target getTarget() const {
        return _target;
    }

    Usage getUsage() const {
        return _usage;
    }

    Buffer(Target target, Usage usage) {
        init(target, usage);
    }

    Buffer(const Buffer<T>& other) : std::vector<T>(other) {
        init(other.getTarget(), other.getUsage());
    }

    Buffer<T>& operator=(const Buffer<T>& other) {
        if (&other == this) {
            return *this;
        }
        glDeleteBuffers(1, &_id);
        std::vector<T>::operator=(other);
        init(other.getTarget(), other.getUsage());
        return *this;
    }
    ~Buffer() {
        glDeleteBuffers(1, &_id);
    }
    void bind() {
        glBindBuffer((GLenum)_target, _id);
    }
    void unbind() {
        glBindBuffer((GLenum)_target, 0);
    }
    template <typename U = T>
    typename std::enable_if<std::is_base_of<Vertex, U>::value>::type
    vertexAttrib() {
    }

private:
    Target _target;
    Usage _usage;
    GLuint _id;
    uint _pipelineUid = 0;

    void init(Target target, Usage usage) {
        _target = target;
        _usage = usage;
        _pipelineUid = 0;
        glGenBuffers(1, &_id);
    }
};

/* template <class T>
void Buffer<T>::bind() {
    BindingDescriptor bindingDescriptor = T().getBindingDescriptor();
    std::vector<AttributeDescriptor> attributeDescriptors =
        T().getAttributeDescriptors();
    glBindBuffer((GLenum)target, _id);
    for (const auto& attributeDescriptor : attributeDescriptors) {
    }
} */