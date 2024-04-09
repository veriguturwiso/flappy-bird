#pragma once

#include <string>
#include <glm/glm.hpp>

struct TextureSpecification
{
    int pixel_internal_format;
    int pixel_format;
    int pixel_type;
};

struct Texture
{
    unsigned int id;
    glm::ivec2 size;
};

Texture texture_new(std::string_view path);
Texture texture_new(glm::ivec2 size, unsigned char *data, TextureSpecification texture_specification);
