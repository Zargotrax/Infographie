#include "application3d.h"
#include "./3d/object/object.h"
#include "./3d/object/transformation.h"
#include "./3d/object/operation.h"
#include "./3d/object/loadedFile.h"
#include "./3d/object/cylinderPrimitive.h"
#include "./3d/object/spherePrimitive.h"

vector<Object*> everything;
vector<Object*> selection;
queue<Operation*> history;
queue<Operation*> historyUndone;

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
	renderer.draw(getRenderMode());
	objectScrollView->draw(); 
	selectionScrollView->setHeight(selectionScrollView->getNumItems() * 26);
	selectionScrollView->draw();
}

void Application3d::update() {
	renderer.update();
	objectScrollView->update();

	for (int i = 0; i < everything.size(); i++) {
		selectionScrollView->remove(0);
	}

	for (Object* object : everything) {
		object->selected = false;
	}

	for (Object* object : selection) {
		selectionScrollView->add(object->name);
		object->selected = true;
	}
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
}

void Application3d::keyReleased(int key) {
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
	if (std::find(selection.begin(), selection.end(), obj) != selection.end()) {
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
	cylinderPrimitive->name = filename;

	objectScrollView->add(filename);

	renderer.scene->objects.push_back(cylinderPrimitive);
	everything.push_back(cylinderPrimitive);

	selection.clear();
	selection.push_back(cylinderPrimitive);
}

void Application3d::onAddSphereEvent(ofxDatGuiButtonEvent e) {
	SpherePrimitive* spherePrimitive = new SpherePrimitive();
	spherePrimitive->sphere = new ofSpherePrimitive();
	spherePrimitive->sphere->setRadius(100);
	spherePrimitive->sphere->setPosition({ 0, 0, 0 });

	std::string filename = "sphere";
	spherePrimitive->originalName = filename;
	filename = getElementName(filename);
	spherePrimitive->name = filename;

	objectScrollView->add(filename);

	renderer.scene->objects.push_back(spherePrimitive);
	everything.push_back(spherePrimitive);

	selection.clear();
	selection.push_back(spherePrimitive);
}

void Application3d::import(string path) {
	ofxAssimpModelLoader* model = new ofxAssimpModelLoader();
	model->loadModel(path);
	LoadedFile* obj = new LoadedFile();
	obj->model = model;

	std::string filename;
	std::string::size_type idx = path.rfind('\\');
	if (idx != std::string::npos) {
		filename = path.substr(idx + 1);
	}

	obj->originalName = filename;
	filename = getElementName(filename);
	obj->name = filename;

	objectScrollView->add(filename);

	renderer.scene->objects.push_back(obj);
	everything.push_back(obj);

	selection.clear();
	selection.push_back(obj);
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
		Operation* operation = history.front();
		operation->rollback();
		historyUndone.push(operation);
		history.pop();
	}
}

void Application3d::redo() {
	ofLog() << "redo";
	if (historyUndone.size() > 0) {
		Operation* operation = historyUndone.front();
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
