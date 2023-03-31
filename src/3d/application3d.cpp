#include "application3d.h"
#include "./3d/object/object.h"
#include "./3d/object/transformation.h"
#include "./3d/object/operation.h"
#include "./3d/object/loadedFile.h"
#include "./3d/object/cylinderPrimitive.h"
#include "./3d/object/spherePrimitive.h"
#include "./3d/object/curve.h"
#include "./3d/object/customObject.h"
#include "./3d/object/surface.h"

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

ofxDatGui* textureMenu;
ofxDatGuiDropdown* textureMagFilterDropdown;
ofxDatGuiDropdown* textureFilterDropdown;
ofxDatGuiSlider* exposureSlider;
ofxDatGuiSlider* gammaSlider;

ofxDatGui* materialMenu;
ofxDatGuiColorPicker* materialAmbiantCP;
ofxDatGuiColorPicker* materialDiffuseCP;
ofxDatGuiColorPicker* materialSpecularCP;

ofxDatGui* curveMenu;
ofxDatGuiDropdown* curvePointControlDropdown;
ofxDatGuiSlider* curveXSlider;
ofxDatGuiSlider* curveYSlider;
ofxDatGuiSlider* curveZSlider;

ofxDatGui* surfaceMenu;
ofxDatGuiDropdown* surfacePointControlDropdown;
ofxDatGuiSlider* surfaceXSlider;
ofxDatGuiSlider* surfaceYSlider;
ofxDatGuiSlider* surfaceZSlider;

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
	vector<string> renderModeOptions = { "Wireframe", "Texture", "Lambert", "Phong", "Blinn-Phong"};
	renderModeDropdown = objectMenu->addDropdown("Render Mode", renderModeOptions);
	ofxDatGuiButton* addCylinderBtn = objectMenu->addButton("Add Cylinder");
	addCylinderBtn->onButtonEvent(this, &Application3d::onAddCylinderEvent);
	ofxDatGuiButton* addShpereBtn = objectMenu->addButton("Add Sphere");
	addShpereBtn->onButtonEvent(this, &Application3d::onAddSphereEvent);
	ofxDatGuiButton* addBezierCurbeBtn = objectMenu->addButton("Add Bezier Curve");
	addBezierCurbeBtn->onButtonEvent(this, &Application3d::onAddBezierCurveEvent);
	ofxDatGuiButton* addBezierSurfaceBtn = objectMenu->addButton("Add Bezier Surface");
	addBezierSurfaceBtn->onButtonEvent(this, &Application3d::onAddBezierSurfaceEvent);
	ofxDatGuiButton* addCustomObjectBtn = objectMenu->addButton("Add Custom Object");
	addCustomObjectBtn->onButtonEvent(this, &Application3d::onAddCustomObjectEvent);
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

	transformationMenu = new ofxDatGui(850, 250);
	transformationMenu->addLabel("Transformation Menu");
	transformationMenu->addHeader(":: Click here to drag ::");
	transformationMenu->addFooter();
	vector<string> transformationOptions = {"Translation", "Rotation", "Proportion"};
	transformationDropdown = transformationMenu->addDropdown("Transformation Type", transformationOptions);
	xAxisSlider = transformationMenu->addSlider("X", -1000, 1000, 0);
	yAxisSlider = transformationMenu->addSlider("Y", -1000, 1000, 0);
	zAxisSlider = transformationMenu->addSlider("Z", -1000, 1000, 0);
	ofxDatGuiButton* applyButton = transformationMenu->addButton("Apply");
	applyButton->onButtonEvent(this, &Application3d::onApplyTransformationEvent);

	textureMenu = new ofxDatGui(850, 460);
	textureMenu->addLabel("Texture Menu");
	textureMenu->addHeader(":: Click here to drag ::");
	textureMenu->addFooter();
	ofxDatGuiButton* textureImportBtn = textureMenu->addButton("Import Diffuse Texture");
	textureImportBtn->onButtonEvent(this, &Application3d::onImportTextureEvent);
	ofxDatGuiButton* normalMapImportBtn = textureMenu->addButton("Import Normal Map");
	normalMapImportBtn->onButtonEvent(this, &Application3d::onImportNormalMapEvent);
	ofxDatGuiButton* randomTextureEvent = textureMenu->addButton("Random");
	randomTextureEvent->onButtonEvent(this, &Application3d::onRandomTextureEvent);
	vector<string> magFilterOptions = { "Nearest", "Linear" };
	textureMagFilterDropdown = textureMenu->addDropdown("Image Scaling", magFilterOptions);
	textureMagFilterDropdown->onDropdownEvent(this, &Application3d::onTextureMagFilterDropdownSelection);
	vector<string> filterOptions = { "None", "Gaussian Blur", "Sharpen", "Edge Detect"};
	textureFilterDropdown = textureMenu->addDropdown("Texture Filter", filterOptions);
	textureFilterDropdown->onDropdownEvent(this, &Application3d::onTextureFilterDropdownSelection);
	textureMenu->addLabel("Tone Mapping");
	exposureSlider = textureMenu->addSlider("Exposure", 0, 5, 1);
	exposureSlider->onSliderEvent(this, &Application3d::onToneMappingEvent);
	gammaSlider = textureMenu->addSlider("Gamma", 0, 5, 2.2);
	gammaSlider->onSliderEvent(this, &Application3d::onToneMappingEvent);

	materialMenu = new ofxDatGui(50, 550);
	materialMenu->addLabel("Material Menu");
	materialMenu->addHeader(":: Click here to drag ::");
	materialAmbiantCP = materialMenu->addColorPicker("Ambiant", ofFloatColor(0.1, 0.1, 0.1));
	materialAmbiantCP->onColorPickerEvent(this, &Application3d::onMaterialColorChangeEvent);
	materialDiffuseCP = materialMenu->addColorPicker("Diffuse", ofFloatColor(0.0, 0.6, 0.6));
	materialDiffuseCP->onColorPickerEvent(this, &Application3d::onMaterialColorChangeEvent);
	materialSpecularCP = materialMenu->addColorPicker("Specular", ofFloatColor(1.0, 0.0, 1.0));
	materialSpecularCP->onColorPickerEvent(this, &Application3d::onMaterialColorChangeEvent);
	materialMenu->addFooter();

	curveMenu = new ofxDatGui(50, 350);
	curveMenu->addLabel("Bezier Curve Menu");
	curveMenu->addHeader(":: Click here to drag ::");
	vector<string> curveControlPoints = { "1", "2", "3", "4" };
	curvePointControlDropdown = curveMenu->addDropdown("Control Point", curveControlPoints);
	curveXSlider = curveMenu->addSlider("Position X", -500, 500, 0);
	curveXSlider->onSliderEvent(this, &Application3d::onCurveControlPointPositionChangeEvent);
	curveYSlider =curveMenu->addSlider("Position Y", -500, 500, 0);
	curveYSlider->onSliderEvent(this, &Application3d::onCurveControlPointPositionChangeEvent);
	curveZSlider = curveMenu->addSlider("Position Z", -500, 500, 0);
	curveZSlider->onSliderEvent(this, &Application3d::onCurveControlPointPositionChangeEvent);

	surfaceMenu = new ofxDatGui(50, 350);
	surfaceMenu->addLabel("Bezier Surface Menu");
	surfaceMenu->addHeader(":: Click here to drag ::");
	vector<string> surfaceControlPoints = { "0", "1", "2", "3", "4", "5", "6", "7", "8"};
	surfacePointControlDropdown = surfaceMenu->addDropdown("Control Point", surfaceControlPoints);
	surfacePointControlDropdown->onDropdownEvent(this, &Application3d::onSurfacePointControlSelectionEvent);
	surfaceXSlider = surfaceMenu->addSlider("Position X", -500, 500, 0);
	surfaceXSlider->onSliderEvent(this, &Application3d::onSurfaceControlPointPositionChangeEvent);
	surfaceYSlider = surfaceMenu->addSlider("Position Y", -500, 500, 0);
	surfaceYSlider->onSliderEvent(this, &Application3d::onSurfaceControlPointPositionChangeEvent);
	surfaceZSlider = surfaceMenu->addSlider("Position Z", -500, 500, 0);
	surfaceZSlider->onSliderEvent(this, &Application3d::onSurfaceControlPointPositionChangeEvent);
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
	if (!selection.empty()) {
		if (dynamic_cast<Curve*>(selection.at(0)) != nullptr) {
			curveMenu->setVisible(true);
		} else if (dynamic_cast<Surface*>(selection.at(0)) != nullptr) {
			surfaceMenu->setVisible(true);
		} else {
			curveMenu->setVisible(false);
			surfaceMenu->setVisible(false);
		}
	} else {
		curveMenu->setVisible(false);
		surfaceMenu->setVisible(false);
	}

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
	textureMenu->setVisible(true);
	materialMenu->setVisible(true);
	curveMenu->setVisible(true);
	surfaceMenu->setVisible(true);
}

void Application3d::hideUi() {
	objectMenu->setVisible(false);
	transformationMenu->setVisible(false);
	textureMenu->setVisible(false);
	materialMenu->setVisible(false);
	curveMenu->setVisible(false);
	surfaceMenu->setVisible(false);
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
	if (button == 2 && !selection.empty()) { // right click
		if (dynamic_cast<CustomObject*>(selection.at(0)) != nullptr) {
			CustomObject* customObject = dynamic_cast<CustomObject*>(selection.at(0));
			ofVec3f worldMouse = renderer.camera->screenToWorld(ofVec3f(x, y, 0));
			ofVec3f direction = worldMouse - renderer.camera->getPosition();
			direction.normalize();
			glm::vec3 newPoint = renderer.camera->getPosition() + direction * 500;
			customObject->triangulator->addPoint(newPoint.x, newPoint.y, newPoint.z);
			customObject->triangulator->triangulate();
		}
	}
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

	std::string filename = "Cylinder";
	cylinderPrimitive->originalName = filename;
	filename = getElementName(filename);
	addObject(cylinderPrimitive, filename);
}

void Application3d::onAddSphereEvent(ofxDatGuiButtonEvent e) {
	SpherePrimitive* spherePrimitive = new SpherePrimitive();
	spherePrimitive->sphere = new ofSpherePrimitive();
	spherePrimitive->sphere->setRadius(100);
	spherePrimitive->sphere->setPosition({ 0, 0, 0 });

	std::string filename = "Sphere";
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

void Application3d::onRandomTextureEvent(ofxDatGuiButtonEvent e) {
	for (Object* object : selection) {
		ofTexture texture;
		ofPixels pixels;
		int width = 256;
		int height = 256;
		pixels.allocate(width, height, OF_PIXELS_RGB);
		float noiseScale = ofRandom(1);
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				float noiseValue = ofNoise(x * noiseScale * ofRandom(1), y * noiseScale * ofRandom(1));
				ofColor color(noiseValue * 255, noiseValue * 255, noiseValue * 255);
				pixels.setColor(x, y, color);
			}
		}
		object->texture.allocate(pixels);
		object->filteredTexture.allocate(pixels);
	}
}

void Application3d::onImportTextureEvent(ofxDatGuiButtonEvent e) {
	ofLog() << "<app::import texture>";
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image");
	if (openFileResult.bSuccess) {
		for (Object* object : selection) {
			ofLoadImage(object->texture, openFileResult.filePath);
			object->filteredTexture = object->texture;
		}
		ofLog() << "<app::import - success>";
	}
	else {
		ofLog() << "<app::import - failed>";
	}
}

void Application3d::onTextureMagFilterDropdownSelection(ofxDatGuiDropdownEvent e) {
	for (Object* object : selection) {
		if (e.child == 0)
			object->magFilter = GL_NEAREST;
		if (e.child == 1)
			object->magFilter = GL_LINEAR;
	}
}

void Application3d::onTextureFilterDropdownSelection(ofxDatGuiDropdownEvent e) {
	float kernel[9];
	for (Object* object : selection) {
		if (e.child == 0) {
			object->filteredTexture = object->texture;
			return;
		}
		if (e.child == 1) {
			float gaussianBlurKernel[9] = 
					{ 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
					   1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0,
					   1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 };
			copy(begin(gaussianBlurKernel), end(gaussianBlurKernel), begin(kernel));
		}
		if (e.child == 2) {
			float sharpenKernel[9] = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };
			copy(begin(sharpenKernel), end(sharpenKernel), begin(kernel));
		}
		if (e.child == 3) {
			float edgeDetectKernel[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
			copy(begin(edgeDetectKernel), end(edgeDetectKernel), begin(kernel));
		}

		int width = object->texture.getWidth();
		int height = object->texture.getHeight();

		ofPixels pixels;
		object->texture.readToPixels(pixels);
		ofImage redImage, greenImage, blueImage;
		redImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
		greenImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
		blueImage.allocate(width, height, OF_IMAGE_GRAYSCALE);
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int index = y * width + x;
				redImage.getPixels()[index] = pixels[index * 3];
				greenImage.getPixels()[index] = pixels[index * 3 + 1];
				blueImage.getPixels()[index] = pixels[index * 3 + 2];
			}
		}
		redImage.update();
		greenImage.update();
		blueImage.update();

		ofImage filteredImage;
		filteredImage.allocate(width, height, OF_IMAGE_COLOR);
		for (int c = 0; c < 3; c++) {
			for (int y = 1; y < height - 1; y++) {
				for (int x = 1; x < width - 1; x++) {
					float sum = 0.0;
					for (int j = -1; j <= 1; j++) {
						for (int i = -1; i <= 1; i++) {
							int index = (y + j) * width + (x + i);
							sum += pixels[index * 3 + c] * kernel[(j + 1) * 3 + (i + 1)];
						}
					}
					int index = y * width + x;
					filteredImage.getPixels()[index * 3 + c] = ofClamp(sum, 0, 255);
				}
			}
		}
		filteredImage.update();
		object->filteredTexture.allocate(filteredImage);
	}
}

void Application3d::onToneMappingEvent(ofxDatGuiSliderEvent e) {
	for (Object* object : selection) {
		object->exposure = exposureSlider->getValue();
		object->gamma = gammaSlider->getValue();
	}
}

void Application3d::onMaterialColorChangeEvent(ofxDatGuiColorPickerEvent e) {
	for (Object* object : selection) {
		object->materialAmbiant = materialAmbiantCP->getColor();
		object->materialDiffuse = materialDiffuseCP->getColor();
		object->materialSpecular = materialSpecularCP->getColor();
	}
}

void Application3d::onCurveControlPointPositionChangeEvent(ofxDatGuiSliderEvent e) {
	if (!selection.empty()) {
		if (dynamic_cast<Curve*>(selection.at(0)) != nullptr) {
			Curve* curve = dynamic_cast<Curve*>(selection.at(0));
			int controlPointsIndex = curvePointControlDropdown->getSelected()->getIndex();
			curve->controlPoints[controlPointsIndex].x = curveXSlider->getValue();
			curve->controlPoints[controlPointsIndex].y = curveYSlider->getValue();
			curve->controlPoints[controlPointsIndex].z = curveZSlider->getValue();
		}
	}
}

void Application3d::onAddBezierCurveEvent(ofxDatGuiButtonEvent e) {
	Curve* curve= new Curve();
	std::string filename = "Bezier Curve";
	curve->originalName = filename;
	filename = getElementName(filename);
	curve->name = filename;
	addObject(curve, filename);
}

void Application3d::onAddCustomObjectEvent(ofxDatGuiButtonEvent e) {
	CustomObject* customObject = new CustomObject();
	std::string filename = "Custom Object";
	customObject->originalName = filename;
	filename = getElementName(filename);
	customObject->name = filename;
	addObject(customObject, filename);
}

void Application3d::onImportNormalMapEvent(ofxDatGuiButtonEvent e) {
	ofLog() << "<app::import normal map>";
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image");
	if (openFileResult.bSuccess) {
		for (Object* object : selection) {
			ofLoadImage(object->normalMap, openFileResult.filePath);
		}
		ofLog() << "<app::import - success>";
	}
	else {
		ofLog() << "<app::import - failed>";
	}
}

void Application3d::onAddBezierSurfaceEvent(ofxDatGuiButtonEvent e) {
	Surface* surface = new Surface();
	surface->bezierSurface->setup(250, 250, 3, 36);
	std::string filename = "Bezier Surface";
	surface->originalName = filename;
	filename = getElementName(filename);
	surface->name = filename;
	addObject(surface, filename);
}

void Application3d::onSurfaceControlPointPositionChangeEvent(ofxDatGuiSliderEvent e) {
	if (!selection.empty()) {
		if (dynamic_cast<Surface*>(selection.at(0)) != nullptr) {
			Surface* surface = dynamic_cast<Surface*>(selection.at(0));
			int controlPointsIndex = surfacePointControlDropdown->getSelected()->getIndex();
			surface->bezierSurface->modifyControlPoint(controlPointsIndex, 
				ofVec3f(surfaceXSlider->getValue(), surfaceYSlider->getValue(), surfaceZSlider->getValue()));
			surface->bezierSurface->update();
		}
	}
}

void Application3d::onSurfacePointControlSelectionEvent(ofxDatGuiDropdownEvent e) {
	if (!selection.empty()) {
		if (dynamic_cast<Surface*>(selection.at(0)) != nullptr) {
			Surface* surface = dynamic_cast<Surface*>(selection.at(0));
			int controlPointsIndex = surfacePointControlDropdown->getSelected()->getIndex();
			ofVec3f controlPoint = surface->bezierSurface->getControlPoint(controlPointsIndex);
			surfaceXSlider->setValue(controlPoint.x);
			surfaceYSlider->setValue(controlPoint.y);
			surfaceZSlider->setValue(controlPoint.z);
		}
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
	if (renderMode == "Texture")
		return Renderer3d::RenderMode::Texture;
	if (renderMode == "Lambert")
		return Renderer3d::RenderMode::Lambert;
	if (renderMode == "Phong")
		return Renderer3d::RenderMode::Phong;
	if (renderMode == "Blinn-Phong")
		return Renderer3d::RenderMode::Blinn_Phong;
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
