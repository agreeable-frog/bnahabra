#include "renderer/camera.hh"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 axis, glm::vec3 forward,
               float nearClip, float farClip, float horizontalFov)
    : _position(position),
      _axis(glm::normalize(axis)),
      _up(glm::normalize(axis)),
      _forward(glm::normalize(forward)),
      _nearClip(nearClip),
      _farClip(farClip),
      _horizontalFov(horizontalFov) {
}

glm::mat4 Camera::projection(float ratio) const {
    return glm::perspective(_horizontalFov, ratio, _nearClip, _farClip);
}

glm::mat4 Camera::view() const {
    return glm::lookAt(_position, _position + _forward, _up);
}

void Camera::processKeys(const std::map<int, bool>& keyStates, float delta) {
    float speed = 0.5f;
    glm::vec3 left = glm::cross(_up, _forward);
    if ((keyStates.find(GLFW_KEY_W) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_W) == GLFW_PRESS) {
        move(_forward * delta * speed);
    }
    if ((keyStates.find(GLFW_KEY_A) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_A) == GLFW_PRESS) {
        move(left * delta * speed);
    }
    if ((keyStates.find(GLFW_KEY_D) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_D) == GLFW_PRESS) {
        move(-left * delta * speed);
    }
    if ((keyStates.find(GLFW_KEY_S) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_S) == GLFW_PRESS) {
        move(-_forward * delta * speed);
    }
    if ((keyStates.find(GLFW_KEY_LEFT_SHIFT) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        move(-_axis * delta * speed);
    }
    if ((keyStates.find(GLFW_KEY_SPACE) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_SPACE) == GLFW_PRESS) {
        move(_axis * delta * speed);
    }
}

void Camera::move(glm::vec3 move) {
    _position += move;
}