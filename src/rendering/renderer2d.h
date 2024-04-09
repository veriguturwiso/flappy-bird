#pragma once

#include <string>
#include <optional>
#include <glm/glm.hpp>

#include "texture.h"
#include "font.h"

namespace renderer
{
    void initialize(glm::vec2 render_size);

    void immediate_quad(
        glm::vec2 position,
        glm::vec2 size,
        glm::vec4 color
    );

    void immediate_quad(
        glm::vec2 position,
        glm::vec2 size,
        glm::vec4 color,
        Texture texture,
        float rotation = 0.0f
    );

    void immediate_quad(
        glm::vec2 position,
        Texture texture,
        float rotation = 0.0f
    );

    void immediate_quad(
        glm::vec2 position,
        Texture texture,
        glm::vec2 scale,
        float rotation = 0.0f
    );

    void text(
        glm::vec2 position,
        const Font &font,
        glm::vec4 color,
        std::string_view text
    );

    void border(
        glm::vec2 position,
        glm::vec2 size,
        glm::vec4 color,
        float thickness
    );

    void immediate_quad(
        unsigned int vao,
        unsigned int shader,
        glm::vec2 position,
        glm::vec2 size,
        glm::vec2 scale,
        glm::vec4 color,
        std::optional<Texture> texture,
        float rotation
    );

    glm::vec2 render_size();
    void render_size(glm::vec2 render_size);
}
