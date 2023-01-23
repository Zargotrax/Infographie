#include "renderer.h"

void Renderer::setup()
{
	ofBackground(100, 100, 100);
}

void Renderer::draw()
{
	for (std::pair<const string, Image*>& pair : Renderer::images) {
		Image* image = pair.second;
		if (image->imageData.isAllocated()) {
			ofSetColor(color.r, color.g, color.b, image->opacity);
			image->imageData.draw(image->coordinates.x + offsetX1, image->coordinates.y + offsetY2);
			ofSetColor(255, 255, 255, 255);
		}
	}

}
