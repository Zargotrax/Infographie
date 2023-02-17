#include "loadedFile.h"

void LoadedFile::drawWireframe()
{
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	model->drawWireframe();

	for (Object* child : children) {
		child->drawWireframe();
	}

	ofPopMatrix();
}

void LoadedFile::drawSolid() {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	model->drawFaces();

	for (Object* child : children) {
		child->drawSolid();
	}

	ofPopMatrix();
}

void LoadedFile::drawShader() {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	model->disableMaterials();

	model->drawFaces();

	for (Object* child : children) {
		child->drawShader();
	}

	model->enableMaterials();

	ofPopMatrix();
}

void LoadedFile::drawBoundingBox()
{
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	ofMesh mesh = model->getMesh(0);
	ofVec3f scale = getMeshBoundingBoxDimension(&mesh);
	float scaleFactor = model->getNormalizedScale();
	scale = (scale * scaleFactor) / 2;
	ofVec3f pos = model->getPosition() + (model->getSceneCenter() * -scaleFactor);

	ofVec3f v1 = { pos.x + scale.x, pos.y - scale.y, pos.z + scale.z };
	ofVec3f v2 = { pos.x + scale.x, pos.y - scale.y, pos.z - scale.z };
	ofVec3f v3 = pos - scale;
	ofVec3f v4 = { pos.x - scale.x, pos.y - scale.y, pos.z + scale.z };
	ofVec3f v5 = pos + scale;
	ofVec3f v6 = { pos.x + scale.x, pos.y + scale.y, pos.z - scale.z };
	ofVec3f v7 = { pos.x - scale.x, pos.y + scale.y, pos.z - scale.z };
	ofVec3f v8 = { pos.x - scale.x, pos.y + scale.y, pos.z + scale.z };

	ofDrawLine(v1, v4);
	ofDrawLine(v1, v2);
	ofDrawLine(v2, v3);
	ofDrawLine(v3, v4);

	ofDrawLine(v1, v5);
	ofDrawLine(v2, v6);
	ofDrawLine(v3, v7);
	ofDrawLine(v4, v8);

	ofDrawLine(v5, v6);
	ofDrawLine(v6, v7);
	ofDrawLine(v7, v8);
	ofDrawLine(v5, v8);

	for (Object* child : children) {
		child->drawBoundingBox();
	}

	ofPopMatrix();
}

ofVec3f LoadedFile::getMeshBoundingBoxDimension(ofMesh* mesh) {
	ofPoint minVertex, maxVertex;
	minVertex.x = minVertex.y = minVertex.z = std::numeric_limits<float>::max();
	maxVertex.x = maxVertex.y = maxVertex.z = -std::numeric_limits<float>::max();

	std::for_each(mesh->getVertices().begin(), mesh->getVertices().end(),
		[&minVertex, &maxVertex](const ofPoint& vertex) {
			minVertex.x = std::min(minVertex.x, vertex.x);
	minVertex.y = std::min(minVertex.y, vertex.y);
	minVertex.z = std::min(minVertex.z, vertex.z);
	maxVertex.x = std::max(maxVertex.x, vertex.x);
	maxVertex.y = std::max(maxVertex.y, vertex.y);
	maxVertex.z = std::max(maxVertex.z, vertex.z);
		});

	return maxVertex - minVertex;
}
