#include "ofMain.h"
#include "object.h"

class Curve : public Object {
public:

	ofPolyline* polyline;

	ofPoint controlPoints[4] = {ofPoint(-200, 0, 0), ofPoint(-200, 100, 0), ofPoint(200, 100, 0), ofPoint(200, 0, 0)};

	void drawWireframeOverride();

	void drawSolidOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);

};