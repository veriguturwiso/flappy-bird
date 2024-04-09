#version 330 core

out vec4 output_color;

in vec2 f_tex_coords;

uniform vec4 color;
uniform sampler2D tex;

void main()
{
    float red = texture(tex, f_tex_coords).r;

    if (red == 0)
        discard;

	output_color = vec4(1, 1, 1, red) * color;
}
