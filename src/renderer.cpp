#include "renderer.h"

void Renderer::setup()
{
	ofBackground(100, 100, 100);
}

void Renderer::draw()
{
	for (TwoDimensionalObject* element : Renderer::elements) {
		ofSetColor(color.r, color.g, color.b, element->opacity);
		element->draw(offsetX1, offsetY1);
		ofSetColor(255, 255, 255, 255);
	}
}

bool Renderer::hitTest(int x, int y)
{
	return x > offsetX1 && x < offsetX2&& y > offsetY1 && y < offsetY2;
}
