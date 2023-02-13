#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

#ifndef OBJECT_H
#define OBJECT_H

class Object {
public:

	string name;
	string originalName;

	std::vector<Object*> children;

	bool selected = true;
	bool rotation_animation = false;
	bool translation_animation = false;

	float rotationX = 0;
	float rotationY = 0;
	float rotationZ = 0;

	float translationX = 0;
	float translationY = 0;
	float translationZ = 0;

	float scaleX = 1;
	float scaleY = 1;
	float scaleZ = 1;

	virtual void drawWireframe() = 0;

	virtual void drawSolid() = 0;

	virtual void drawShader() = 0;

	virtual void drawBoundingBox() = 0;

	virtual ofVec3f getMeshBoundingBoxDimension(ofMesh* mesh) = 0;

};

#endif // !1

