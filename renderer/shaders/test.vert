#version 330 core

#extension GL_ARB_shading_language_420pack : require
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_separate_shader_objects : require
#extension GL_ARB_explicit_uniform_location : require

vec3 coords[3] = {vec3(0.0, 1.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0)};


void main() {
    gl_Position = vec4(coords[gl_VertexID], 1.0);
}