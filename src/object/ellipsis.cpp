#include "ellipsis.h"

void Ellipsis::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = TwoDimensionalObject::coordinates.x + offsetX;
    position.y = TwoDimensionalObject::coordinates.y + offsetY;

    ofDrawEllipse(position, width, height);
}

void Ellipsis::rotate90() {
    int originalHeight = height;
    height = width;
    width = originalHeight;
}