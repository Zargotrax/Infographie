#include "renderer.h"

void Renderer::setup()
{
	ofBackground(100, 100, 100);
}

void Renderer::draw()
{
	for (Image*image : Renderer::images) {
		if (image->imageData.isAllocated()) {
			ofSetColor(color.r, color.g, color.b, image->opacity);
			image->imageData.draw(image->coordinates.x + offsetX1, image->coordinates.y + offsetY1);
			ofSetColor(255, 255, 255, 255);
		}
	}

}
