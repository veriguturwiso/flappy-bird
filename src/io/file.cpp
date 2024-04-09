#include "file.h"

#include <string>
#include <iostream>
#include <fstream>

std::string read_file(std::string_view path)
{
	std::ifstream file(path.data());

	if (!file.is_open()) {
		std::cerr << "ERROR: could not open file `" << path << "`\n";
		return "";
	}

	std::string buffer;
	buffer.reserve(1024);

	std::string current_line;

	std::getline(file, current_line);
	buffer.append(current_line);

	while (std::getline(file, current_line)) {
		buffer.append("\n");
		buffer.append(current_line);
	}

	file.close();

	return buffer;
}

void write_to_file(std::string_view path, std::string_view content)
{
	std::ofstream file(path.data());

	if (!file.is_open()) {
		std::cerr << "ERROR: could not open file `" << path << "`\n";
		return;
	}

	file << content;
	file.close();

	return;
}
