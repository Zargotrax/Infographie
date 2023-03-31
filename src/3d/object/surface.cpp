#include "surface.h"

void Surface::drawWireframeOverride() {
	ofPushStyle();

	ofTranslate(-250/2, -250/2, 0);

	bezierSurface->drawWireframe();
	bezierSurface->drawControls();

	ofPopStyle();
}

void Surface::drawSolidOverride() {
	ofTranslate(-250 / 2, -250 / 2, 0);

	bezierSurface->draw();
}

void Surface::drawShaderOverride() {
	ofTranslate(-250 / 2, -250 / 2, 0);

	bezierSurface->draw();
}

void Surface::drawBoundingBoxOverride() {
}

ofVec3f Surface::getMeshBoundingBoxDimension(ofMesh* mesh) {
	return ofVec3f();
}
