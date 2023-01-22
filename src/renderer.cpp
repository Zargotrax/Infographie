#include "renderer.h"

void Renderer::setup()
{
	ofBackground(100, 100, 100);
}

void Renderer::draw()
{
	if (image.isAllocated()) {
		ofSetColor(color);
        image.draw(offsetX, offsetY);
		ofSetColor(255, 255, 255, 255);
	}
}
