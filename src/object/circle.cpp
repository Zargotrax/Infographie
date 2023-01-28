#include "circle.h"

void Circle::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = TwoDimensionalObject::coordinates.x + (width / 2) + offsetX;
    position.y = TwoDimensionalObject::coordinates.y + (width / 2) + offsetY;

    ofFill();
    ofSetLineWidth(0);
    ofSetColor(fillColor);
    ofDrawCircle(position, width / 2);

    ofNoFill();
    ofSetLineWidth(outlineThickness);
    ofSetColor(outlineColor);
    ofDrawCircle(position, width/2);
}

void Circle::rotate90() {
    //you cant rotate a circle 90 degres lol
}