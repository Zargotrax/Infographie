#include "ofMain.h"
#include "object.h"

class SpherePrimitive : public Object {
public:

	ofSpherePrimitive* sphere;

	void drawWireframe();

	void drawSolid();

	void drawShader();

	void drawBoundingBox();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};