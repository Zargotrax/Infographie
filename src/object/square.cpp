#include "square.h"

void Square::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = TwoDimensionalObject::coordinates.x + offsetX;
    position.y = TwoDimensionalObject::coordinates.y + offsetY;

    ofDrawRectangle(position, width, width);
}

void Square::rotate90() {
    //you cant rotate a square 90 degres lol
}
