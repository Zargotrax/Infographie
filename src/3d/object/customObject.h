#include "ofMain.h"
#include "object.h"
#include "triangulator.h"

class CustomObject : public Object {
public:

	Triangulator* triangulator = new Triangulator();

	void drawWireframeOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};