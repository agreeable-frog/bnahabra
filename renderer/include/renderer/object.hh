#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "renderer/mesh.hh"
#include "renderer/texture.hh"

struct InstanceVertex : public Vertex {
    BindingDescriptor getBindingDescriptor() const override;
    std::vector<AttributeDescriptor> getAttributeDescriptors() const override;
    InstanceVertex(glm::mat4 model, glm::vec4 texAtlasParams);
    InstanceVertex();
    glm::mat4 model;
    glm::vec4 texAtlasParams;
};

class Object {
public:
    Object(std::shared_ptr<Mesh> pMesh, std::shared_ptr<Texture> pTexture,
           glm::vec3 pos, glm::vec3 rot, glm::vec3 size);
    glm::mat4 model() const;
    std::shared_ptr<Mesh> getPMesh() const {
        return _pMesh;
    }
    std::shared_ptr<Texture> getPTexture() const {
        return _pTexture;
    }

private:
    std::shared_ptr<Mesh> _pMesh;
    std::shared_ptr<Texture> _pTexture;
    glm::vec3 _pos;
    glm::vec3 _rot;
    glm::vec3 _scale;
};
