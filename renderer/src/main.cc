#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.hh"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

int main() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto w = Window("test", 960, 540);

    ImGui_ImplGlfw_InitForOpenGL(w.getHandle(), true);
    ImGui_ImplOpenGL3_Init();

    while (!glfwWindowShouldClose(w.getHandle())) {
        static double lastFrameTime = glfwGetTime();
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastFrameTime;
        if (deltaTime < (1.0 / 60.0)) {
            continue;
        }
        lastFrameTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        float test;
        ImGui::SliderFloat("HFOV", &test, 0.0f, 1.0f);

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