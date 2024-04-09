#include "shader.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

unsigned int shader_new(const std::string &vertex_shader_source, const std::string &fragment_shader_source)
{
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vertex_shader_source_ptr = vertex_shader_source.c_str();
	const char *fragment_shader_source_ptr = fragment_shader_source.c_str();

	glShaderSource(vertex_shader, 1, &vertex_shader_source_ptr, NULL);
	glShaderSource(fragment_shader, 1, &fragment_shader_source_ptr, NULL);

	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

	char info_log[512];
	int success;

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

	if (!success)
    {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cerr << "ERROR: " << info_log << '\n';
	}

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

	if (!success)
    {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cerr << "ERROR: " << info_log << '\n';
	}

	unsigned int shader = glCreateProgram();

	glAttachShader(shader, vertex_shader);
	glAttachShader(shader, fragment_shader);

	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	if (!success)
    {
		glGetProgramInfoLog(shader, 512, NULL, info_log);
		std::cerr << "ERROR: " << info_log << '\n';
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

    return shader;
}

void shader_set_int(unsigned int shader, std::string_view name, int value)
{
	glUniform1i(glGetUniformLocation(shader, name.data()), value);
}

void shader_set_vec4(unsigned int shader, std::string_view name, glm::vec4 value)
{
    glUniform4fv(glGetUniformLocation(shader, name.data()), 1, glm::value_ptr(value));
}

void shader_set_mat4(unsigned int shader, std::string_view name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(shader, name.data()), 1, false, glm::value_ptr(value));
}
