#include "ofMain.h"
#include "object.h"

class LoadedFile : public Object {
public:

	ofxAssimpModelLoader* model;

	void drawWireframe();

	void drawSolid();

	void drawShader();

	void drawBoundingBox();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);
};