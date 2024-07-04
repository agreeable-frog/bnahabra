#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <sys/types.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "renderer/window.hh"
#include "renderer/camera.hh"
#include "streamer/rtsp_pipeline.hh"
#include "renderer/framebuffer.hh"
#include "engine.hh"
#include "drones.hh"

int main(int argc, char** argv) {
    // Init
    gst_init(&argc, &argv);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto w = Window("test", 960, 540);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui_ImplGlfw_InitForOpenGL(w.getHandle(), true);
    Resources resources = Resources(std::string(SHADERS_PATH) + "test.vert",
                                    std::string(SHADERS_PATH) + "test.frag");

    // Define drones
    drones::Swarm swarm;
    drones::Drone drone;
    drone.position = glm::vec3{0.0f, 0.0f, 0.0f};
    drone.rotation =
        glm::rotate(glm::rotate(glm::rotate(glm::mat4(1.0f), 0.0f, world::X),
                                0.0f, world::Y),
                    0.0f, world::Z);
    drones::Camera camera1{
        glm::vec3{0.0f, 0.0f, 0.0f},
        glm::rotate(glm::rotate(glm::rotate(glm::mat4(1.0f), 0.0f, world::X),
                                0.0f, world::Y),
                    0.0f, world::X),
        {960, 540, GL_RGBA}};
    drone.cameras.push_back(camera1);
    swarm.drones.push_back(drone);
    std::shared_ptr<RtspPipeline> rtspPipeline1 =
        std::make_shared<RtspPipeline>("127.0.0.1", "8000", "test",
                                       camera1.framebuffer.getWidth(),
                                       camera1.framebuffer.getHeight());
    rtspPipeline1->start();
    std::vector<std::shared_ptr<RtspPipeline>> rtspPipelines = {rtspPipeline1};
    size_t mainCameraIndex = 0;

    // Setup resources
    auto cube = std::make_shared<CubeMesh>();
    auto sphere = std::make_shared<SphereMesh>(16, 16);
    cube->registerInBuffer(resources.meshBuffer, resources.indexBuffer);
    sphere->registerInBuffer(resources.meshBuffer, resources.indexBuffer);
    auto texture = resources.textureAtlas->addTexture(
        std::string(RESOURCES_PATH) + "leaves.jpg");
    auto texture2 = resources.textureAtlas->addTexture(
        std::string(RESOURCES_PATH) + "wood2.jpg");
    resources.build();

    // Build scene
    Scene scene;
    scene.objects.push_back(std::make_shared<Object>(
        cube, texture, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f},
        glm::vec3{1.0f, 1.0f, 1.0f}));
    scene.objects.push_back(std::make_shared<Object>(
        cube, texture2, glm::vec3{3.0f, 0.0f, 0.0f},
        glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}));
    scene.objects.push_back(std::make_shared<Object>(
        sphere, texture2, glm::vec3{3.0f, 4.0f, 0.0f},
        glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}));

    // Render loop
    glfwMakeContextCurrent(w.getHandle());
    resources.program.bind();
    resources.pipeline.bind();
    int frameId = 0;
    while (!glfwWindowShouldClose(w.getHandle())) {
        // Framerate limiter
        static double lastFrameTime = glfwGetTime();
        static double lastCoutTime = lastFrameTime;
        static int lastCoutFrameId = 0;
        static float actualFps = 0.0f;
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastFrameTime;
        if (deltaTime < (1.0 / 60.0)) {
            continue;
        }
        if (currentTime - lastCoutTime > 0.5f) {
            actualFps = float(frameId - lastCoutFrameId) * 2.0f;
            lastCoutFrameId = frameId;
            lastCoutTime = currentTime;
        }
        lastFrameTime = currentTime;

        // Update scene
        w.resetCursorMove();
        glfwPollEvents();
        auto& keyStates = w.getKeyStates();
        float speed = 0.5f;
        float delta = (float)deltaTime;
        glm::vec3 forward = glm::vec4(world::X, 1.0f) * drone.rotation;
        glm::vec3 up = glm::vec4(world::Z, 1.0f) * drone.rotation;
        auto left = glm::cross(up, forward);
        if ((keyStates.find(GLFW_KEY_W) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_W)) {
            drone.position = drone.position + forward * delta * speed;
        }
        if ((keyStates.find(GLFW_KEY_A) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_A)) {
            drone.position = drone.position + left * delta * speed;
        }
        if ((keyStates.find(GLFW_KEY_D) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_D)) {
            drone.position = drone.position - left * delta * speed;
        }
        if ((keyStates.find(GLFW_KEY_S) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_S)) {
            drone.position = drone.position - forward * delta * speed;
        }
        if ((keyStates.find(GLFW_KEY_LEFT_SHIFT) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_LEFT_SHIFT)) {
            drone.position = drone.position - up * delta * speed;
        }
        if ((keyStates.find(GLFW_KEY_SPACE) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_SPACE)) {
            drone.position = drone.position + up * delta * speed;
        }
        if ((keyStates.find(GLFW_KEY_RIGHT) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_RIGHT)) {
            drone.rotation = glm::rotate(drone.rotation, +delta * speed, up);
        }
        if ((keyStates.find(GLFW_KEY_LEFT) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_LEFT)) {
            drone.rotation = glm::rotate(drone.rotation, -delta * speed, up);
        }
        if ((keyStates.find(GLFW_KEY_UP) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_UP)) {
            drone.rotation = glm::rotate(drone.rotation, +delta * speed, left);
        }
        if ((keyStates.find(GLFW_KEY_DOWN) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_DOWN)) {
            drone.rotation = glm::rotate(drone.rotation, -delta * speed, left);
        }
        if ((keyStates.find(GLFW_KEY_Q) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_Q)) {
            drone.rotation =
                glm::rotate(drone.rotation, +delta * speed, forward);
        }
        if ((keyStates.find(GLFW_KEY_E) != keyStates.cend()) &&
            keyStates.at(GLFW_KEY_E)) {
            drone.rotation =
                glm::rotate(drone.rotation, -delta * speed, forward);
        }
        scene.buildInstanceGroups();

        // Draw to window
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, w.getWidth(), w.getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer::Camera renderCamera(
            drone.position +
                glm::vec3(drone.rotation *
                          glm::vec4(drone.cameras[0].position, 1.0f)),
            glm::vec4(world::Z, 1.0f) * drone.cameras[0].rotation *
                drone.rotation,
            glm::vec4(world::X, 1.0f) * drone.cameras[0].rotation *
                drone.rotation,
            0.1f, 50.0f, M_PI / 2);
        scene.draw(resources, renderCamera, w.getRatio());
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(400.0f, 100.0f));
        ImGui::SetNextWindowBgAlpha(0.3f);
        ImGui::Begin("Debug", 0, ImGuiWindowFlags_NoDecoration);
        ImGui::Text("%f fps", actualFps);
        ImGui::Text("Drone pos %f, %f, %f", drone.position.x, drone.position.y,
                    drone.position.z);
        glm::vec3 euler = glm::eulerAngles(glm::quat(drone.rotation));
        ImGui::Text("Drone rot %f, %f, %f", euler.x, euler.y, euler.z);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Draw to framebuffers
        /*         for (size_t i = 0; i < cameras.size(); i++) {
                    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
           framebuffers[i].getFbo()); glViewport(0, 0,
           framebuffers[i].getWidth(), framebuffers[i].getHeight());
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    scene.draw(resources, cameras[i],
                               framebuffers[i].getWidth() /
                                   (float)framebuffers[i].getHeight());
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplGlfw_NewFrame();
                    ImGui::NewFrame();
                    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
                    ImGui::SetNextWindowSize(ImVec2(200.0f, 200.0f));
                    ImGui::SetNextWindowBgAlpha(0.3f);
                    ImGui::Begin("Debug", 0, ImGuiWindowFlags_NoDecoration);
                    ImGui::Text("Camera %li", i);
                    ImGui::End();
                    ImGui::Render();
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                    glBindFramebuffer(GL_READ_FRAMEBUFFER,
           framebuffers[i].getFbo()); auto data = framebuffers[i].read(); Image
           image = Image{data, framebuffers[i].getWidth(),
                              framebuffers[i].getHeight(), 3 * sizeof(u_char),
           frameId}; rtspPipelines[i]->getSwapchain().present(image);
                } */

        glfwSwapBuffers(w.getHandle());
        frameId++;
    }

    for (auto rtspPipeline : rtspPipelines) {
        rtspPipeline->stop();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}