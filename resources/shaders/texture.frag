#version 330 core

out vec4 output_color;

in vec2 f_tex_coords;

uniform vec4 color;
uniform sampler2D tex;

void main()
{
	output_color = color * texture(tex, f_tex_coords);
}
