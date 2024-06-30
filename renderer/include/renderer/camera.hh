#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>

namespace renderer {
class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 axis, glm::vec3 forward,
           float nearClip, float farClip, float horizontalFov);
    glm::mat4 projection(float ratio) const;
    glm::mat4 view() const;
    void processKeys(const std::map<int, bool>& keyStates, float delta);
    void processMouse(const std::map<int, bool>& mouseButtonStates,
                      const std::array<int, 2>& cursorMove, float delta);
    void move(glm::vec3 move);
    void rotate(float yaw, float pitch, float roll);

private:
    glm::vec3 _position;
    glm::vec3 _axis;
    glm::vec3 _up;
    glm::vec3 _forward;
    float _nearClip;
    float _farClip;
    float _horizontalFov;
};
} // namespace renderer