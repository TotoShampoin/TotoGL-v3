#version 460

struct AmbientLight {
    vec3 color;
    float strength;
};
struct DirectionalLight {
    vec3 color;
    float strength;
    vec3 direction;
};
struct PointLight {
    vec3 color;
    float strength;
    vec3 position;
};

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

uniform float u_time;
uniform sampler2D u_texture;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform mat3 u_normal;

uniform AmbientLight u_amb_light;
uniform DirectionalLight u_dir_light;

vec3 blinnPhong(vec3 w_i, vec3 l_i, vec3 n, vec3 k_d, vec3 k_s, float shininess) {
    vec3 w_o = vec3(0, 0, 1);
    vec3 half_vector = (w_o + w_i) / 2;
    
    vec3 result_diffuse = k_d * max(0, dot(w_i, n));
    vec3 result_specular = k_s * pow(max(0, dot(half_vector, n)), shininess);

    return l_i * ( result_diffuse + result_specular );
}

vec3 calculateAmbientLight(AmbientLight light, vec3 color) {
    return color  * light.color * light.strength;
}
vec3 calculateDirectionalLight(DirectionalLight light, vec3 color, vec3 normal) {
    return blinnPhong(-light.direction, light.color * light.strength, normal, color, vec3(1), 32.);
}
vec3 calculatePointLight(PointLight light, vec3 color, vec3 normal) {
    vec3 direction = normalize(light.position - v_position);
    float strength = 1.0 / pow(length(light.position - v_position), 2);
    return blinnPhong(direction, light.color * light.strength * strength, normal, color, vec3(1), 32.);
}

void main() {
    vec4 diffuse = texture(u_texture, v_uv);
    vec3 color = vec3(0);
    color += calculateAmbientLight(u_amb_light, diffuse.rgb);
    color += calculateDirectionalLight(u_dir_light, diffuse.rgb, v_normal);
    f_frag_color = vec4(color, diffuse.a);
}
