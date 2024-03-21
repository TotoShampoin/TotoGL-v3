#version 460

layout(location = 0) in vec3 a_position;

out vec3 v_position;

void main() {
    vec4 position = vec4(a_position, 1.);
    v_position = position.xyz;

    gl_Position = position;
}
