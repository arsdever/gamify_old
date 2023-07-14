uniform vec3 diffuse_color;
uniform vec3 ambient_color;
uniform vec3 specular_color;
uniform vec3 emissive_color;
uniform vec3 transparent_color;
uniform vec3 reflective_color;

vec4 frag() { return vec4(diffuse_color, 1.0); }
