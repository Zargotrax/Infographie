#include "renderer3d.h"

void Renderer3d::setup()
{
	scene = new Scene();
	camera = new ofEasyCam();

	camera->setupPerspective();
	camera->setDistance(1000);

	shader = new ofShader();
	shader->load("lambert_330_vs.glsl", "lambert_330_fs.glsl");
}

void Renderer3d::update() {

	light.setPointLight();
	light.setDiffuseColor(255);
	light.setGlobalPosition(ofGetWidth() / 2, ofGetHeight() / 2, 500);

	shader->begin();
	shader->setUniform3f("color_ambient", 0.1f, 0.1f, 0.1f);
	shader->setUniform3f("color_diffuse", 1, 1, 1);
	shader->setUniform3f("light_position", light.getGlobalPosition());
	shader->end();
}

void Renderer3d::draw(Renderer3d::RenderMode renderMode, vector<Object*> selected)
{
	if (renderMode != Renderer3d::RenderMode::Wireframe) {
		ofEnableLighting();
		ofEnableDepthTest();
		light.enable();
	}

	shader->begin();

	camera->begin();

	for (Object* object : scene->objects) {

		switch (renderMode) {
			case Renderer3d::RenderMode::Wireframe :
				ofSetColor(ofColor::black);
				object->drawWireframe();
				ofSetColor(ofColor::white);
			break;
			case Renderer3d::RenderMode::Solid:
				object->drawSolid();
			break;
			case Renderer3d::RenderMode::Shader:
				object->drawShader();
			break;
		}
	}
	
	for (Object* object : selected) {
		object->drawBoundingBox();
	}

	shader->end();
	camera->end();
	light.disable();
	ofDisableDepthTest();
	ofDisableLighting();
}

void Renderer3d::setCameraToPerspective() {
	camera->enableOrtho();
}

void Renderer3d::setCameraToOrthographic() {
	camera->disableOrtho();
}
