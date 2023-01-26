#include "shape.h"

class Amongus : public Shape
{
public:

    int radius = 100;

    //ofPoint center = {500, 500, 0};
    float innerRadius = 50;
    float outerRadius = 100;

    virtual void draw(int offsetX, int offsetY) {
        ofPoint center = { TwoDimensionalObject::coordinates.x + offsetX, TwoDimensionalObject::coordinates.y + offsetY, 0 };
        ofPushMatrix();
        ofTranslate(center);
        ofScale(100, 100);
        ofSetColor(ofColor::red);

        //head
        ofDrawTriangle(ofPoint(-0.25, -0.5), ofPoint(0.25, -0.5), ofPoint(0, -0.25));
        //body
        ofDrawTriangle(ofPoint(-0.25, -0.25), ofPoint(0.25, -0.25), ofPoint(0, 0.5));
        //legs
        ofDrawTriangle(ofPoint(-0.125, 0.25), ofPoint(0.125, 0.25), ofPoint(0, 0.5));

        ofPopMatrix();
    }

    virtual void rotate90() {
        //you cant rotate a square 90 degres lol
    }
};