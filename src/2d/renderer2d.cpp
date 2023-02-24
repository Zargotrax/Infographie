#include "renderer2d.h"

void Renderer2d::setup()
{
	ofBackground(100, 100, 100);
}

void Renderer2d::draw()
{
	for (TwoDimensionalObject* element : Renderer2d::elements) {
		ofSetColor(element->color.r, element->color.g, element->color.b, element->opacity);
		element->draw(offsetX1, offsetY1);
		ofSetColor(255, 255, 255, 255);
	}
}

bool Renderer2d::hitTest(int x, int y)
{
	return x > offsetX1 && x < offsetX2&& y > offsetY1 && y < offsetY2;
}
