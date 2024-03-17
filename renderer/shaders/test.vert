#version 330 core

#extension GL_ARB_shading_language_420pack : require
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) in vec3 pos;

layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 view;

void main() {
    gl_Position = proj * view * vec4(pos, 1.0);
}