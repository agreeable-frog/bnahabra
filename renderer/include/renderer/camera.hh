#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 axis, glm::vec3 forward, float nearClip, float farClip, float horizontalFov);
    glm::mat4 projection(float ratio) const;
    glm::mat4 view() const;
private:
    glm::vec3 _position;
    glm::vec3 _axis;
    glm::vec3 _up;
    glm::vec3 _forward;
    float _nearClip;
    float _farClip;
    float _horizontalFov;
};