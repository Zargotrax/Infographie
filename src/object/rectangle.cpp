#include "rectangle.h"

void RectangleShape::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = TwoDimensionalObject::coordinates.x + offsetX;
    position.y = TwoDimensionalObject::coordinates.y + offsetY;

    ofDrawRectangle(position, width, height);
}

void RectangleShape::rotate90() {
    int originalHeight = height;
    height = width;
    width = originalHeight;
}