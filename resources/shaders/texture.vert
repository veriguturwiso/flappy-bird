#version 330 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_tex_coords;

out vec2 f_tex_coords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	gl_Position = projection * model * vec4(a_pos, 0, 1);

    f_tex_coords = a_tex_coords;
}
