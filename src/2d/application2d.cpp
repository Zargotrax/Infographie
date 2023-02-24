#include "application2d.h"
#include "./ui/histogramComponent.h"

bool isExporting = false;

ofPoint position;
bool isDelete;
bool isDragging;
bool isRotate;
ofPoint dragStart = {0, 0, 0};
ofPoint originalImagePosition = { 0, 0, 0 };

ofxDatGui* headerGui;

ofxDatGui* toolsGui;

ofxDatGui* shapeGui;
ofxDatGuiSlider* widthSlider;
ofxDatGuiSlider* heightSlider;
ofxDatGuiSlider* apexCountSlider;
ofxDatGuiColorPicker* fillColorPicker;
ofxDatGuiColorPicker* outlineColorPicker;
ofxDatGuiToggle* outlineToggle;

ofxDatGuiScrollView* imgScrollView;
HistogramComponent* histogram;
ofxDatGuiSlider* redChannelSlider;
ofxDatGuiSlider* greenChannelSlider;
ofxDatGuiSlider* blueChannelSlider;
ofxDatGuiSlider* hueSlider;
ofxDatGuiSlider* saturationSlider;
ofxDatGuiSlider* brightnessSlider;
ofxDatGuiSlider* opacitySlider;

ofImage* basicCursor;
ofImage* moveCursor;
ofImage* exportingCursing;
ofImage* rotateCursor;
ofImage* deleteCursor;

void Application2d::setup(ofxDatGui* header)
{
    renderer.setup();

    headerGui = header;

    basicCursor = new ofImage();
    basicCursor->load("/cursors/basic-cursor.png");
    moveCursor = new ofImage();
    moveCursor->load("/cursors/move-cursor.png");
    exportingCursing = new ofImage();
    exportingCursing->load("/cursors/download-cursor.png");
    rotateCursor= new ofImage();
    rotateCursor->load("/cursors/rotate-cursor.png");
    deleteCursor = new ofImage();
    deleteCursor->load("/cursors/trash-cursor.png");

    toolsGui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    toolsGui->setVisible(false);
    toolsGui->setPosition(0, headerGui->getHeight() - 1);
    toolsGui->setWidth(256);

    ofxDatGuiButton* importBtn = toolsGui->addButton("Import");
    importBtn->onButtonEvent(this, &Application2d::onImportEvent);
    ofxDatGuiButton* exportBtn = toolsGui->addButton("Export");
    exportBtn->onButtonEvent(this, &Application2d::onExportEvent);

    ofxDatGuiColorPicker* bgColorPicker = toolsGui->addColorPicker("Background", ofColor::lightGray);
    bgColorPicker->onColorPickerEvent(this, &Application2d::onBgColorChangeEvent);

    ofxDatGuiFolder* rgbSliderFolder = toolsGui->addFolder("RGB", ofColor::black);
    redChannelSlider = rgbSliderFolder->addSlider("Red", 0, 255, 255);
    greenChannelSlider = rgbSliderFolder->addSlider("Green", 0, 255, 255);
    blueChannelSlider = rgbSliderFolder->addSlider("Blue", 0, 255, 255);

    redChannelSlider->onSliderEvent(this, &Application2d::onRgbSliderEvent);
    greenChannelSlider->onSliderEvent(this, &Application2d::onRgbSliderEvent);
    blueChannelSlider->onSliderEvent(this, &Application2d::onRgbSliderEvent);

    ofxDatGuiFolder* hsbSliderFolder = toolsGui->addFolder("HSB", ofColor::black);
    hueSlider = hsbSliderFolder->addSlider("Hue", 0, 255, 255);
    saturationSlider = hsbSliderFolder->addSlider("Saturation", 0, 255, 255);
    brightnessSlider = hsbSliderFolder->addSlider("Brightness", 0, 255, 255);

    hueSlider->onSliderEvent(this, &Application2d::onHsbSliderEvent);
    saturationSlider->onSliderEvent(this, &Application2d::onHsbSliderEvent);
    brightnessSlider->onSliderEvent(this, &Application2d::onHsbSliderEvent);

    opacitySlider = toolsGui->addSlider("Opacity", 0, 100, 100);
    opacitySlider->onSliderEvent(this, &Application2d::onOpacitySliderEvent);
    ofxDatGuiButton* moveLayerUpBtn = toolsGui->addButton("Move Layer Up");
    moveLayerUpBtn->onButtonEvent(this, &Application2d::onMoveLayerUpEvent);
    ofxDatGuiButton* moveLayerDownBtn = toolsGui->addButton("Move Layer Down");
    moveLayerDownBtn->onButtonEvent(this, &Application2d::onMoveLayerDownEvent);

    ofxDatGuiButton* rotateBtn = toolsGui->addButton("Rotate");
    rotateBtn->onButtonEvent(this, &Application2d::onRotateEvent);

    ofxDatGuiButton* removeBtn = toolsGui->addButton("Remove");
    removeBtn->onButtonEvent(this, &Application2d::onRemoveEvent);

    ofxDatGuiFolder* createFolder = toolsGui->addFolder("Create", ofColor::black);
    ofxDatGuiButton* addSquareBtn = createFolder->addButton("Add Square");
    addSquareBtn->onButtonEvent(this, &Application2d::onAddSquareEvent);
    ofxDatGuiButton* addRectangleBtn = createFolder->addButton("Add Rectangle");
    addRectangleBtn->onButtonEvent(this, &Application2d::onAddRectangleEvent);
    ofxDatGuiButton* addCircleBtn = createFolder->addButton("Add Circle");
    addCircleBtn->onButtonEvent(this, &Application2d::onAddCircleleEvent);
    ofxDatGuiButton* addEllipsisBtn = createFolder->addButton("Add Ellipsis");
    addEllipsisBtn->onButtonEvent(this, &Application2d::onAddEllipsisEvent);
    ofxDatGuiButton* addRegularPolygonBtn = createFolder->addButton("Add Regular Polygon");
    addRegularPolygonBtn->onButtonEvent(this, &Application2d::onAddRegularPolygonEvent);

    ofxDatGuiButton* addStarBtn= createFolder->addButton("Add Star");
    addStarBtn->onButtonEvent(this, &Application2d::onAddStarEvent);
    ofxDatGuiButton* addArrowButton = createFolder->addButton("Add Arrow");
    addArrowButton->onButtonEvent(this, &Application2d::onAddArrowEvent);

    histogram = new HistogramComponent("Histogram", ofColor::black);
    toolsGui->addFolder(histogram);

    imgScrollView = new ofxDatGuiScrollView("My scroll vierw", 100);
    imgScrollView->setWidth(255);
    imgScrollView->setPosition(ofGetWidth() - 255, header->getHeight() - 1);
    imgScrollView->onScrollViewEvent(this, &Application2d::onImageSelection);

    shapeGui = new ofxDatGui(300, 300);
    shapeGui->addLabel("Shape Menu");
    shapeGui->addHeader(":: Click here to drag ::");
    widthSlider = shapeGui->addSlider("Width", 0, 1000, 100);
    widthSlider->onSliderEvent(this, &Application2d::onUpdateShapeSliderEvent);
    heightSlider = shapeGui->addSlider("Height", 0, 1000, 100);
    heightSlider->onSliderEvent(this, &Application2d::onUpdateShapeSliderEvent);
    apexCountSlider = shapeGui->addSlider("Apex Count", 0, 10, 5);
    apexCountSlider->onSliderEvent(this, &Application2d::onUpdateShapeSliderEvent);
    fillColorPicker = shapeGui->addColorPicker("Fill", ofColor::lightGray);
    fillColorPicker->onColorPickerEvent(this, &Application2d::onUpdateShapeColorPickerEvent);
    outlineColorPicker = shapeGui->addColorPicker("Outline", ofColor::lightGray);
    outlineColorPicker->onColorPickerEvent(this, &Application2d::onUpdateShapeColorPickerEvent);
    outlineToggle = shapeGui->addToggle("Outline");
    outlineToggle->onToggleEvent(this, &Application2d::onUpdateShapeToggleEvent);

    renderer.offsetX1 = toolsGui->getWidth();
    renderer.offsetY1 = headerGui->getHeight() - 1;
    renderer.offsetX2 = ofGetWidth() - imgScrollView->getWidth();
    renderer.offsetY2 = ofGetHeight();
}

void Application2d::draw()
{
    shapeGui->setVisible(false);

    renderer.draw();

    if (isExporting && ofGetFrameNum() % 2 == 0) {
        ofLog() << "exporting";
        ofImage img;
        img.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
        string filename = "/export/" + to_string(ofGetFrameNum()) + ".png";
        img.save(filename);
    }

    imgScrollView->draw();

    float x = static_cast<float>(ofGetMouseX());
    float y = static_cast<float>(ofGetMouseY());

    if (isDelete && renderer.hitTest(x, y) && renderer.active != nullptr) {
        ofHideCursor();
        deleteCursor->draw(x - (deleteCursor->getWidth() / 2), y - (deleteCursor->getWidth() / 2));
    } else if (isRotate && renderer.hitTest(x, y) && renderer.active != nullptr) {
        ofHideCursor();
        rotateCursor->draw(x - (rotateCursor->getWidth() / 2), y - (moveCursor->getWidth() / 2));
    }
    else if (isDragging && renderer.active != nullptr) {
        ofHideCursor();
        moveCursor->draw(x - (moveCursor->getWidth() / 2), y - (moveCursor->getWidth() / 2));
    } else if (isExporting) {
        ofHideCursor();
        exportingCursing->draw(x, y);
    } else if (!guiHitTest(x, y)) {
        ofHideCursor();
        basicCursor->draw(x, y);
    } else {
        ofShowCursor();
    }

    if (dynamic_cast<Shape*>(renderer.active) != nullptr)
        shapeGui->setVisible(true);
}

void Application2d::update() {
    imgScrollView->update();
    if (isDragging && renderer.active != nullptr) {
        renderer.active->coordinates.x = ofGetMouseX() - dragStart.x + originalImagePosition.x;
        renderer.active->coordinates.y = ofGetMouseY() - dragStart.y + originalImagePosition.y;
    }
}

void Application2d::showUi() {
    toolsGui->setVisible(true);
    shapeGui->setVisible(true);
}

void Application2d::hideUi() {
    toolsGui->setVisible(false);
    shapeGui->setVisible(false);
}

void Application2d::windowResized(int w, int h)
{
    renderer.offsetX2 = ofGetWidth() - imgScrollView->getWidth();
    renderer.offsetY2 = ofGetHeight();
    imgScrollView->setPosition(ofGetWidth() - 255, headerGui->getHeight() - 1);
}

void Application2d::dragEvent(ofDragInfo dragInfo)
{
    ofLog() << "<app::ofDragInfo file[0]: " << dragInfo.files.at(0)
        << " at : " << dragInfo.position << ">";

    import(dragInfo.files.at(0));
}

void Application2d::keyPressed(int key)
{
    if (key == OF_KEY_ALT) {
        isDelete = true;
    } else if (key == OF_KEY_SHIFT) {
        isRotate = true;
    } else if (key == 48) {
        isExporting = true;
    }
}

void Application2d::keyReleased(int key)
{
    if (key == OF_KEY_ALT) {
        isDelete = false;
    } else if (key == OF_KEY_SHIFT) {
        isRotate = false;
    } else if (key == 48) {
        isExporting = false;
    }
}

void Application2d::mousePressed(int x, int y, int button)
{
    ofLog() << "<app::mouse pressed at: (" << x << ", " << y << ")>";

    if (renderer.active != nullptr && renderer.hitTest(x, y) && !guiHitTest(x, y)) {
        if (isDelete) {
            removeCurrentSelectedObject();
        } else if (isRotate) {
            renderer.active->rotate90();
        }
        else {
            isDragging = true;
            dragStart = { static_cast<float>(x), static_cast<float>(y), 0 };
            originalImagePosition = { renderer.active->coordinates.x, renderer.active->coordinates.y, 0 };
        }
    }
}

void Application2d::mouseReleased(int x, int y, int button)
{
    ofLog() << "<app::mouse released at: (" << x << ", " << y << ")>";
    isDragging = false;
}

void Application2d::onImportEvent(ofxDatGuiButtonEvent e) {
    ofLog() << "<app::import>";
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image");
    if (openFileResult.bSuccess) {
        import(openFileResult.getPath());
        ofLog() << "<app::import - success>";
    }
    else {
        ofLog() << "<app::import - failed>";
    }
}

void Application2d::onExportEvent(ofxDatGuiButtonEvent e) 
{
    ofLog() << "<app::export>";
    ofImage image;
    image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    image.save("render.png");
}

void Application2d::onImageSelection(ofxDatGuiScrollViewEvent e) {
    ofLog() << "<app::selecting image : at index " << e.target->getIndex() << ">";
    renderer.active = renderer.elements.at(e.target->getIndex());
    renderer.activeIndex = e.target->getIndex();
    opacitySlider->setValue(renderer.active->opacity);
    calculateHistogramData();
    updateUiFromShape();
}

void Application2d::onRgbSliderEvent(ofxDatGuiSliderEvent e)
{
    renderer.active->color = ofColor::ofColor_(redChannelSlider->getValue(), greenChannelSlider->getValue(), blueChannelSlider->getValue());
    hueSlider->setValue(renderer.active->color.getHue(), false);
    saturationSlider->setValue(renderer.active->color.getSaturation(), false);
    brightnessSlider->setValue(renderer.active->color.getBrightness(), false);
};

void Application2d::onHsbSliderEvent(ofxDatGuiSliderEvent e)
{
    renderer.active->color = ofColor::fromHsb(hueSlider->getValue(), saturationSlider->getValue(), brightnessSlider->getValue());
    redChannelSlider->setValue(renderer.active->color.r, false);
    greenChannelSlider->setValue(renderer.active->color.g, false);
    blueChannelSlider->setValue(renderer.active->color.b, false);
};

void Application2d::onOpacitySliderEvent(ofxDatGuiSliderEvent e) {
    if (renderer.active != nullptr)
        renderer.active->opacity = opacitySlider->getValue() * 2.55;
}

void Application2d::onRotateEvent(ofxDatGuiButtonEvent e) {
    if (renderer.active != nullptr)
        renderer.active->rotate90();
}

void Application2d::onRemoveEvent(ofxDatGuiButtonEvent e) {
    removeCurrentSelectedObject();
}

void Application2d::onAddSquareEvent(ofxDatGuiButtonEvent e) {
    Square* square = new Square();
    string filename = "square";
    square->originalName = filename;
    filename = getElementName(filename);
    square->name = filename;

    imgScrollView->add(filename);

    addElementToRenderer(square);
    updateUiFromShape();
}

void Application2d::onAddRectangleEvent(ofxDatGuiButtonEvent e) {
    RectangleShape* rectangle = new RectangleShape();
    string filename = "rectangle";
    rectangle->originalName = filename;
    filename = getElementName(filename);
    rectangle->name = filename;

    imgScrollView->add(filename);

    addElementToRenderer(rectangle);
    updateUiFromShape();
}

void Application2d::onAddCircleleEvent(ofxDatGuiButtonEvent e) {
    Circle* circle = new Circle();
    string filename = "circle";
    circle->originalName = filename;
    filename = getElementName(filename);
    circle->name = filename;

    imgScrollView->add(filename);

    addElementToRenderer(circle);
    updateUiFromShape();
}

void Application2d::onAddEllipsisEvent(ofxDatGuiButtonEvent e) {
    Ellipsis* ellipsis = new Ellipsis();
    string filename = "ellipsis";
    ellipsis->originalName = filename;
    filename = getElementName(filename);
    ellipsis->name = filename;

    imgScrollView->add(filename);

    addElementToRenderer(ellipsis);
    updateUiFromShape();
}

void Application2d::onAddRegularPolygonEvent(ofxDatGuiButtonEvent e) {
    RegularPolygon* regularPolygon = new RegularPolygon();
    string filename = "regular polygon";
    regularPolygon->originalName = filename;
    filename = getElementName(filename);
    regularPolygon->name = filename;

    imgScrollView->add(filename);

    addElementToRenderer(regularPolygon);
    updateUiFromShape();
}

void Application2d::onAddStarEvent(ofxDatGuiButtonEvent e) {
    Star* star = new Star();
    string filename = "star";
    star->originalName = filename;
    filename = getElementName(filename);
    star->name = filename;

    imgScrollView->add(filename);

    addElementToRenderer(star);
    updateUiFromShape();
}

void Application2d::onAddArrowEvent(ofxDatGuiButtonEvent e) {
    Arrow* arrow = new Arrow();
    string filename = "arrow";
    arrow->originalName = filename;
    filename = getElementName(filename);
    arrow->name = filename;

    imgScrollView->add(filename);

    addElementToRenderer(arrow);
    updateUiFromShape();
}

void Application2d::onUpdateShapeSliderEvent(ofxDatGuiSliderEvent e) {
    updateShapeFromUi();
}

void Application2d::onUpdateShapeColorPickerEvent(ofxDatGuiColorPickerEvent e) {
    updateShapeFromUi();
}

void Application2d::onUpdateShapeToggleEvent(ofxDatGuiToggleEvent e) {
    updateShapeFromUi();
}

void Application2d::onBgColorChangeEvent(ofxDatGuiColorPickerEvent e) {
    ofSetBackgroundColor(e.color);
}

void Application2d::onMoveLayerUpEvent(ofxDatGuiButtonEvent e) {
    if (renderer.active != nullptr) {
        if (renderer.activeIndex != 0) {
            imgScrollView->swap(renderer.activeIndex, renderer.activeIndex - 1);
            swap(renderer.elements[renderer.activeIndex], renderer.elements[renderer.activeIndex - 1]);
        }
    }
}

void Application2d::onMoveLayerDownEvent(ofxDatGuiButtonEvent e) {
    if (renderer.active != nullptr) {
        if (renderer.activeIndex != renderer.elements.size() - 1) {
            imgScrollView->swap(renderer.activeIndex, renderer.activeIndex + 1);
            swap(renderer.elements[renderer.activeIndex], renderer.elements[renderer.activeIndex + 1]);
        }
    }
}

void Application2d::import(string path) {
    ofImage imageData;
    imageData.load(path);
    if (imageData.isAllocated()) {
        string filename;
        string::size_type idx = path.rfind('\\');
        if (idx != string::npos) {
            filename = path.substr(idx + 1);
        }

        Image* image = new Image();
        image->imageData = imageData;
        image->originalName = filename;
        image->path = path;
        filename = getElementName(filename);
        image->name = filename;

        imgScrollView->add(filename);

        renderer.elements.push_back(image);
        renderer.active = image;
        renderer.activeIndex++;
        calculateHistogramData();
    }
    else {
        ofLog() << "echec de l'import d'image";
    }
}

void Application2d::calculateHistogramData()
{
    ofLog() << "<app::calculateHistogramData>";

    int redHist[256] = { 0 };
    int greenHist[256] = { 0 };
    int blueHist[256] = { 0 };

    //Applies only to images
    if (dynamic_cast<Image*>(renderer.active) != nullptr) {
        Image img = *dynamic_cast<Image*>(renderer.active);
        ofPixels pixels = img.imageData.getPixels();

        int width = img.imageData.getWidth();
        int height = img.imageData.getHeight();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                ofColor color = pixels.getColor(x, y);
                redHist[color.r]++;
                greenHist[color.g]++;
                blueHist[color.b]++;
            }
        }
    }

    histogram->setRedHist(redHist);
    histogram->setGreenHist(greenHist);
    histogram->setBlueHist(blueHist);
}

string Application2d::getElementName(string filename) {
    int duplicate = 0;
    for (TwoDimensionalObject* element : renderer.elements) {
        if (element->originalName == filename) {
            duplicate++;
        }
    }

    if (duplicate > 0) {
        filename = filename + " (" + to_string(duplicate) + ")";
    }
    return filename;
}

bool Application2d::guiHitTest(int x, int y) {
    if (shapeGui->getVisible()) {
        return (x > shapeGui->getPosition().x && x < shapeGui->getPosition().x + shapeGui->getWidth() &&
            y > shapeGui->getPosition().y && y < shapeGui->getPosition().y + shapeGui->getHeight()) ||
            (y > headerGui->getPosition().y && y < headerGui->getPosition().y + headerGui->getHeight()) ||
            (x > toolsGui->getPosition().x && x < toolsGui->getPosition().x + toolsGui->getWidth() &&
                y > toolsGui->getPosition().y && y < toolsGui->getPosition().y + toolsGui->getHeight());
    }
    else {
        return (y > headerGui->getPosition().y && y < headerGui->getPosition().y + headerGui->getHeight()) ||
            (x > toolsGui->getPosition().x && x < toolsGui->getPosition().x + toolsGui->getWidth() &&
                y > toolsGui->getPosition().y && y < toolsGui->getPosition().y + toolsGui->getHeight());
    }
}

void Application2d::addElementToRenderer(TwoDimensionalObject* object) {
    renderer.elements.push_back(object);
    renderer.active = object;
    renderer.activeIndex++;
    calculateHistogramData();
}

void Application2d::removeCurrentSelectedObject() {
    if (renderer.active != nullptr) {
        renderer.elements.erase(renderer.elements.begin() + renderer.activeIndex);
        imgScrollView->remove(renderer.activeIndex);
        if (renderer.elements.size() > 0) {
            renderer.active = renderer.elements.at(0);
            renderer.activeIndex = 0;
        }
        else {
            renderer.active = nullptr;
            renderer.activeIndex = -1;
        }
    }
}

void Application2d::updateShapeFromUi() {
    //Applies only to shapes
    if (dynamic_cast<Shape*>(renderer.active) != nullptr) {
        Shape* shape = dynamic_cast<Shape*>(renderer.active);
        shape->width = widthSlider->getValue();
        shape->height = heightSlider->getValue();
        shape->nbApex = apexCountSlider->getValue();
        shape->fillColor = fillColorPicker->getColor();
        shape->outlineColor = outlineColorPicker->getColor();
        shape->outline = outlineToggle->getChecked();
    }
}

void Application2d::updateUiFromShape() {
    //Applies only to shapes
    if (dynamic_cast<Shape*>(renderer.active) != nullptr) {
        Shape* shape = dynamic_cast<Shape*>(renderer.active);
        widthSlider->setValue(shape->width);
        heightSlider->setValue(shape->height);
        apexCountSlider->setValue(shape->nbApex);
        fillColorPicker->setColor(shape->fillColor);
        outlineColorPicker->setColor(shape->outlineColor);
        outlineToggle->setChecked(shape->outline);
    }
}
