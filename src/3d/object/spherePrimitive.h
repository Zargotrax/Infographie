#include "ofMain.h"
#include "object.h"

class SpherePrimitive : public Object {
public:

	ofSpherePrimitive* sphere;

	void drawWireframeOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};