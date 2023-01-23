#include "ofMain.h"
#include "image.h"

class Renderer
{
public:

	std::map<string, Image*> images;
	Image* activeImage;
	ofColor color;
	int offsetX1 = 0;
	int offsetY1 = 0;
	int offsetX2 = 0;
	int offsetY2 = 0;

	void setup();

	void draw();

};
