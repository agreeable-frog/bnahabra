#pragma once

#include "renderer/texture_atlas.hh"
#include "renderer/pipeline.hh"
#include "renderer/buffer.hh"
#include "renderer/mesh.hh"
#include "renderer/object.hh"

struct Resources {
    Program program;
    Pipeline pipeline;
    Buffer<uint32_t> indexBuffer;
    Buffer<MeshVertex> meshBuffer;
    Buffer<InstanceVertex> instanceBuffer;
    std::shared_ptr<TextureAtlas> textureAtlas;
    Resources(const std::string& vertShaderPath,
              const std::string& fragShaderPath)
        : program(vertShaderPath, fragShaderPath),
          pipeline(),
          indexBuffer(Target::INDEX, Usage::STATIC),
          meshBuffer(Target::VERTEX, Usage::STATIC),
          instanceBuffer(Target::VERTEX, Usage::STREAM),
          textureAtlas(std::make_shared<TextureAtlas>(GL_RGB)) {
    }
    void build() {
        indexBuffer.bufferData();
        meshBuffer.bufferData();
        pipeline.bind();
        indexBuffer.bind();
        instanceBuffer.vertexAttrib();
        meshBuffer.vertexAttrib();
        textureAtlas->build();
    }
};

struct Scene {
    std::vector<std::shared_ptr<Object>> objects;
    std::map<std::shared_ptr<Mesh>, std::vector<std::shared_ptr<Object>>>
        instanceGroups;
    void buildInstanceGroups() {
        for (auto object : objects) {
            instanceGroups[object->getPMesh()].push_back(object);
        }
    }
    void draw(Resources& resources, renderer::Camera& camera, float ratio) {
        glUniformMatrix4fv(0, 1, GL_FALSE,
                           glm::value_ptr(camera.projection(ratio)));
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(camera.view()));
        for (auto instanceGroup : instanceGroups) {
            auto pMesh = instanceGroup.first;
            std::vector<std::shared_ptr<Object>> groupObjects =
                instanceGroup.second;
            resources.instanceBuffer.clear();
            for (auto object : groupObjects) {
                resources.instanceBuffer.push_back(InstanceVertex{
                    object->model(), object->getPTexture()->texAtlasParams()});
            }
            resources.instanceBuffer.bufferData();
            glDrawElementsInstanced(
                GL_TRIANGLES, pMesh->getIndexSize(), GL_UNSIGNED_INT,
                (void*)(pMesh->getIndexOffset() * sizeof(uint32_t)),
                resources.instanceBuffer.size());
        }
    }
};