#include "renderer.h"

void Renderer::setup()
{
	ofBackground(100, 100, 100);
}

void Renderer::draw()
{
	if (image.isAllocated()) {
		ofSetColor(redIntensity, greenIntensity, blueIntensity, 255);
        image.draw(offsetX, offsetY);
		ofSetColor(255, 255, 255, 255);
	}
}