#include "twoDimensionalObject.h"

class Image : public TwoDimensionalObject 
{
public:

    ofImage imageData;
    string path;

    void draw(int offsetX, int offsetY);

    void rotate90();
};