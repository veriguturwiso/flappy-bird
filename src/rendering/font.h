#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>

#include "texture.h"

struct Character
{
    Texture texture;
    glm::ivec2 bearing;
    unsigned int advance;
};

struct Font
{
    std::map<char, Character> characters;
};

Font font_new(std::string_view path, int size);

glm::vec2 measure_text(const Font &font, std::string_view text);
