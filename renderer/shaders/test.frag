#version 330 core

#extension GL_ARB_shading_language_420pack : require
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require

out vec4 outFragColor;

void main() {
    outFragColor = vec4(1.0, 0.0, 0.0, 1.0);
}