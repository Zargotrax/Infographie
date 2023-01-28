#include "square.h"

void Square::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = TwoDimensionalObject::coordinates.x + offsetX;
    position.y = TwoDimensionalObject::coordinates.y + offsetY;

    ofFill();
    ofSetLineWidth(0);
    ofSetColor(fillColor);
    ofDrawRectangle(position, width, width);

    ofNoFill();
    ofSetLineWidth(outlineThickness);
    ofSetColor(outlineColor);
    ofDrawRectangle(position, width, width);
}

void Square::rotate90() {
    //you cant rotate a square 90 degres lol
}
