#include "renderer3d.h"

void Renderer3d::setup()
{
	scene = new Scene();
	camera = new ofEasyCam();

	toneMapping = new ofShader();
	toneMapping->load("shaders/tone_mapping_330_vs.glsl", "shaders/tone_mapping_330_fs.glsl");

	lambert = new ofShader();
	lambert->load("shaders/basic_shader.vert", "shaders/lambert.frag");

	phong = new ofShader();
	phong->load("shaders/basic_shader.vert", "shaders/phong.frag");

	blinnPhong = new ofShader();
	blinnPhong->load("shaders/basic_shader.vert", "shaders/blinn-phong.frag");

	pbr = new ofShader();
	pbr->load("shaders/pbr_330_vs.glsl", "shaders/pbr_330_fs.glsl");

	ambiantLight.color = ofColor::black;
	pointLight.position = ofVec3f(- 200, 200, 200);
	pointLight.brightness = 40;
	pointLight.color = ofColor::white;
	directionalLight.position = ofVec3f(200, 200, 200);
	directionalLight.direction = ofVec3f(1, 1, 1);
	directionalLight.color = ofColor::darkGreen;
	directionalLight.brightness = 40;
	spotLight.position = ofVec3f(-200, -200, -200);
	spotLight.brightness = 40;
	spotLight.direction = ofVec3f(-1, -1, -1);
	spotLight.color = ofColor::blueSteel;
	spotLight.innerCutoff = 10;
	spotLight.outerCutoff = 15;

	tesselator = new TesselatorUtil();
	tesselator->setup();
}

void Renderer3d::update() {

}

void Renderer3d::draw(Renderer3d::RenderMode renderMode, vector<Object*> selected) {
	ofPushStyle();

	if (renderMode == RenderMode::Tesselation) {
		tesselator->draw();
		ofPopStyle();
		return;
	}

	camera->begin();

	if (renderMode != RenderMode::Wireframe) {
		ofEnableDepthTest();
	}
	if (renderMode == RenderMode::Lambert or renderMode == RenderMode::Phong or renderMode == RenderMode::Blinn_Phong) {
		ofSetColor(pointLight.color);
		ofDrawSphere(pointLight.position, 20);
		ofSetColor(directionalLight.color);
		ofDrawSphere(directionalLight.position, 20);
		ofSetColor(spotLight.color);
		ofDrawSphere(spotLight.position, 20);
		ofSetColor(ofColor::white);
	}

	for (Object* object : scene->objects) {

		switch (renderMode) {
			case RenderMode::Wireframe :
				ofSetColor(ofColor::black);
				object->drawWireframe();
				ofSetColor(ofColor::white);
			break;
			case RenderMode::Texture:
				object->drawTexture(toneMapping);
			break;
			case RenderMode::Lambert:
				object->drawShader(lambert, ambiantLight, pointLight, directionalLight, spotLight);
			break;
			case RenderMode::Phong:
				object->drawShader(phong, ambiantLight, pointLight, directionalLight, spotLight);
			break;
			case RenderMode::Blinn_Phong:
				object->drawShader(blinnPhong, ambiantLight, pointLight, directionalLight, spotLight);
			break;
			case RenderMode::PBR:
				object->drawPBR(pbr, pointLight, directionalLight, spotLight);
				break;
		}

		ofSetColor(ofColor::orangeRed);
		object->checkIfSelected();
		ofSetColor(ofColor::white);
	}

	camera->end();
	ofDisableDepthTest();
	ofPopStyle();
}

void Renderer3d::setCameraToPerspective() {
	camera->enableOrtho();
}

void Renderer3d::setCameraToOrthographic() {
	camera->disableOrtho();
}
