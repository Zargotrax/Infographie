#include "ofMain.h"
#include "object.h"

class CylinderPrimitive : public Object {
public:

	ofCylinderPrimitive* cylinder;

	void drawWireframe();

	void drawSolid();

	void drawShader();

	void drawBoundingBox();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};