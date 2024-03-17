#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

struct BindingDescriptor {
    GLsizei stride;
    GLuint divisor;
};

struct AttributeDescriptor {
    GLuint location;
    GLint size;
    GLenum type;
    GLboolean normalized;
    size_t offset;
};

struct Vertex {
    virtual BindingDescriptor getBindingDescriptor() = 0;
    virtual std::vector<AttributeDescriptor> getAttributeDescriptors() = 0;
};