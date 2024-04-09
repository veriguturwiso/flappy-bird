#pragma once

#include <glm/glm.hpp>

struct Rectangle
{
	glm::vec2 position;
	glm::vec2 size;
};

bool rectangles_intersect(Rectangle rectangle1, Rectangle rectangle2);
