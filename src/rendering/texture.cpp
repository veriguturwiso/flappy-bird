#include "texture.h"

#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture texture_new(std::string_view path)
{
    glm::ivec2 texture_size;
    int nr_channels;

    unsigned char *data = stbi_load(path.data(), &texture_size.x, &texture_size.y, &nr_channels, 0);

    if (!data)
    {
        std::cerr << "ERROR: failed to load texture `" << path << "`\n";
    }

    unsigned int texture_id;
    
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        nr_channels == 4 ? GL_RGBA : GL_RGB,
        texture_size.x,
        texture_size.y,
        0,
        nr_channels == 4 ? GL_RGBA : GL_RGB,
        GL_UNSIGNED_BYTE,
        data
    );

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return Texture
    {
        .id = texture_id,
        .size = texture_size
    };
}

Texture texture_new(glm::ivec2 size, unsigned char *data, TextureSpecification texture_specification)
{
    unsigned int texture_id;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        texture_specification.pixel_internal_format,
        size.x,
        size.y,
        0,
        texture_specification.pixel_format,
        texture_specification.pixel_type,
        data
    );

    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture
    {
        .id = texture_id,
        .size = size
    };
}
