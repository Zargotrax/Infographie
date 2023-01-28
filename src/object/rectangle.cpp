#include "rectangle.h"

void RectangleShape::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = TwoDimensionalObject::coordinates.x + offsetX;
    position.y = TwoDimensionalObject::coordinates.y + offsetY;

    ofFill();
    ofSetLineWidth(0);
    ofSetColor(fillColor);
    ofDrawRectangle(position, width, height);

    ofNoFill();
    ofSetLineWidth(outlineThickness);
    ofSetColor(outlineColor);
    ofDrawRectangle(position, width, height);
}

void RectangleShape::rotate90() {
    int originalHeight = height;
    height = width;
    width = originalHeight;
}