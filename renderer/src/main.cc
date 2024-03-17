#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.hh"
#include "pipeline.hh"
#include "buffer.hh"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

struct TestVertex : public Vertex {
    glm::vec3 _pos;
    BindingDescriptor getBindingDescriptor() override {
        BindingDescriptor bindingDescriptor;
        bindingDescriptor.stride = sizeof(TestVertex);
        bindingDescriptor.divisor = 0;
        return bindingDescriptor;
    }
    std::vector<AttributeDescriptor> getAttributeDescriptors() override {
        std::vector<AttributeDescriptor> attributeDescriptors(
            1, AttributeDescriptor());

        attributeDescriptors[0].location = 0;
        attributeDescriptors[0].size = 3;
        attributeDescriptors[0].type = GL_FLOAT;
        attributeDescriptors[0].normalized = GL_FALSE;
        attributeDescriptors[0].offset = offsetof(TestVertex, _pos);

        return attributeDescriptors;
    }

    TestVertex(glm::vec3 pos) : _pos(pos) {
    }

    TestVertex() {
    }
};

int main() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto w = Window("test", 960, 540);
    auto program = Program(
        std::make_shared<ShaderModule>(std::string(SHADERS_PATH) + "test.vert",
                                       ShaderModule::Type::VERTEX),
        std::make_shared<ShaderModule>(std::string(SHADERS_PATH) + "test.frag",
                                       ShaderModule::Type::FRAGMENT));
    auto pipeline = Pipeline();

    auto indexBuffer = Buffer<uint32_t>(Target::INDEX, Usage::STATIC);
    indexBuffer.insert(indexBuffer.end(), {0, 1, 2});
    indexBuffer.bufferData();
    auto meshBuffer = Buffer<TestVertex>(Target::VERTEX, Usage::STATIC);
    meshBuffer.insert(meshBuffer.end(), {{glm::vec3(0.0f, 1.0f, 0.0f)},
                                         {glm::vec3(-1.0f, 0.0f, 0.0f)},
                                         {glm::vec3(1.0f, 0.0f, 0.0f)}});
    meshBuffer.bufferData();

    pipeline.bind();
    indexBuffer.bind();
    meshBuffer.vertexAttrib();
    pipeline.unbind();

    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(w.getHandle(), true);

    while (!glfwWindowShouldClose(w.getHandle())) {
        static double lastFrameTime = glfwGetTime();
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastFrameTime;
        if (deltaTime < (1.0 / 60.0)) {
            continue;
        }
        lastFrameTime = currentTime;
        program.bind();
        pipeline.bind();
        glfwMakeContextCurrent(w.getHandle());
        glViewport(0, 0, w.getWidth(), w.getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(w.getHandle());
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}