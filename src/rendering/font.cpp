#include "font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <freetype/freetype.h>
#include <iostream>
#include <glad/glad.h>

Font font_new(std::string_view path, int size)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cerr << "ERROR: could not init FreeType library\n";
    }

    FT_Face face;
    if (FT_New_Face(ft, path.data(), 0, &face))
    {
        std::cerr << "ERROR: failed to load font\n";
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    std::map<char, Character> font_characters;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char character = 0; character < 128; character++)
    {
        if (FT_Load_Char(face, character, FT_LOAD_RENDER))
        {
            std::cerr << "ERROR: failed to load glyph\n";
            continue;
        }

        font_characters.insert(
            std::make_pair(
                character,
                Character
                {
                    .texture = texture_new(
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        face->glyph->bitmap.buffer,
                        TextureSpecification
                        {
                            .pixel_internal_format = GL_COMPRESSED_RED,
                            .pixel_format = GL_RED,
                            .pixel_type = GL_UNSIGNED_BYTE
                        }
                    ),
                    .bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    .advance = (unsigned int)face->glyph->advance.x
                }
            )
        );
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return Font
    {
        .characters = std::move(font_characters)
    };
}

glm::vec2 measure_text(const Font &font, std::string_view text)
{
    float maximum_size_y = 0.0f;

    for (const auto &[_, character] : font.characters) {
        if (maximum_size_y < character.texture.size.y) {
            maximum_size_y = character.texture.size.y;
        }
    }

    float current_width = 0.0f;
    glm::vec2 size = glm::vec2(0.0f, maximum_size_y);

    for (char character : text) {
        if (character == '\n') {
            size.y += maximum_size_y;
            current_width = 0.0f;
            continue;
        }

        Character current_character = font.characters.at(character);

        current_width += current_character.advance >> 6;

        if (current_width > size.x) {
            size.x = current_width;
        }
    }

    return size;
}
