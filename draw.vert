#version 330 core

layout (location=0) in vec2 in_pos;

uniform mat4 M;
uniform mat4 P;

void main()
{
    gl_Position = P * M * vec4(in_pos, 0.0f ,1.0f);

    gl_PointSize = 1.0f;
}
