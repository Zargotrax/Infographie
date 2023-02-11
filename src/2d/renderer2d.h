#include "ofMain.h"
#include "./object/drawable.h"

class Renderer2d
{
public:

	vector<TwoDimensionalObject*> elements;
	TwoDimensionalObject* active;
	int activeIndex = -1;
	ofColor color;
	int offsetX1 = 0;
	int offsetY1 = 0;
	int offsetX2 = 0;
	int offsetY2 = 0;

	void setup();

	void draw();

	bool hitTest(int x, int y);

};
