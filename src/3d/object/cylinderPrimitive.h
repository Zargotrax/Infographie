#include "ofMain.h"
#include "object.h"

class CylinderPrimitive : public Object {
public:

	ofCylinderPrimitive* cylinder;

	void drawWireframeOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};