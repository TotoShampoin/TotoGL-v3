#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

uniform mat3 u_normal;

out vec4 f_frag_color;

void main() {
    // depth
    float depth = (2 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) / (gl_DepthRange.far - gl_DepthRange.near) / gl_FragCoord.w;
    vec3 normal = normalize(v_normal);
    f_frag_color = vec4(normal * depth, 1);

}
