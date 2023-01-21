#include "renderer.h"

void Renderer::setup()
{
	ofBackground(100, 100, 100);
}

void Renderer::draw()
{
	if (image.isAllocated()) {
        image.draw(0, 0);// , ofGetWindowWidth(), ofGetWindowHeight());
	}
}