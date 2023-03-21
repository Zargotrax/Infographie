#include "renderer3d.h"

void Renderer3d::setup()
{
	scene = new Scene();
	camera = new ofEasyCam();

	camera->setupPerspective();
	camera->setDistance(1000);

	toneMapping = new ofShader();
	toneMapping->load("shaders/tone_mapping_330_vs.glsl", "shaders/tone_mapping_330_fs.glsl");

	lambert = new ofShader();
	lambert->load("shaders/basic_shader.vert", "shaders/lambert.frag");

	phong = new ofShader();
	phong->load("shaders/basic_shader.vert", "shaders/phong.frag");

	blinnPhong = new ofShader();
	blinnPhong->load("shaders/basic_shader.vert", "shaders/blinn-phong.frag");

	light.enable();
	light.setPosition(ofVec3f(100, 100, 500));
	light.lookAt(ofVec3f(0, 0, 0));

}

void Renderer3d::update() {

}

void Renderer3d::draw(Renderer3d::RenderMode renderMode, vector<Object*> selected)
{
	ofPushStyle();

	camera->begin();

	if (renderMode != Renderer3d::RenderMode::Wireframe) {
		ofEnableDepthTest();
		ofEnableLighting();
	}

	for (Object* object : scene->objects) {

		switch (renderMode) {
			case Renderer3d::RenderMode::Wireframe :
				ofSetColor(ofColor::black);
				object->drawWireframe();
				ofSetColor(ofColor::white);
			break;
			case Renderer3d::RenderMode::Texture:
				object->drawTexture(toneMapping);
			break;
			case Renderer3d::RenderMode::Lambert:
				object->drawShader(lambert);
			break;
			case Renderer3d::RenderMode::Phong:
				object->drawShader(phong);
			break;
			case Renderer3d::RenderMode::Blinn_Phong:
				object->drawShader(blinnPhong);
			break;
		}

		ofSetColor(ofColor::orangeRed);
		object->checkIfSelected();
		ofSetColor(ofColor::white);
	}

	camera->end();
	ofDisableDepthTest();
	ofDisableLighting();
	ofPopStyle();
}

void Renderer3d::setCameraToPerspective() {
	camera->enableOrtho();
}

void Renderer3d::setCameraToOrthographic() {
	camera->disableOrtho();
}
