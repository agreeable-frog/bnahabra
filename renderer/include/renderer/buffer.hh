#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <type_traits>

#include "renderer/vertex.hh"

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
    void bind() const {
        glBindBuffer((GLenum)_target, _id);
    }
    void unbind() {
        glBindBuffer((GLenum)_target, 0);
    }
    template <typename U = T>
    typename std::enable_if<std::is_base_of<Vertex, U>::value>::type
    vertexAttrib() {
        BindingDescriptor bindingDescriptor = T().getBindingDescriptor();
        std::vector<AttributeDescriptor> attributeDescritors =
            T().getAttributeDescriptors();
        bind();
        for (const auto& attributeDescriptor : attributeDescritors) {
            glVertexAttribPointer(
                attributeDescriptor.location, attributeDescriptor.size,
                attributeDescriptor.type, attributeDescriptor.normalized,
                bindingDescriptor.stride, (void*)attributeDescriptor.offset);
            glVertexAttribDivisor(attributeDescriptor.location,
                                  bindingDescriptor.divisor);
            glEnableVertexAttribArray(attributeDescriptor.location);
        }
        unbind();
    }

    void bufferData() {
        bind();
        glBufferData((GLenum)_target, this->size() * sizeof(T), this->data(),
                     (GLenum)_usage);
        unbind();
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