#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <memory>

#include "streamer/rtsp_pipeline.hh"
#include "renderer/framebuffer.hh"

namespace drones {
struct Camera {
    std::string streamMount;
    Framebuffer framebuffer;
    glm::vec3 positionOnDrone;
    glm::vec3 rotationOnDrone;
};

struct Drone {
    glm::vec3 position;
    glm::vec3 up;
    const glm::vec3 axis = glm::vec3{0.0f, 0.0f, 1.0f};
    glm::vec3 forward;
    std::vector<Camera> cameras;
    std::string ip;
    std::string port;
    std::shared_ptr<RtspPipeline> streamer;
    void build() { // temporary while RtspPipeline is mono-mount
        streamer = std::make_shared<RtspPipeline>(
            ip, port, cameras[0].streamMount, cameras[0].framebuffer.getWidth(),
            cameras[0].framebuffer.getHeight());
    }
};

struct Swarm {
    std::vector<Drone> drones;
};

} // namespace drones