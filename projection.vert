#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

vec4 vert(vec3 position);

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 frag_normal;
out vec2 frag_uv;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    frag_normal = normal;
    frag_uv = uv;
}
