
constexpr const char* vertex_shader = R"glsl(
#version 460

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_uv;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform mat3 u_normal;

void main() {
    vec4 position = u_modelview * vec4(a_position, 1.);
    vec3 normal = u_normal * a_normal;

    v_position = position.xyz;
    v_normal = normal;
    v_uv = a_uv;

    gl_Position = u_projection * position;
}
)glsl";

constexpr const char* prong_shader = R"glsl(
#version 460

const int LIGHT_MAX = 16;
const int LIGHT_AMBIENT = 0;
const int LIGHT_POINT = 1;
const int LIGHT_DIRECTIONAL = 2;

struct Ligth {
    vec3 color;
    float strength;
    vec3 pos_or_dir;
    int type;
};

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform mat3 u_normal;

uniform vec3 u_amibent;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform vec3 u_emissive;
uniform float u_shininess;

uniform sampler2D u_ambient_texture;
uniform sampler2D u_diffuse_texture;
uniform sampler2D u_specular_texture;
uniform sampler2D u_emissive_texture;

uniform bool u_use_ambient_texture;
uniform bool u_use_diffuse_texture;
uniform bool u_use_specular_texture;
uniform bool u_use_emissive_texture;

uniform int u_lights_count;
uniform Ligth u_lights[LIGHT_MAX];

vec3 blinnPhong(vec3 w_i, vec3 l_i, vec3 n, vec3 k_d, vec3 k_s, float shininess) {
    vec3 w_o = vec3(0, 0, 1);
    vec3 half_vector = (w_o + w_i) / 2;
    
    vec3 result_diffuse = k_d * max(dot(w_i, n), 0);
    vec3 result_specular = k_s * pow(max(dot(half_vector, n), 0), shininess);

    return l_i * ( result_diffuse + result_specular );
}

vec3 calculateLight(Ligth light, vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal) {
    if (light.type == LIGHT_AMBIENT) {
        return ambient * light.color * light.strength;
    } else if (light.type == LIGHT_POINT) {
        vec3 direction = normalize(light.pos_or_dir - v_position);
        float strength = 1.0 / pow(length(light.pos_or_dir - v_position), 2);
        return blinnPhong(direction, light.color * light.strength * strength, normal, diffuse, specular, 32.);
    } else if (light.type == LIGHT_DIRECTIONAL) {
        vec3 direction = normalize(-light.pos_or_dir);
        return blinnPhong(direction, light.color * light.strength, normal, diffuse, specular, 32.);
    } 
    return vec3(0);
}

void main() {
    vec3 ambient = u_amibent;
    vec3 diffuse = u_diffuse;
    vec3 specular = u_specular;
    vec3 emissive = u_emissive;
    float shininess = u_shininess;

    if (u_use_ambient_texture) {
        vec3 tex = texture(u_ambient_texture, v_uv).rgb;
        ambient *= tex;
    }
    if (u_use_diffuse_texture) {
        vec3 tex = texture(u_diffuse_texture, v_uv).rgb;
        diffuse *= tex;
    }
    if (u_use_specular_texture) {
        vec3 tex = texture(u_specular_texture, v_uv).rgb;
        specular *= tex;
    }
    if (u_use_emissive_texture) {
        vec3 tex = texture(u_emissive_texture, v_uv).rgb;
        emissive *= tex;
    }

    vec3 color = emissive;
    vec3 normal = normalize(u_normal * v_normal);
    for(int i = 0; i < u_lights_count; i++) {
        color += calculateLight(u_lights[i], ambient, diffuse, specular, normal);
    }

    f_frag_color = vec4(color, 1.0);
}
)glsl";
