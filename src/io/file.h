#pragma once

#include <string_view>

std::string read_file(std::string_view path);
void write_to_file(std::string_view path, std::string_view content);
