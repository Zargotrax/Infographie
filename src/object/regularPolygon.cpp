#include "regularPolygon.h"

void RegularPolygon::draw(int offsetX, int offsetY) {
    ofPoint center = {  TwoDimensionalObject::coordinates.x + (width / 2) + offsetX,
                        TwoDimensionalObject::coordinates.y + (width / 2) + offsetY, 0 };

    ofFill();
    ofSetLineWidth(0);
    ofSetColor(fillColor);
    drawPolygon(center);

    ofNoFill();
    ofSetLineWidth(outlineThickness);
    ofSetColor(outlineColor);
    drawPolygon(center);
}

void RegularPolygon::rotate90() {
    angleModifier += PI/2;
}

void RegularPolygon::drawPolygon(ofPoint center) {
    float angle = TWO_PI / nbApex; // 72 degrees
    int radius = width / 2;
    for (int i = 0; i < nbApex; i++) {
        ofPoint outer1 = center + ofPoint(radius * cos((i * angle) + angleModifier), radius * sin((i * angle) + angleModifier), 0);
        ofPoint outer2 = center + ofPoint(radius * cos(((i + 1) * angle) + angleModifier), radius * sin(((i + 1) * angle) + angleModifier), 0);
        ofDrawTriangle(outer1, outer2, center);
    }
}