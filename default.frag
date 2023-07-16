uniform vec3 diffuse_color;
uniform sampler2D texture_diffuse;
uniform vec3 ambient_color;
uniform vec3 specular_color;
uniform vec3 emissive_color;
uniform vec3 transparent_color;
uniform vec3 reflective_color;

in vec2 frag_uv;

vec4 frag()
{
    vec4 diffuse = texture2D(texture_diffuse, frag_uv);
    return diffuse;
}
