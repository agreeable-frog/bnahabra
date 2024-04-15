#version 330 core

#extension GL_ARB_shading_language_420pack : require
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in mat4 model;
layout(location = 7) in vec4 texAtlasParams;

layout(location = 0) out vec2 uvOut;
layout(location = 1) out vec4 texAtlasParamsOut;

layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 view;

void main() {
    gl_Position = proj * view * model * vec4(pos, 1.0);
    uvOut = uv;
    texAtlasParamsOut = texAtlasParams;
}