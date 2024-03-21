#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

uniform mat3 u_normal;

out vec4 f_frag_color;

void main() {
    // f_frag_color = vec4(inverse(u_normal) * v_normal, 1);
    f_frag_color = vec4(normalize(v_normal), 1);
}
