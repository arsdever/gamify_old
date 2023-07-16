#version 330 core

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;

uniform vec3 diffuse_color;
uniform vec3 ambient_color;
uniform vec3 specular_color;
uniform vec3 emissive_color;
uniform vec3 transparent_color;
uniform vec3 reflective_color;
uniform vec3 view_position;

out vec4 color;

struct light_t
{
    vec3 position;
    vec3 color;
    float intensity;
};

#define LIGHT_COUNT 2
light_t light[ LIGHT_COUNT ];

// user provided fragment shader which calculates the color of the fragment
// without lighting
vec4 frag();

void main()
{
    light[ 0 ].position = vec3(0.0, 0.0, 5.0);
    light[ 0 ].color = vec3(1.0, 1.0, 1.0);
    light[ 0 ].intensity = 1.0;

    light[ 1 ].position = vec3(0.0, 5.0, 0.0);
    light[ 1 ].color = vec3(1.0, 1.0, 1.0);
    light[ 1 ].intensity = 0.6;

    vec3 result = vec3(0.0);
    float shininess = 1.0;

    for (int i = 0; i < LIGHT_COUNT; ++i)
    {
        // ambient
        // vec3 ambient = light[ i ].color * ambient_color;

        // diffuse
        vec3 norm = normalize(frag_normal);
        vec3 lightDir = normalize(light[ i ].position - frag_position.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light[ i ].color * (diff * frag().rgb);

        // // specular
        // vec3 viewDir = normalize(view_position - frag_position.xyz);
        // vec3 reflectDir = reflect(-lightDir, norm);
        // float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        // vec3 specular = light[ i ].color * (spec * specular_color);

        // result += ambient + diffuse + specular;
        result += diffuse;
    }

    color = vec4(result, 1.0);
}
