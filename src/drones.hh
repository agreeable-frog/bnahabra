#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <memory>

#include "streamer/rtsp_pipeline.hh"
#include "renderer/framebuffer.hh"

namespace world {
const glm::vec3 X = {1.0f, 0.0f, 0.0f};
const glm::vec3 Y = {0.0f, 1.0f, 0.0f};
const glm::vec3 Z = {0.0f, 0.0f, 1.0f};
} // namespace world

namespace drones {
struct Camera {
    glm::vec3 position; // position relative to center of the drone
    glm::mat4 rotation; // rotation relative to the drone's coordinate system

    std::shared_ptr<Framebuffer> pFramebuffer;
    std::string streamMountPoint;
};

struct Drone {
    glm::vec3 position; // position relative to center of world
    glm::mat4 rotation; // rotation relative to the world's coordinate system
    std::vector<std::shared_ptr<Camera>> cameras;

    std::string ip;
    std::string port;
    std::shared_ptr<RtspPipeline> streamer;
    void build() { // temporary while RtspPipeline is mono-mount
        streamer = std::make_shared<RtspPipeline>(
            ip, port, cameras[0]->streamMountPoint,
            cameras[0]->pFramebuffer->getWidth(),
            cameras[0]->pFramebuffer->getHeight());
        streamer->start();
    }
};

struct Swarm {
    std::vector<std::shared_ptr<Drone>> drones;
};

} // namespace drones