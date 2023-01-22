#include "ofMain.h"

class Renderer
{
public:

	ofImage image;
	ofColor color;
	int offsetX = 0;
	int offsetY = 0;

	void setup();

	void draw();

};
