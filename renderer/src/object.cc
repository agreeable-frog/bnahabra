#include "renderer/object.hh"

#include <glm/gtx/transform.hpp>

BindingDescriptor InstanceVertex::getBindingDescriptor() const {
    BindingDescriptor bindingDescriptor;
    bindingDescriptor.stride = sizeof(InstanceVertex);
    bindingDescriptor.divisor = 1;
    return bindingDescriptor;
}

std::vector<AttributeDescriptor> InstanceVertex::getAttributeDescriptors()
    const {
    std::vector<AttributeDescriptor> attributeDescriptors(4, AttributeDescriptor());

    attributeDescriptors[0].location = 3;
    attributeDescriptors[0].size = 4;
    attributeDescriptors[0].type = GL_FLOAT;
    attributeDescriptors[0].normalized = GL_FALSE;
    attributeDescriptors[0].offset = offsetof(InstanceVertex, model); // OK

    attributeDescriptors[1].location = 4;
    attributeDescriptors[1].size = 4;
    attributeDescriptors[1].type = GL_FLOAT;
    attributeDescriptors[1].normalized = GL_FALSE;
    attributeDescriptors[1].offset = offsetof(InstanceVertex, model) + 4 * sizeof(GL_FLOAT); // OK

    attributeDescriptors[2].location = 5;
    attributeDescriptors[2].size = 4;
    attributeDescriptors[2].type = GL_FLOAT;
    attributeDescriptors[2].normalized = GL_FALSE;
    attributeDescriptors[2].offset = offsetof(InstanceVertex, model) + 8 * sizeof(GL_FLOAT); // OK

    attributeDescriptors[3].location = 6;
    attributeDescriptors[3].size = 4;
    attributeDescriptors[3].type = GL_FLOAT;
    attributeDescriptors[3].normalized = GL_FALSE;
    attributeDescriptors[3].offset = offsetof(InstanceVertex, model) + 12 * sizeof(GL_FLOAT); // OK

    return attributeDescriptors;
}

InstanceVertex::InstanceVertex(glm::mat4 model) : model(model) {
}

InstanceVertex::InstanceVertex() {
}

Object::Object(std::shared_ptr<Mesh> pMesh, glm::vec3 pos, glm::vec3 rot,
               glm::vec3 scale)
    : _pMesh(pMesh), _pos(pos), _rot(rot), _scale(scale) {
}

glm::mat4 Object::model() const {
    return glm::translate(_pos) *
           glm::rotate(_rot[2], glm::vec3{0.0f, 0.0f, 1.0f}) *
           glm::rotate(_rot[1], glm::vec3{0.0f, 1.0f, 0.0f}) *
           glm::rotate(_rot[0], glm::vec3{1.0f, 0.0f, 0.0f}) *
           glm::scale(_scale);
}