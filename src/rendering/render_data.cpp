#include "render_data.h"

#include <glad/glad.h>

RenderData render_data_new(float *vertices, int vertices_size_in_bytes, unsigned int *indices, int indices_size_in_bytes, std::function<void(void)> vao_setup)
{
    unsigned int render_data_vao, render_data_vbo, render_data_ebo;

    glGenVertexArrays(1, &render_data_vao);
    glGenBuffers(1, &render_data_vbo);
    glGenBuffers(1, &render_data_ebo);

    glBindVertexArray(render_data_vao);
    glBindBuffer(GL_ARRAY_BUFFER, render_data_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_data_ebo);

    vao_setup();

    glBufferData(GL_ARRAY_BUFFER, vertices_size_in_bytes, vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size_in_bytes, indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return RenderData
    {
        .vao = render_data_vao,
        .vbo = render_data_vbo,
        .ebo = render_data_ebo
    };
}
