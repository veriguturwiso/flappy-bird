#pragma once

#include <array>
#include <functional>

struct RenderData
{
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};

RenderData render_data_new(float *vertices, int vertices_size_in_bytes, unsigned int *indices, int indices_size_in_bytes, std::function<void(void)> vao_setup);
