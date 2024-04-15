#version 330 core

#extension GL_ARB_shading_language_420pack : require
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require
#extension GL_ARB_explicit_uniform_location : require

layout(location = 0) in vec2 uv;
layout(location = 1) in vec4 texAtlasParams;

layout(location = 0) out vec4 outFragColor;

uniform sampler2DArray textureAtlas;
layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 view;

void main() {
    outFragColor = texAtlasParams.x *
                   texture(textureAtlas, texAtlasParams.yzw * vec3(uv, 1.0));
}