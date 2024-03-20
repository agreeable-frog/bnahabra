#pragma once

#include <glm/glm.hpp>
#include <string>

#include "vertex.hh"
#include "buffer.hh"

struct MeshVertex : public Vertex {
    BindingDescriptor getBindingDescriptor() const override;
    std::vector<AttributeDescriptor> getAttributeDescriptors() const override;
    MeshVertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv);
    MeshVertex();
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
public:
    size_t getIndexOffset() const {
        return _indexOffset;
    }
    size_t getIndexSize() const {
        return _indices.size();
    }
    void registerInBuffer(Buffer<MeshVertex>& vertexBuffer,
                          Buffer<uint32_t>& indexBuffer);
    // static Mesh loadFromOBJ(const std::string& path);
protected:
    Mesh() {
        static int counter = 0;
        _uid = counter++;
    }
    int _uid;
    std::vector<MeshVertex> _vertices;
    std::vector<uint32_t> _indices;
    size_t _indexOffset;
};

class CubeMesh : public Mesh {
public:
    CubeMesh();
};

class SphereMesh : public Mesh {
public:
    SphereMesh(size_t nRings, size_t nSegments);
};

class SkySphereMesh : public Mesh {
public:
    SkySphereMesh(size_t nRings, size_t nSegments);
};

class PlaneMesh : public Mesh {
public:
    PlaneMesh(glm::vec3 up, glm::vec3 dir, float ratio);
};