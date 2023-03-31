#include "ofMain.h"
#include "object.h"
#include "bezierSurface.h"

class Surface : public Object {
public:

	BezierSurface* bezierSurface = new BezierSurface();

	void drawWireframeOverride();

	void drawSolidOverride();

	void drawShaderOverride();

	void drawBoundingBoxOverride();

	ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh);

};