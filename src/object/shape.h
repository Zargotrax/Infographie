#include "twoDimensionalObject.h"

#ifndef SHAPE_H
#define SHAPE_H

class Shape : public TwoDimensionalObject
{
public:

    int height = 100;
    int width = 150;
    int nbApex = 5;
    ofColor fillColor = ofColor::white;
    ofColor outlineColor = ofColor::black;
    float outlineThickness = 2;

};

#endif // !1
