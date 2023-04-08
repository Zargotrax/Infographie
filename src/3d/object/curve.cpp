#include "curve.h"

void Curve::drawWireframeOverride() {
	ofPushStyle();

	ofSetColor(ofColor::black);
	ofNoFill();

	ofBeginShape();
	ofVertex(controlPoints[0].x, controlPoints[0].y, controlPoints[0].z);
	ofBezierVertex(controlPoints[1].x, controlPoints[1].y, controlPoints[1].z,
		controlPoints[2].x, controlPoints[2].y, controlPoints[2].z,
		controlPoints[3].x, controlPoints[3].y, controlPoints[3].z);
	ofEndShape();


	ofDrawSphere(controlPoints[0], 5);
	ofDrawSphere(controlPoints[1], 5);
	ofDrawSphere(controlPoints[2], 5);
	ofDrawSphere(controlPoints[3], 5);

	ofPopStyle();
}

void Curve::drawShaderOverride() {
}

void Curve::drawBoundingBoxOverride() {

}

ofVec3f Curve::getMeshBoundingBoxDimension(ofMesh* mesh) {
	return ofVec3f();
}
