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