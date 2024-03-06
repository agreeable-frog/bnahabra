#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.hh"
#include "pipeline.hh"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

int main() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto w = Window("test", 960, 540);
    auto pVertShader = std::make_shared<ShaderModule>(
        std::string(SHADERS_PATH) + "test.vert", ShaderModule::Type::VERTEX);
    auto pFragShader = std::make_shared<ShaderModule>(
        std::string(SHADERS_PATH) + "test.frag", ShaderModule::Type::FRAGMENT);
    auto pProgram = std::make_shared<Program>(pVertShader, pFragShader);
    auto pPipeline = std::make_shared<Pipeline>(pProgram);

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
        pPipeline->bind();
        glfwMakeContextCurrent(w.getHandle());
        glViewport(0, 0, w.getWidth(), w.getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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