#include "mesh.hh"

#include <algorithm>
#include <iostream>

BindingDescriptor MeshVertex::getBindingDescriptor() {
    BindingDescriptor bindingDescriptor;
    bindingDescriptor.stride = sizeof(MeshVertex);
    bindingDescriptor.divisor = 0;
    return bindingDescriptor;
}
std::vector<AttributeDescriptor> MeshVertex::getAttributeDescriptors() {
    std::vector<AttributeDescriptor> attributeDescriptors(
        3, AttributeDescriptor());

    attributeDescriptors[0].location = 0;
    attributeDescriptors[0].size = 3;
    attributeDescriptors[0].type = GL_FLOAT;
    attributeDescriptors[0].normalized = GL_FALSE;
    attributeDescriptors[0].offset = offsetof(MeshVertex, pos);

    attributeDescriptors[1].location = 0;
    attributeDescriptors[1].size = 3;
    attributeDescriptors[1].type = GL_FLOAT;
    attributeDescriptors[1].normalized = GL_FALSE;
    attributeDescriptors[1].offset = offsetof(MeshVertex, normal);

    attributeDescriptors[2].location = 0;
    attributeDescriptors[2].size = 2;
    attributeDescriptors[2].type = GL_FLOAT;
    attributeDescriptors[2].normalized = GL_FALSE;
    attributeDescriptors[2].offset = offsetof(MeshVertex, uv);

    return attributeDescriptors;
}

MeshVertex::MeshVertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv)
    : pos(pos), normal(normal), uv(uv) {
}

MeshVertex::MeshVertex() {
}

void Mesh::registerInBuffer(std::vector<MeshVertex>& vertexBuffer, std::vector<uint32_t>& indexBuffer) {
    _indexOffset = indexBuffer.size();
    auto vertexBufferSize = vertexBuffer.size();

    vertexBuffer.insert(vertexBuffer.end(), _vertices.begin(), _vertices.end());

    std::vector<uint32_t> indicesOffsetted = std::vector<uint32_t>(_indices.size());
    std::transform(_indices.begin(), _indices.end(), indicesOffsetted.begin(),
                   [&, this](uint32_t i) { return i + vertexBufferSize; });
    indexBuffer.insert(indexBuffer.end(), indicesOffsetted.begin(), indicesOffsetted.end());
}

CubeMesh::CubeMesh() : Mesh() {
    _vertices.push_back(MeshVertex{{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}});
    _vertices.push_back(MeshVertex{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace1 = {0, 1, 3, 1, 2, 3};
    _indices.insert(_indices.end(), indexFace1.begin(), indexFace1.end());

    _vertices.push_back(MeshVertex{{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}});
    _vertices.push_back(MeshVertex{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace2 = {4, 5, 7, 5, 6, 7};
    _indices.insert(_indices.end(), indexFace2.begin(), indexFace2.end());

    _vertices.push_back(MeshVertex{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}});
    _vertices.push_back(MeshVertex{{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}});
    _vertices.push_back(MeshVertex{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}});
    std::vector<uint32_t> indexFace3 = {8, 9, 11, 9, 10, 11};
    _indices.insert(_indices.end(), indexFace3.begin(), indexFace3.end());

    _vertices.push_back(MeshVertex{{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}});
    _vertices.push_back(MeshVertex{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace4 = {12, 13, 15, 13, 14, 15};
    _indices.insert(_indices.end(), indexFace4.begin(), indexFace4.end());

    _vertices.push_back(MeshVertex{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}});
    _vertices.push_back(MeshVertex{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace5 = {16, 17, 19, 17, 18, 19};
    _indices.insert(_indices.end(), indexFace5.begin(), indexFace5.end());

    _vertices.push_back(MeshVertex{{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}});
    _vertices.push_back(MeshVertex{{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}});
    _vertices.push_back(MeshVertex{{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace6 = {20, 21, 23, 21, 22, 23};
    _indices.insert(_indices.end(), indexFace6.begin(), indexFace6.end());
}

SphereMesh::SphereMesh(size_t nRings, size_t nSegments) : Mesh() {
    for (size_t j = 0; j < nSegments; j++) {
        float theta = 2 * M_PI * float(j + 1) / float(nSegments);
        _vertices.push_back(MeshVertex{glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f},
                                   glm::vec2{0.0f, theta / (2 * M_PI)}});
    }
    for (size_t i = 0; i < nRings; i++) {
        float phi = M_PI * float(i + 1) / float(nRings);
        for (size_t j = 0; j < nSegments; j++) {
            float theta = 2 * M_PI * float(j + 1) / float(nSegments);
            float x = std::sin(phi) * std::cos(theta);
            float y = std::cos(phi);
            float z = std::sin(phi) * std::sin(theta);
            _vertices.push_back(MeshVertex{glm::vec3{x, y, z}, glm::normalize(glm::vec3{x, y, z}),
                                       glm::vec2{phi / M_PI, theta / (2 * M_PI)}});
        }
        auto v = _vertices[_vertices.size() - 1];
        v.uv.y = 0.0f;
        _vertices.push_back(v);
    }

    for (size_t i = 0; i < nRings + 1; i++) {
        for (size_t j = 0; j < nSegments; j++) {
            size_t k = i * nSegments + j;
            size_t knext = i * nSegments + (j + 1);

            _indices.push_back(k);
            _indices.push_back(knext + nSegments);
            _indices.push_back(k + nSegments);

            _indices.push_back(knext + nSegments);
            _indices.push_back(k);
            _indices.push_back(knext);
        }
    }
}

SkySphereMesh::SkySphereMesh(size_t nRings, size_t nSegments) : Mesh() {
    for (size_t j = 0; j < nSegments; j++) {
        float theta = 2 * M_PI * float(j + 1) / float(nSegments);
        _vertices.push_back(MeshVertex{glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f},
                                   glm::vec2{0.0f, theta / (2 * M_PI)}});
    }
    for (size_t i = 0; i < nRings; i++) {
        float phi = M_PI * float(i + 1) / float(nRings);
        for (size_t j = 0; j < nSegments; j++) {
            float theta = 2 * M_PI * float(j + 1) / float(nSegments);
            float x = std::sin(phi) * std::cos(theta);
            float y = std::cos(phi);
            float z = std::sin(phi) * std::sin(theta);
            _vertices.push_back(MeshVertex{glm::vec3{x, y, z}, glm::normalize(glm::vec3{x, y, z}),
                                       glm::vec2{phi / M_PI, theta / (2 * M_PI)}});
        }
        auto v = _vertices[_vertices.size() - 1];
        v.uv.y = 0.0f;
        _vertices.push_back(v);
    }

    for (size_t i = 0; i < nRings + 1; i++) {
        for (size_t j = 0; j < nSegments; j++) {
            size_t k = i * nSegments + j;
            size_t knext = i * nSegments + (j + 1);

            _indices.push_back(k);
            _indices.push_back(k + nSegments);
            _indices.push_back(knext + nSegments);

            _indices.push_back(knext + nSegments);
            _indices.push_back(knext);
            _indices.push_back(k);
        }
    }
}

PlaneMesh::PlaneMesh(glm::vec3 up, glm::vec3 dir, float ratio) : Mesh() {
    up = glm::normalize(up);
    dir = glm::normalize(dir - glm::dot(dir, up) / glm::length(up) * up); // Gram–Schmidt
    glm::vec3 dir2 = glm::cross(up, dir);
    _vertices.push_back(MeshVertex{glm::vec3{-dir + dir2}, up, {0.0f, 0.0f}});
    _vertices.push_back(MeshVertex{glm::vec3{-dir - dir2}, up, {1.0f, 0.0f}});
    _vertices.push_back(MeshVertex{glm::vec3{dir - dir2}, up, {1.0f, 1.0f}});
    _vertices.push_back(MeshVertex{glm::vec3{dir + dir2}, up, {0.0f, 1.0f}});
    std::vector<uint32_t> index = {0, 1, 2, 0, 2, 3};
    _indices.insert(_indices.end(), index.begin(), index.end());
}

/* Mesh Mesh::loadFromOBJ(const std::string& path) {
    Mesh mesh;
    objl::Loader loader;
    loader.LoadFile(path);
    for (const auto& vertex : loader.LoadedMeshes[0].Vertices) {
        mesh._vertices.push_back(
            MeshVertex{glm::vec3{vertex.Position.X, vertex.Position.Y, vertex.Position.Z},
                   glm::vec3{vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z},
                   glm::vec2{vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y}});
    }

    mesh._indices = loader.LoadedMeshes[0].Indices;
    std::cout << "indices size : " << mesh._indices.size() << '\n';

    return mesh;
} */