#include "ofMain.h"
#include "object.h"

class LoadedFile : public Object {
public:

	ofxAssimpModelLoader* model;

	void drawWireframeOverride();


	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};