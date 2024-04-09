#include "rectangle.h"

bool rectangles_intersect(Rectangle rectangle1, Rectangle rectangle2)
{
    return rectangle1.position.x <= rectangle2.position.x + rectangle2.size.x && rectangle2.position.x <= rectangle1.position.x + rectangle1.size.x &&
        rectangle1.position.y <= rectangle2.position.y + rectangle2.size.y && rectangle2.position.y <= rectangle1.position.y + rectangle1.size.y;
}
