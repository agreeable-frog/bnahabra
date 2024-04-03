#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <sys/types.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "renderer/window.hh"
#include "renderer/pipeline.hh"
#include "renderer/buffer.hh"
#include "renderer/camera.hh"
#include "renderer/mesh.hh"
#include "renderer/object.hh"
#include "streamer/rtsp_pipeline.hh"

static std::map<std::shared_ptr<Mesh>, std::vector<Object>> makeInstanceGroups(
    const std::vector<Object>& objects) {
    std::map<std::shared_ptr<Mesh>, std::vector<Object>> instances;
    for (const auto& object : objects) {
        instances[object.getPMesh()].push_back(object);
    }
    return instances;
}

static Image readFramebufferToImage(size_t width, size_t height, int frameId) {
    u_char* data = new u_char[width * height * 3];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    Image image;
    image.data.insert(image.data.end(), &data[0],
                      &data[width * height * 3]);
    delete data;
    for (size_t line = 0; line != height / 2; ++line) {
        std::swap_ranges(image.data.begin() + 3 * width * line,
                         image.data.begin() + 3 * width * (line + 1),
                         image.data.begin() + 3 * width * (height - line - 1));
    }
    image.width = width;
    image.height = height;
    image.depth = 3 * sizeof(u_char);
    image.id = frameId;
    return image;
}

int main(int argc, char** argv) {
    gst_init(&argc, &argv);
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
    auto meshBuffer = Buffer<MeshVertex>(Target::VERTEX, Usage::STATIC);
    auto instanceBuffer = Buffer<InstanceVertex>(Target::VERTEX, Usage::STREAM);
    auto cube = std::make_shared<CubeMesh>();
    cube->registerInBuffer(meshBuffer, indexBuffer);
    indexBuffer.bufferData();
    meshBuffer.bufferData();

    pipeline.bind();
    indexBuffer.bind();
    instanceBuffer.vertexAttrib();
    meshBuffer.vertexAttrib();
    pipeline.unbind();

    Camera camera(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f), 0.1f, 50.0f, M_PI / 2);
    std::vector<Object> scene;
    scene.push_back(Object(cube, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
                           {1.0f, 1.0f, 1.0f}));

    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui_ImplGlfw_InitForOpenGL(w.getHandle(), true);

    RtspPipeline rtspPipeline("127.0.0.1", "8000", "test", w.getWidth(),
                              w.getHeight());
    // rtspPipeline.start();

    int frameId = 0;
    while (!glfwWindowShouldClose(w.getHandle())) {
        static double lastFrameTime = glfwGetTime();
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastFrameTime;
        if (deltaTime < (1.0 / 60.0)) {
            continue;
        }
        lastFrameTime = currentTime;
        glfwMakeContextCurrent(w.getHandle());
        w.resetCursorMove();
        glfwPollEvents();
        program.bind();
        pipeline.bind();
        glViewport(0, 0, w.getWidth(), w.getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.processKeys(w.getKeyStates(), deltaTime);
        camera.processMouse(w.getMouseButtonStates(), w.getCursorMove(), deltaTime);
        glUniformMatrix4fv(0, 1, GL_FALSE,
                           glm::value_ptr(camera.projection(w.getRatio())));
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(camera.view()));
        auto instanceGroups = makeInstanceGroups(scene);
        for (auto& instanceGroup : instanceGroups) {
            auto pMesh = instanceGroup.first;
            auto groupObjects = instanceGroup.second;
            instanceBuffer.clear();
            for (const auto& object : groupObjects) {
                instanceBuffer.push_back(InstanceVertex{object.model()});
            }
            instanceBuffer.bufferData();
            glDrawElementsInstanced(
                GL_TRIANGLES, pMesh->getIndexSize(), GL_UNSIGNED_INT,
                (void*)pMesh->getIndexOffset(), instanceBuffer.size());
        }
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        Image image =
            readFramebufferToImage(w.getWidth(), w.getHeight(), frameId);
        rtspPipeline.getSwapchain().present(image);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(w.getHandle());
        frameId++;
    }

    // rtspPipeline.stop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}