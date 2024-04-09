#include "renderer2d.h"

#include <iostream>
#include <algorithm>
#include <optional>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "io/file.h"
#include "shader.h"
#include "render_data.h"

static RenderData s_color_render_data;
static RenderData s_texture_render_data;

static unsigned int s_color_shader;
static unsigned int s_texture_shader;
static unsigned int s_text_shader;

static glm::vec2 s_render_size;

#define PROJECTION glm::ortho(0.0f, s_render_size.x, s_render_size.y, 0.0f, -1.0f, 1.0f)

namespace renderer
{
    void initialize(glm::vec2 render_size)
    {
        s_render_size = render_size;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float color_quad_vertices[] = {
            -0.5f,  0.5f,
             0.5f,  0.5f,
             0.5f, -0.5f,
            -0.5f, -0.5f
        };

        float texture_quad_vertices[] = {
            -0.5f,  0.5f, 0.0f, 1.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        s_color_render_data = render_data_new(color_quad_vertices, sizeof(color_quad_vertices), indices, sizeof(indices), []()
        {
            glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);
        });

        s_texture_render_data = render_data_new(texture_quad_vertices, sizeof(texture_quad_vertices), indices, sizeof(indices), []()
        {
            glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void *)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);
        });

        s_color_shader = shader_new(
            read_file("resources/shaders/color.vert"),
            read_file("resources/shaders/color.frag")
        );

        s_texture_shader = shader_new(
            read_file("resources/shaders/texture.vert"),
            read_file("resources/shaders/texture.frag")
        );

        s_text_shader = shader_new(
            read_file("resources/shaders/texture.vert"),
            read_file("resources/shaders/text.frag")
        );
    }

    void immediate_quad(glm::vec2 position, glm::vec2 size, glm::vec4 color)
    {
        immediate_quad(s_color_render_data.vao, s_color_shader, position, size, glm::vec2(1.0f), color, std::nullopt, 0.0f);
    }

    void immediate_quad(glm::vec2 position, glm::vec2 size, glm::vec4 color, Texture texture, float rotation)
    {
        immediate_quad(s_texture_render_data.vao, s_texture_shader, position, size, glm::vec2(1.0f), color, texture, rotation);
    }

    void immediate_quad(glm::vec2 position, Texture texture, float rotation)
    {
        immediate_quad(position, texture, glm::vec2(1.0f), rotation);
    }

    void immediate_quad(glm::vec2 position, Texture texture, glm::vec2 scale, float rotation)
    {
        immediate_quad(s_texture_render_data.vao, s_texture_shader, position, (glm::vec2)texture.size, scale, glm::vec4(1.0f), texture, rotation);
    }

    void text(glm::vec2 position, const Font &font, glm::vec4 color, std::string_view text)
    {
        glm::vec2 initial_position = position;

        float maximum_bearing_y = std::max_element(
            font.characters.begin(),
            font.characters.end(),
            [](const auto &a, const auto &b)
            {
                return a.second.bearing.y < b.second.bearing.y;
            }
        )->second.bearing.y;

        float maximum_size_y = 0.0f;

        for (const auto &[_, character] : font.characters) {
            if (maximum_size_y < character.texture.size.y) {
                maximum_size_y = character.texture.size.y;
            }
        }

        for (char character : text) {
            if (character == '\n') {
                position.x = initial_position.x;
                position.y += maximum_size_y;
                continue;
            }

            const Character &current_character = font.characters.at(character);

            glm::ivec2 place_position(
                position.x +  current_character.bearing.x,
                position.y + (current_character.texture.size.y - current_character.bearing.y) + (maximum_bearing_y - current_character.texture.size.y)
            );

            immediate_quad(
                s_texture_render_data.vao,
                s_text_shader,
                place_position,
                current_character.texture.size,
                glm::vec2(1.0f),
                color,
                current_character.texture,
                0.0f
            );

            position.x += current_character.advance >> 6;
        }
    }

    void border(glm::vec2 position, glm::vec2 size, glm::vec4 color, float thickness)
    {
        // Top
        immediate_quad(position, glm::vec2(size.x, thickness), color);
    
        // Right
        immediate_quad(position + glm::vec2(size.x - thickness, 0), glm::vec2(thickness, size.y), color);

        // Left
        immediate_quad(position, glm::vec2(thickness, size.y), color);
    
        // Bottom
        immediate_quad(position + glm::vec2(0, size.y - thickness), glm::vec2(size.x, thickness), color);
    }

    void immediate_quad(unsigned int vao, unsigned int shader, glm::vec2 position, glm::vec2 size, glm::vec2 scale, glm::vec4 color, std::optional<Texture> texture, float rotation)
    {
        glBindVertexArray(vao);
        glUseProgram(shader);
        
        shader_set_vec4(shader, "color", color);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.has_value() ? texture.value().id : 0);

        shader_set_int(shader, "tex", 0);

        shader_set_mat4(
            shader,
            "model",
            glm::translate(glm::mat4(1.0f), glm::vec3(position + size / 2.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(size * scale, 0.0f))
        );

        shader_set_mat4(shader, "projection", PROJECTION);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(0);
        glBindVertexArray(0);
    }

    glm::vec2 render_size()
    {
        return s_render_size;
    }

    void render_size(glm::vec2 render_size)
    {
        s_render_size = render_size;
    }
}
