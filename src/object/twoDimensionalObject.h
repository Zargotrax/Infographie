#include "ofMain.h"
#ifndef TWODIMENSIONALOBJECT_H
#define TWODIMENSIONALOBJECT_H

class TwoDimensionalObject
{
public:

    string name;
    string originalName;
    ofPoint coordinates;
    int opacity = 255;

    virtual void draw(int offsetX, int offsetY) = 0;
    virtual void rotate90() = 0;

};

#endif // !1
