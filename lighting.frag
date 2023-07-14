#version 330 core

in vec3 frag_normal;
in vec2 frag_uv;

out vec4 color;

vec3 light_dir = vec3(0.0, 0.0, 1.0);
vec3 light_color = vec3(1.0, 1.0, 1.0);

vec4 frag();

void main()
{
    vec3 light_intensity = vec3(1.0, 1.0, 1.0);
    vec3 lightness =
        light_intensity * max(dot(normalize(frag_normal), light_dir), 0.0);
    vec3 diffuse = light_color * lightness;

    color = vec4(diffuse, 1.0) * frag();
}
