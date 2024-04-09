#pragma once

#include <string>
#include <glm/gtc/matrix_transform.hpp>

unsigned int shader_new(const std::string &vertex_shader_source, const std::string &fragment_shader_source);

void shader_set_int(unsigned int shader, std::string_view name, int value);
void shader_set_vec4(unsigned int shader, std::string_view name, glm::vec4 value);
void shader_set_mat4(unsigned int shader, std::string_view name, glm::mat4 value);
