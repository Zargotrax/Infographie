#include "ofMain.h"

class Renderer
{
public:

	ofImage image;
	int offsetX = 0;
	int offsetY = 0;
	int redIntensity = 255;
	int greenIntensity = 255;
	int blueIntensity = 255;

	void setup();

	void draw();

};
