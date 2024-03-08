#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>
#include <string>

class Vertex {
public:
    static VertexDescriptor descriptor;
};

class VertexDescriptor {

};

class Buffer {
public:
    enum class Type {
        VERTEX = GL_ARRAY_BUFFER,
        INDEX = GL_ELEMENT_ARRAY_BUFFER
    };

    enum class Usage {
        STATIC = GL_STATIC_DRAW,
        STREAM = GL_STREAM_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW
    };
    Buffer(Type type, Usage usage);
    Buffer(const Buffer& other);
    Buffer& operator=(const Buffer& other);
    ~Buffer();
    void data(void* data, size_t size);

private:
    Type _type;
    Usage _usage;
    GLuint _id;
    uint _pipelineUid;
    size_t _size;
};