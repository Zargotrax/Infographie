#include "ellipsis.h"

void Ellipsis::draw(int offsetX, int offsetY) {
    glm::vec3 position;
    position.x = TwoDimensionalObject::coordinates.x + (width/2) + offsetX;
    position.y = TwoDimensionalObject::coordinates.y + (height/2) + offsetY;

    ofFill();
    ofSetLineWidth(0);
    ofSetColor(fillColor);
    ofDrawEllipse(position, width, height);

    ofNoFill();
    ofSetLineWidth(outlineThickness);
    ofSetColor(outlineColor);
    ofDrawEllipse(position, width, height);
}

void Ellipsis::rotate90() {
    int originalHeight = height;
    height = width;
    width = originalHeight;
}