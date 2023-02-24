#include "application3d.h"
#include "./3d/object/object.h"
#include "./3d/object/transformation.h"
#include "./3d/object/operation.h"
#include "./3d/object/loadedFile.h"
#include "./3d/object/cylinderPrimitive.h"
#include "./3d/object/spherePrimitive.h"

bool exporting = false;
bool isWPressed = false;
bool isAPressed = false;
bool isSPressed = false;
bool isDPressed = false;
bool isQPressed = false;
bool isEPressed = false;

ofImage* downloadCursor;

vector<Object*> everything;
vector<Object*> selection;
stack<Operation*> history;
stack<Operation*> historyUndone;

ofxDatGui* headerUi;

ofxDatGui* objectMenu;
ofxDatGuiDropdown* renderModeDropdown;

ofxDatGuiScrollView* objectScrollView;
ofxDatGuiScrollView* selectionScrollView;

ofxDatGui* transformationMenu;
ofxDatGuiDropdown* transformationDropdown;
ofxDatGuiSlider* xAxisSlider;
ofxDatGuiSlider* yAxisSlider;
ofxDatGuiSlider* zAxisSlider;

void Application3d::setup(ofxDatGui* header)
{
	renderer.setup();

	headerUi = header;

	downloadCursor = new ofImage();
	downloadCursor->load("/cursors/download-cursor.png");

	objectMenu = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
	objectMenu->setVisible(false);
	objectMenu->setPosition(0, headerUi->getHeight() - 1);
	objectMenu->setWidth(256);

	ofxDatGuiButton* importBtn = objectMenu->addButton("Import");
	importBtn->onButtonEvent(this, &Application3d::onImportEvent);
	ofxDatGuiButton* undoBtn = objectMenu->addButton("Undo");
	undoBtn->onButtonEvent(this, &Application3d::onUndoEvent);
	ofxDatGuiButton* redoBtn = objectMenu->addButton("Redo");
	redoBtn->onButtonEvent(this, &Application3d::onRedoEvent);	
	vector<string> renderModeOptions = { "Wireframe", "Solid", "Shader" };
	renderModeDropdown = objectMenu->addDropdown("Render Mode", renderModeOptions);
	ofxDatGuiButton* addCylinderBtn = objectMenu->addButton("Add Cylinder");
	addCylinderBtn->onButtonEvent(this, &Application3d::onAddCylinderEvent);
	ofxDatGuiButton* addShpereBtn = objectMenu->addButton("Add Sphere");
	addShpereBtn->onButtonEvent(this, &Application3d::onAddSphereEvent);
	ofxDatGuiButton* deleteBtn = objectMenu->addButton("Delete");
	deleteBtn->onButtonEvent(this, &Application3d::onDeleteEvent);
	ofxDatGuiButton* enableTurntableBtn = objectMenu->addButton("Enable Turntable");
	enableTurntableBtn->onButtonEvent(this, &Application3d::onEnableTurntable);
	ofxDatGuiButton* enableTranslationAnimBtn = objectMenu->addButton("Enable Translation Animation");
	enableTranslationAnimBtn->onButtonEvent(this, &Application3d::onEnableTranslationAnimation);
	ofxDatGuiButton* changeCameraModeBtn = objectMenu->addButton("Switch Camera Mode");
	changeCameraModeBtn->onButtonEvent(this, &Application3d::onChangeCameraMode);

	objectScrollView = new ofxDatGuiScrollView("My scroll view", 100);
	objectScrollView->setWidth(255);
	objectScrollView->setPosition(ofGetWidth() - 255, headerUi->getHeight() - 1);
	objectScrollView->onScrollViewEvent(this, &Application3d::onObjectSelection);

	selectionScrollView = new ofxDatGuiScrollView("My scroll view", 100);
	selectionScrollView->setPosition(objectMenu->getWidth(), headerUi->getHeight() - 1);
	selectionScrollView->setOpacity(0.1);

	transformationMenu = new ofxDatGui(300, 300);
	transformationMenu->addLabel("Transformation Menu");
	transformationMenu->addHeader(":: Click here to drag ::");
	vector<string> transformationOptions = {"Translation", "Rotation", "Proportion"};
	transformationDropdown = transformationMenu->addDropdown("Transformation Type", transformationOptions);
	xAxisSlider = transformationMenu->addSlider("X", -1000, 1000, 0);
	yAxisSlider = transformationMenu->addSlider("Y", -1000, 1000, 0);
	zAxisSlider = transformationMenu->addSlider("Z", -1000, 1000, 0);
	ofxDatGuiButton* applyButton = transformationMenu->addButton("Apply");
	applyButton->onButtonEvent(this, &Application3d::onApplyTransformationEvent);
}

void Application3d::draw() {
	renderer.draw(getRenderMode(), selection);

	if (exporting && ofGetFrameNum()%2==0) {
		ofLog() << "exporting";
		ofImage img;
		img.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		string filename = "/export/" + to_string(ofGetFrameNum()) + ".png";
		img.save(filename);
	}

	objectScrollView->draw(); 
	selectionScrollView->setHeight(selectionScrollView->getNumItems() * 26);
	selectionScrollView->draw();


	float x = static_cast<float>(ofGetMouseX());
	float y = static_cast<float>(ofGetMouseY());

	if (exporting) {
		ofHideCursor();
		downloadCursor->draw(x - (downloadCursor->getWidth() / 2), y - (downloadCursor->getWidth() / 2));
	} else {
		ofShowCursor();
	}
}

void Application3d::update() {
	renderer.update();
	objectScrollView->update();

	for (int i = 0; i < everything.size(); i++) {
		selectionScrollView->remove(0);
	}

	for (Object* object : everything) {
		object->selected = false;

		if (object->rotation_animation)
			object->rotationY = ofGetFrameNum() * 0.3;

		if (object->translation_animation) {
			int pos = ofGetFrameNum() % 360;
			if (pos >= 180) {
				object->translationX += 5;
			}
			else {
				object->translationX -= 5;
			}
		}
	}

	for (Object* object : selection) {
		selectionScrollView->add(object->name);
		object->selected = true;
	}

	if (isWPressed)
		renderer.camera->dolly(-5);
	if (isSPressed)
		renderer.camera->dolly(5);
	if (isAPressed)
		renderer.camera->truck(-5);
	if (isDPressed)
		renderer.camera->truck(5);
	if (isQPressed)
		renderer.camera->boom(5);
	if (isEPressed)
		renderer.camera->boom(-5);

}

void Application3d::showUi() {
	objectMenu->setVisible(true);
	transformationMenu->setVisible(true);
}

void Application3d::hideUi() {
	objectMenu->setVisible(false);
	transformationMenu->setVisible(false);
}

void Application3d::keyPressed(int key) {
	if (key == 48) // 0
		exporting = true;
	if (key == 119) // w
		isWPressed = true;
	if (key == 97) // a
		isAPressed = true;
	if (key == 115) // s
		isSPressed = true;
	if (key == 100) // d
		isDPressed = true;
	if (key == 113) // q
		isQPressed = true;
	if (key == 101) // e
		isEPressed = true;
}

void Application3d::keyReleased(int key) {
	if (key == 48) // 0
		exporting = false;
	if (key == 119) // w
		isWPressed = false;
	if (key == 97) // a
		isAPressed = false;
	if (key == 115) // s
		isSPressed = false;
	if (key == 100) // d
		isDPressed = false;
	if (key == 113) // q
		isQPressed = false;
	if (key == 101) // e
		isEPressed = false;
}

void Application3d::mousePressed(int x, int y, int button) {
}

void Application3d::mouseReleased(int x, int y, int button) {
	ofLog() << "Mouse release at : " << x << ", " << y;
}

void Application3d::windowResized(int w, int h) {
	objectScrollView->setPosition(ofGetWidth() - 255, headerUi->getHeight() - 1);
}

void Application3d::dragEvent(ofDragInfo dragInfo) {
	ofLog() << "<app::ofDragInfo file[0]: " << dragInfo.files.at(0)
		<< " at : " << dragInfo.position << ">";

	import(dragInfo.files.at(0));
}

void Application3d::onImportEvent(ofxDatGuiButtonEvent e) {
	ofLog() << "<app::import>";
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an object");
	if (openFileResult.bSuccess) {
		import(openFileResult.getPath());
		ofLog() << "<app::import - success>";
	}
	else {
		ofLog() << "<app::import - failed>";
	}
}

void Application3d::onObjectSelection(ofxDatGuiScrollViewEvent e) {
	Object* obj = everything.at(e.target->getIndex());
	if (find(selection.begin(), selection.end(), obj) != selection.end()) {
		selection.erase(std::find(selection.begin(), selection.end(), obj));
	}
	else {
		selection.push_back(obj);
	}
}

void Application3d::onApplyTransformationEvent(ofxDatGuiButtonEvent e) {
	Transformation* transformation = new Transformation();

	if (transformationDropdown->getSelected()->getLabel() == "Translation") {
		transformation->translationX = xAxisSlider->getValue();
		transformation->translationY = yAxisSlider->getValue();
		transformation->translationZ = zAxisSlider->getValue();
	}
	else if (transformationDropdown->getSelected()->getLabel() == "Rotation") {
		transformation->rotationX = xAxisSlider->getValue();
		transformation->rotationY = yAxisSlider->getValue();
		transformation->rotationZ = zAxisSlider->getValue();
	}
	else if (transformationDropdown->getSelected()->getLabel() == "Proportion") {
		transformation->scaleX = xAxisSlider->getValue();
		transformation->scaleY = yAxisSlider->getValue();
		transformation->scaleZ = zAxisSlider->getValue();
	}
	Operation* operation = new Operation();
	operation->targets = selection;
	operation->transformation = transformation;
	operation->apply();
	history.push(operation);
}

void Application3d::onUndoEvent(ofxDatGuiButtonEvent e) {
	undo();
}

void Application3d::onRedoEvent(ofxDatGuiButtonEvent e) {
	redo();
}

void Application3d::onAddCylinderEvent(ofxDatGuiButtonEvent e) {
	CylinderPrimitive* cylinderPrimitive = new CylinderPrimitive();
	cylinderPrimitive->cylinder = new ofCylinderPrimitive();
	cylinderPrimitive->cylinder->setRadius(100);
	cylinderPrimitive->cylinder->setHeight(200);
	cylinderPrimitive->cylinder->setResolutionRadius(30);
	cylinderPrimitive->cylinder->setPosition({ 0, 0, 0 });

	std::string filename = "cylinder";
	cylinderPrimitive->originalName = filename;
	filename = getElementName(filename);
	addObject(cylinderPrimitive, filename);
}

void Application3d::onAddSphereEvent(ofxDatGuiButtonEvent e) {
	SpherePrimitive* spherePrimitive = new SpherePrimitive();
	spherePrimitive->sphere = new ofSpherePrimitive();
	spherePrimitive->sphere->setRadius(100);
	spherePrimitive->sphere->setPosition({ 0, 0, 0 });

	std::string filename = "sphere";
	spherePrimitive->originalName = filename;
	filename = getElementName(filename);
	addObject(spherePrimitive, filename);
}

void Application3d::onEnableTurntable(ofxDatGuiButtonEvent e) {
	for (Object* object : selection) {
		object->rotation_animation = !object->rotation_animation;
	}
}

void Application3d::onEnableTranslationAnimation(ofxDatGuiButtonEvent e) {
	for (Object* object : selection) {
		object->translation_animation = !object->translation_animation;
	}
}

void Application3d::onChangeCameraMode(ofxDatGuiButtonEvent e) {
	if (renderer.cameraMode == Renderer3d::Perspective) {
		renderer.setCameraToOrthographic();
		renderer.cameraMode = Renderer3d::Orthographic;
	}
	else if (renderer.cameraMode == Renderer3d::Orthographic) {
		renderer.setCameraToPerspective();
		renderer.cameraMode = Renderer3d::Perspective;
	}
}

void Application3d::onDeleteEvent(ofxDatGuiButtonEvent e) {
	while (!selection.empty()) {
		Object* object = selection.at(0);
		auto itInSelection = find(selection.begin(), selection.end(), object);
		selectionScrollView->remove(itInSelection - selection.begin());
		selection.erase(find(selection.begin(), selection.end(), object));

		if (object->parent != nullptr) {
			if (count(object->parent->children.begin(), object->parent->children.end(), object))
				object->parent->children.erase(find(object->parent->children.begin(), object->parent->children.end(), object));
		}

		for (Object* child : object->children) {
			child->parent = nullptr;
			renderer.scene->objects.push_back(child);
		}

		auto it = find(everything.begin(), everything.end(), object);
		objectScrollView->remove(it - everything.begin());
		if (count(renderer.scene->objects.begin(), renderer.scene->objects.end(), object))
			renderer.scene->objects.erase(find(renderer.scene->objects.begin(), renderer.scene->objects.end(), object));
		everything.erase(it);
		delete object;
	}
}

void Application3d::import(string path) {
	ofxAssimpModelLoader* model = new ofxAssimpModelLoader();
	if (model->loadModel(path)) {
		LoadedFile* obj = new LoadedFile();
		obj->model = model;

		std::string filename;
		std::string::size_type idx = path.rfind('\\');
		if (idx != std::string::npos) {
			filename = path.substr(idx + 1);
		}
		obj->originalName = filename;
		filename = getElementName(filename);

		addObject(obj, filename);
	} else {
		ofLog() << "<app::import - failed>";
	}
}

string Application3d::getElementName(string filename) {
	int duplicate = 0;
	for (Object* element : getAllElementFromScene()) {
		if (element->originalName == filename) {
			duplicate++;
		}
	}

	if (duplicate > 0) {
		filename = filename + " (" + std::to_string(duplicate) + ")";
	}
	return filename;
}

std::vector<Object*> Application3d::getAllElementFromScene() {
	std::vector<Object*> everything;

	for (Object* children : renderer.scene->objects) {
		everything.push_back(children);
		std::vector<Object*> everythingFromChildren = getAllElementFromObject(children);
		everything.insert(everything.end(), everythingFromChildren.begin(), everythingFromChildren.end());
	}

	return everything;
}

std::vector<Object*> Application3d::getAllElementFromObject(Object* object) {
	std::vector<Object*> everything;

	for (Object* children : object->children) {
		std::vector<Object*> everythingFromChildren = getAllElementFromObject(children);
		everything.insert(everything.end(), everythingFromChildren.begin(), everythingFromChildren.end());
	}

	return everything;
}

void Application3d::undo() {
	ofLog() << "undo";
	if (history.size() > 0) {
		Operation* operation = history.top();
		operation->rollback();
		historyUndone.push(operation);
		history.pop();
	}
}

void Application3d::redo() {
	ofLog() << "redo";
	if (historyUndone.size() > 0) {
		Operation* operation = historyUndone.top();
		operation->apply();
		history.push(operation);
		historyUndone.pop();
	}
}

Renderer3d::RenderMode Application3d::getRenderMode() {
	string renderMode = renderModeDropdown->getSelected()->getLabel();
	if (renderMode == "Wireframe")
		return Renderer3d::RenderMode::Wireframe;
	if (renderMode == "Solid")
		return Renderer3d::RenderMode::Solid;
	if (renderMode == "Shader")
		return Renderer3d::RenderMode::Shader;
}

void Application3d::addObject(Object* object, string filename) {
	if (selection.empty()) {
		object->name = filename;
		renderer.scene->objects.push_back(object);
	}
	else {
		Object* parent = selection.at(0);
		object->name = filename;
		parent->children.push_back(object);
		object->parent = parent;
	}

	objectScrollView->add(object->name);

	everything.push_back(object);

	selection.clear();
	selection.push_back(object);
}
