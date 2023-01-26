#include "circle.h"

void Circle::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = TwoDimensionalObject::coordinates.x + offsetX;
    position.y = TwoDimensionalObject::coordinates.y + offsetY;

    ofDrawCircle(position, width/2);
}

void Circle::rotate90() {
    //you cant rotate a circle 90 degres lol
}