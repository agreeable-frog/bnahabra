#version 330 core

#extension GL_ARB_shading_language_420pack : require
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 outFragColor;

uniform sampler2D objTexture;

void main() {
    outFragColor = vec4(1.0, 0.0, 0.0, 1.0);
}