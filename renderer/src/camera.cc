#include "renderer/camera.hh"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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
        keyStates.at(GLFW_KEY_W)) {
        move(_forward * delta * speed);
    }
    if ((keyStates.find(GLFW_KEY_A) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_A)) {
        move(left * delta * speed);
    }
    if ((keyStates.find(GLFW_KEY_D) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_D)) {
        move(-left * delta * speed);
    }
    if ((keyStates.find(GLFW_KEY_S) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_S)) {
        move(-_forward * delta * speed);
    }
    if ((keyStates.find(GLFW_KEY_LEFT_SHIFT) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_LEFT_SHIFT)) {
        move(-_axis * delta * speed);
    }
    if ((keyStates.find(GLFW_KEY_SPACE) != keyStates.cend()) &&
        keyStates.at(GLFW_KEY_SPACE)) {
        move(_axis * delta * speed);
    }
}

void Camera::processMouse(const std::map<int, bool>& mouseButtonStates,
                          const std::array<int, 2>& cursorMove, float delta) {
    float speed = -0.1f;
    if (mouseButtonStates.find(GLFW_MOUSE_BUTTON_LEFT) !=
            mouseButtonStates.cend() &&
        mouseButtonStates.at(GLFW_MOUSE_BUTTON_LEFT)) {
        rotate(cursorMove[0] * delta * speed, cursorMove[1] * delta * speed,
               0.0f);
    }
}

void Camera::move(glm::vec3 move) {
    _position += move;
}

void Camera::rotate(float yaw, float pitch, float roll) {
    glm::mat4 yawRot = glm::rotate(glm::mat4(1.0f), yaw, _axis);
    _forward = yawRot * glm::vec4(_forward, 1.0f);
    _up = yawRot * glm::vec4(_up, 1.0f);
    glm::mat4 pitchRot =
        glm::rotate(glm::mat4(1.0f), pitch, glm::cross(_forward, _up));
    _forward = pitchRot * glm::vec4(_forward, 1.0f);
    _up = pitchRot * glm::vec4(_up, 1.0f);
    glm::mat4 rollRot = glm::rotate(glm::mat4(1.0f), roll, _forward);
    _up = rollRot * glm::vec4(_up, 1.0f);
}