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
}

namespace drones {
struct Camera {
    glm::vec3 position;
    glm::vec3 rotation;


    Framebuffer framebuffer;
    std::string streamMountPoint;
};

struct Drone {
    glm::vec3 position;
    void move(glm::vec3 move, glm::vec3 speed);
    glm::vec3 rotation;
    void rotate(glm::vec3 rotation);
    std::vector<Camera> cameras;


    std::string ip;
    std::string port;
    std::shared_ptr<RtspPipeline> streamer;
    void build() { // temporary while RtspPipeline is mono-mount
        streamer = std::make_shared<RtspPipeline>(
            ip, port, cameras[0].streamMountPoint, cameras[0].framebuffer.getWidth(),
            cameras[0].framebuffer.getHeight());
    }
};

struct Swarm {
    std::vector<Drone> drones;
};

} // namespace drones