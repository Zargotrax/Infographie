#include "object.h"

void Object::drawWireframe()
{
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	drawWireframeOverride();

	for (Object* child : children) {
		child->drawWireframe();
	}

	ofPopMatrix();
}

void Object::drawSolid() {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	drawSolidOverride();

	for (Object* child : children) {
		child->drawSolid();
	}

	ofPopMatrix();
}

void Object::drawTexture(ofShader* toneMapping) {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	if (filteredTexture.isAllocated()) {
		filteredTexture.setTextureMinMagFilter(magFilter, magFilter);
		filteredTexture.bind();

		toneMapping->begin();
		toneMapping->setUniformTexture("image", filteredTexture, 1);
		toneMapping->setUniform1f("tone_mapping_exposure", exposure);
		toneMapping->setUniform1f("tone_mapping_gamma", gamma);
		toneMapping->setUniform1i("tone_mapping_toggle", false);
	}

	drawShaderOverride();


	if (filteredTexture.isAllocated()) {
		filteredTexture.unbind();
		toneMapping->end();
	}

	for (Object* child : children) {
		child->drawShader(toneMapping);
	}

	ofPopMatrix();
}

void Object::drawShader(ofShader* shader) {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	shader->begin();
	shader->setUniform3f("pointLight.position", -200, 200, 200);
	shader->setUniform1f("pointLight.brightness", 32);
	shader->setUniform3f("pointLight.color", 0.0, 1.0, 1.0);

	shader->setUniform3f("directionalLight.direction", 200, 200, 200);
	shader->setUniform1f("directionalLight.brightness", 32);
	shader->setUniform3f("directionalLight.color", 1.0, 0.0, 1.0);

	shader->setUniform3f("projectorLight.position", 200, 200, -200);
	shader->setUniform3f("projectorLight.direction", -1, -1, 1);
	shader->setUniform1f("projectorLight.brightness", 32);
	shader->setUniform1f("projectorLight.cutoffAngle", 20.0);
	shader->setUniform1f("projectorLight.outerCutoffAngle", 30.0);
	shader->setUniform3f("projectorLight.color", 1.0, 1.0, 0.0);

	shader->setUniformTexture("diffuseMap", filteredTexture, 0);
	shader->setUniformTexture("normalMap", normalMap, 1);

	ofFloatColor ambiant = ofFloatColor(materialAmbiant);
	shader->setUniform3f("material.ambiant", ambiant.r, ambiant.g, ambiant.b);
	ofFloatColor diffuse = ofFloatColor(materialDiffuse);
	shader->setUniform3f("material.diffuse", diffuse.r, diffuse.g, diffuse.b);
	ofFloatColor specular = ofFloatColor(materialSpecular);
	shader->setUniform3f("material.specular", specular.r, specular.g, specular.b);

	shader->setUniform3f("ambiant", ofVec3f(0.1, 0.1, 0.1));
	//shader->setUniform3f("camera", cam->getPosition());

	drawShaderOverride();

	shader->end();


	for (Object* child : children) {
		child->drawShader(shader);
	}

	ofPopMatrix();
}

void Object::checkIfSelected() {
	if (selected) {
		drawBoundingBox();
	}
	else {
		ofPushMatrix();

		ofTranslate(translationX, translationY, translationZ);

		ofRotateXDeg(rotationX);
		ofRotateYDeg(rotationY);
		ofRotateZDeg(rotationZ);

		ofScale(scaleX, scaleY, scaleZ);

		for (Object* child : children) {
			child->checkIfSelected();
		}

		ofPopMatrix();
	}
}

void Object::drawBoundingBox() {
	ofPushMatrix();

	ofTranslate(translationX, translationY, translationZ);

	ofRotateXDeg(rotationX);
	ofRotateYDeg(rotationY);
	ofRotateZDeg(rotationZ);

	ofScale(scaleX, scaleY, scaleZ);

	drawBoundingBoxOverride();

	for (Object* child : children) {
		child->drawBoundingBox();
	}

	ofPopMatrix();
}