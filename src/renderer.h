#include "ofMain.h"
#include "image.h"

class Renderer
{
public:

	std::vector<Image*> images;
	Image* activeImage;
	int activeImageIndex = -1;
	ofColor color;
	int offsetX1 = 0;
	int offsetY1 = 0;
	int offsetX2 = 0;
	int offsetY2 = 0;

	void setup();

	void draw();

};
