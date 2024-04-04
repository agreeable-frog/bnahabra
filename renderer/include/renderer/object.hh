#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "renderer/mesh.hh"

struct InstanceVertex : public Vertex {
    BindingDescriptor getBindingDescriptor() const override;
    std::vector<AttributeDescriptor> getAttributeDescriptors() const override;
    InstanceVertex(glm::mat4 model, glm::vec3 albedo);
    InstanceVertex();
    glm::mat4 model;
    glm::vec3 albedo;
};

class Object {
public:
    Object(std::shared_ptr<Mesh> pMesh, glm::vec3 pos, glm::vec3 rot, glm::vec3 size);
    glm::mat4 model() const;
    std::shared_ptr<Mesh> getPMesh() const {
        return _pMesh;
    }
    void setAlbedo(glm::vec3 albedo) {
        _albedo = albedo;
    }
    glm::vec3 getAlbedo() const {
        return _albedo;
    }
private:
    std::shared_ptr<Mesh> _pMesh;
    glm::vec3 _pos;
    glm::vec3 _rot;
    glm::vec3 _scale;
    glm::vec3 _albedo;
};

