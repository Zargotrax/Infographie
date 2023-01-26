#include "ofApp.h"
#include "./ui/histogramComponent.h"

ofPoint position;
bool isDragging;
bool isRotate;
ofPoint dragStart = {0, 0, 0};
ofPoint originalImagePosition = { 0, 0, 0 };

ofxDatGui* filesGui;
ofxDatGuiFolder* filesFolder;
ofxDatGui* toolsGui;
ofxDatGui* headerGui;

ofxDatGui* shapeGui;
ofxDatGuiSlider* widthSlider;
ofxDatGuiSlider* heightSlider;
ofxDatGuiSlider* apexCountSlider;

ofxDatGuiLabel* headerLabel;
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
ofImage* sliderCursor;
ofImage* rotateCursor;

// Gui setup /////////////////////////////////////////////////////////////////////////////////////////////////
void Application::setup()
{
    ofLog() << "<app::setup>";

    ofSetWindowTitle("Projet Infographie");
    ofSetCircleResolution(50);
    ofHideCursor();

    basicCursor = new ofImage();
    basicCursor->load("/cursors/basic-cursor.png");
    moveCursor = new ofImage();
    moveCursor->load("/cursors/move-cursor.png");
    sliderCursor = new ofImage();
    sliderCursor->load("/cursors/slider-cursor.png");
    rotateCursor= new ofImage();
    rotateCursor->load("/cursors/rotate-cursor.png");

    headerGui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    headerLabel = headerGui->addLabel("Projet Infographie");
    headerLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    headerLabel->setWidth(ofGetWidth());

    filesGui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    filesGui->setWidth(60);

    filesFolder = filesGui->addFolder("Files", ofColor::black);
    ofxDatGuiButton* importBtn = filesFolder->addButton("Import");
    importBtn->setBackgroundColor(ofColor::darkGrey);
    ofxDatGuiButton* exportBtn = filesFolder->addButton("Export");
    exportBtn->setBackgroundColor(ofColor::darkGrey);

    importBtn->onButtonEvent(this, &Application::onImportEvent);
    exportBtn->onButtonEvent(this, &Application::onExportEvent);

    toolsGui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    toolsGui->setPosition(0, headerGui->getHeight()-1);
    toolsGui->setWidth(256);

    ofxDatGuiFolder* rgbSliderFolder = toolsGui->addFolder("RGB", ofColor::black);
    redChannelSlider = rgbSliderFolder->addSlider("Red", 0, 255, 255);
    greenChannelSlider = rgbSliderFolder->addSlider("Green", 0, 255, 255);
    blueChannelSlider = rgbSliderFolder->addSlider("Blue", 0, 255, 255);

    redChannelSlider->onSliderEvent(this, &Application::onRgbSliderEvent);
    greenChannelSlider->onSliderEvent(this, &Application::onRgbSliderEvent);
    blueChannelSlider->onSliderEvent(this, &Application::onRgbSliderEvent);

    ofxDatGuiFolder* hsbSliderFolder = toolsGui->addFolder("HSB", ofColor::black);
    hueSlider = hsbSliderFolder->addSlider("Hue", 0, 360, 360);
    saturationSlider = hsbSliderFolder->addSlider("Saturation", 0, 100, 100);
    brightnessSlider = hsbSliderFolder->addSlider("Brightness", 0, 100, 100);

    hueSlider->onSliderEvent(this, &Application::onHsbSliderEvent);
    saturationSlider->onSliderEvent(this, &Application::onHsbSliderEvent);
    brightnessSlider->onSliderEvent(this, &Application::onHsbSliderEvent);

    opacitySlider = toolsGui->addSlider("Opacity", 0, 100, 100);
    opacitySlider->onSliderEvent(this, &Application::onOpacitySliderEvent);

    ofxDatGuiButton* rotateBtn = toolsGui->addButton("Rotate");
    rotateBtn->onButtonEvent(this, &Application::onRotateEvent);

    ofxDatGuiButton* removeBtn = toolsGui->addButton("Remove");
    removeBtn->onButtonEvent(this, &Application::onRemoveEvent);

    ofxDatGuiFolder* createFolder = toolsGui->addFolder("Create", ofColor::black);
    ofxDatGuiButton* addSquareBtn = createFolder->addButton("Add Square");
    addSquareBtn->onButtonEvent(this, &Application::onAddSquareEvent);
    ofxDatGuiButton* addRectangleBtn = createFolder->addButton("Add Rectangle");
    addRectangleBtn->onButtonEvent(this, &Application::onAddRectangleEvent);
    ofxDatGuiButton* addCircleBtn = createFolder->addButton("Add Circle");
    addCircleBtn->onButtonEvent(this, &Application::onAddCircleleEvent);
    ofxDatGuiButton* addEllipsisBtn = createFolder->addButton("Add Ellipsis");
    addEllipsisBtn->onButtonEvent(this, &Application::onAddEllipsisEvent);
    ofxDatGuiButton* addRegularPolygonBtn = createFolder->addButton("Add Regular Polygon");
    addRegularPolygonBtn->onButtonEvent(this, &Application::onAddRegularPolygonEvent);

    ofxDatGuiButton* addStarBtn= createFolder->addButton("Add Star");
    addStarBtn->onButtonEvent(this, &Application::onAddStarEvent);
    ofxDatGuiButton* addAmongusBtn = createFolder->addButton("Add Star");
    addAmongusBtn->onButtonEvent(this, &Application::onAddAmongusEvent);

    histogram = new HistogramComponent("Histogram", ofColor::black);
    toolsGui->addFolder(histogram);

    imgScrollView = new ofxDatGuiScrollView("My scroll vierw", 100);
    imgScrollView->setWidth(255);
    imgScrollView->setPosition(ofGetWidth() - 255, headerGui->getHeight() - 1);
    imgScrollView->setHeight(ofGetHeight());
    imgScrollView->onScrollViewEvent(this, &Application::onImageSelection);

    //ShapeGui* shapeGui = new ShapeGui();
    shapeGui = new ofxDatGui(300, 300);
    shapeGui->addLabel("Shape Menu");
    shapeGui->addHeader(":: Click here to drag ::");
    widthSlider = shapeGui->addSlider("Width", 0, 1000, 100);
    widthSlider->onSliderEvent(this, &Application::onUpdateShape);
    heightSlider = shapeGui->addSlider("Height", 0, 1000, 100);
    heightSlider->onSliderEvent(this, &Application::onUpdateShape);
    apexCountSlider = shapeGui->addSlider("Apex Count", 0, 10, 5);
    apexCountSlider->onSliderEvent(this, &Application::onUpdateShape);

    renderer.offsetX1 = toolsGui->getWidth();
    renderer.offsetY1 = headerGui->getHeight();
    renderer.offsetX2 = ofGetWidth() - imgScrollView->getWidth();
    renderer.offsetY2 = ofGetHeight();
    renderer.setup();
}

void Application::draw()
{
    shapeGui->setVisible(false);

    renderer.draw();
    imgScrollView->draw();

    float x = static_cast<float>(ofGetMouseX());
    float y = static_cast<float>(ofGetMouseY());

    if (isRotate && renderer.active != nullptr && renderer.hitTest(x, y)) {
        rotateCursor->draw(x - (rotateCursor->getWidth() / 2), y - (moveCursor->getWidth() / 2));
    } else if (isDragging && renderer.active != nullptr) {
        moveCursor->draw(x - (moveCursor->getWidth() / 2), y - (moveCursor->getWidth() / 2));
    }
    else {
        basicCursor->draw(x - (basicCursor->getWidth() / 2), y - (basicCursor->getWidth() / 2));
    }

    if (dynamic_cast<Shape*>(renderer.active) != nullptr)
        shapeGui->setVisible(true);
}

void Application::update() {
    imgScrollView->update();
    if (isDragging && renderer.active != nullptr) {
        renderer.active->coordinates.x = ofGetMouseX() - dragStart.x + originalImagePosition.x;
        renderer.active->coordinates.y = ofGetMouseY() - dragStart.y + originalImagePosition.y;
    }
}

void Application::windowResized(int w, int h)
{
    ofLog() << "<app::windowResized to: (" << w << ", " << h << ")>";
    renderer.offsetX2 = ofGetWidth() - imgScrollView->getWidth();
    renderer.offsetY2 = ofGetHeight();
    headerLabel->setWidth(ofGetWidth());
    imgScrollView->setPosition(ofGetWidth() - 255, headerGui->getHeight() - 1);
    imgScrollView->setHeight(ofGetHeight());
}

void Application::dragEvent(ofDragInfo dragInfo)
{
    ofLog() << "<app::ofDragInfo file[0]: " << dragInfo.files.at(0)
        << " at : " << dragInfo.position << ">";

    import(dragInfo.files.at(0));
}

void Application::keyPressed(int key)
{
    ofLog() << "<app::keyReleased: " << key << ">";
    if (OF_KEY_SHIFT)
        isRotate = true;
}

void Application::keyReleased(int key)
{
  ofLog() << "<app::keyReleased: " << key << ">";
  if (OF_KEY_SHIFT)
      isRotate = false;
}

void Application::mousePressed(int x, int y, int button)
{
    ofLog() << "<app::mouse pressed at: (" << x << ", " << y << ")>";

    if (renderer.active != nullptr && renderer.hitTest(x, y) && !guiHitTest(x, y)) {
        if (isRotate) {
            renderer.active->rotate90();
        }
        else {
            isDragging = true;
            dragStart = { static_cast<float>(x), static_cast<float>(y), 0 };
            originalImagePosition = { renderer.active->coordinates.x, renderer.active->coordinates.y, 0 };
        }
    }
}

void Application::mouseReleased(int x, int y, int button)
{
    ofLog() << "<app::mouse released at: (" << x << ", " << y << ")>";
    isDragging = false;
}

void Application::exit()
{
  ofLog() << "<app::exit>";
}


// Application events /////////////////////////////////////////////////////////////////////////////////////////////////
void Application::onImportEvent(ofxDatGuiButtonEvent e) {
    ofLog() << "<app::import>";
    filesFolder->collapse();
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image");
    if (openFileResult.bSuccess) {
        import(openFileResult.getPath());
       ofLog() << "<app::import - success>";
    }
    else {
        ofLog() << "<app::import - failed>";
    }
}

void Application::onExportEvent(ofxDatGuiButtonEvent e) 
{
    ofLog() << "<app::export>";
    filesFolder->collapse();
    ofImage image;
    image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    image.save("render.png");
}

void Application::onImageSelection(ofxDatGuiScrollViewEvent e) {
    ofLog() << "<app::selecting image : at index " << e.target->getIndex() << ">";
    renderer.active = renderer.elements.at(e.target->getIndex());
    renderer.activeIndex = e.target->getIndex();
    calculateHistogramData();
}

void Application::onRgbSliderEvent(ofxDatGuiSliderEvent e)
{
    renderer.color = ofColor::ofColor_(redChannelSlider->getValue(), greenChannelSlider->getValue(), blueChannelSlider->getValue());
    hueSlider->setValue(renderer.color.getHue());
    saturationSlider->setValue(renderer.color.getSaturation());
    brightnessSlider->setValue(renderer.color.getBrightness());
};

void Application::onHsbSliderEvent(ofxDatGuiSliderEvent e)
{
    renderer.color = ofColor::fromHsb(hueSlider->getValue(), saturationSlider->getValue(), brightnessSlider->getValue());
    redChannelSlider->setValue(renderer.color.r);
    greenChannelSlider->setValue(renderer.color.g);
    blueChannelSlider->setValue(renderer.color.b);
};

void Application::onOpacitySliderEvent(ofxDatGuiSliderEvent e) {
    if (renderer.active != nullptr)
        renderer.active->opacity = opacitySlider->getValue() * 2.55;
}

void Application::onRotateEvent(ofxDatGuiButtonEvent e) {
    if (renderer.active != nullptr)
        renderer.active->rotate90();
}

void Application::onRemoveEvent(ofxDatGuiButtonEvent e) {
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

void Application::onAddSquareEvent(ofxDatGuiButtonEvent e) {
    Square* square = new Square();
    string filename = "square";
    square->originalName = filename;
    filename = getElementName(filename);
    square->name = filename;

    imgScrollView->add(filename);

    renderer.elements.push_back(square);
    renderer.active = square;
    renderer.activeIndex++;
}

void Application::onAddRectangleEvent(ofxDatGuiButtonEvent e) {
    RectangleShape* rectangle = new RectangleShape();
    string filename = "rectangle";
    rectangle->originalName = filename;
    filename = getElementName(filename);
    rectangle->name = filename;

    imgScrollView->add(filename);

    renderer.elements.push_back(rectangle);
    renderer.active = rectangle;
    renderer.activeIndex++;
}

void Application::onAddCircleleEvent(ofxDatGuiButtonEvent e) {
    Circle* circle = new Circle();
    string filename = "circle";
    circle->originalName = filename;
    filename = getElementName(filename);
    circle->name = filename;

    imgScrollView->add(filename);

    renderer.elements.push_back(circle);
    renderer.active = circle;
    renderer.activeIndex++;
}

void Application::onAddEllipsisEvent(ofxDatGuiButtonEvent e) {
    Ellipsis* ellipsis = new Ellipsis();
    string filename = "ellipsis";
    ellipsis->originalName = filename;
    filename = getElementName(filename);
    ellipsis->name = filename;

    imgScrollView->add(filename);

    renderer.elements.push_back(ellipsis);
    renderer.active = ellipsis;
    renderer.activeIndex++;
}

void Application::onAddRegularPolygonEvent(ofxDatGuiButtonEvent e) {
    RegularPolygon* regularPolygon = new RegularPolygon();
    string filename = "regular polygon";
    regularPolygon->originalName = filename;
    filename = getElementName(filename);
    regularPolygon->name = filename;

    imgScrollView->add(filename);

    renderer.elements.push_back(regularPolygon);
    renderer.active = regularPolygon;
    renderer.activeIndex++;
}

void Application::onAddStarEvent(ofxDatGuiButtonEvent e) {
    Star* star = new Star();
    string filename = "star";
    star->originalName = filename;
    filename = getElementName(filename);
    star->name = filename;

    imgScrollView->add(filename);

    renderer.elements.push_back(star);
    renderer.active = star;
    renderer.activeIndex++;
}

void Application::onAddAmongusEvent(ofxDatGuiButtonEvent e) {
    //TODO
}

void Application::onUpdateShape(ofxDatGuiSliderEvent e) {
    //Applies only to shapes
    if (dynamic_cast<Shape*>(renderer.active) != nullptr) {
        Shape* shape = dynamic_cast<Shape*>(renderer.active);
        shape->width = widthSlider->getValue();
        shape->height = heightSlider->getValue();
        shape->nbApex = apexCountSlider->getValue();
    }
}


// Privates utility methods /////////////////////////////////////////////////////////////////////////////////////////////////

void Application::import(string path) {
    ofImage imageData;
    imageData.load(path);

    std::string filename;
    std::string::size_type idx = path.rfind('\\');
    if (idx != std::string::npos) {
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

void Application::calculateHistogramData()
{
    ofLog() << "<app::calculateHistogramData>";

    //Applies only to images
    if (dynamic_cast<ofImage*>(renderer.active) != nullptr) {
        ofImage img = *dynamic_cast<ofImage*>(renderer.active);
        ofPixels pixels = img.getPixels();

        int width = img.getWidth();
        int height = img.getHeight();

        int redHist[256] = { 0 };
        int greenHist[256] = { 0 };
        int blueHist[256] = { 0 };

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                ofColor color = pixels.getColor(x, y);
                redHist[color.r]++;
                greenHist[color.g]++;
                blueHist[color.b]++;
            }
        }

        histogram->setRedHist(redHist);
        histogram->setGreenHist(greenHist);
        histogram->setBlueHist(blueHist);
    }
}

string Application::getElementName(string filename) {
    int duplicate = 0;
    for (TwoDimensionalObject* element : renderer.elements) {
        if (element->originalName == filename) {
            duplicate++;
        }
    }

    if (duplicate > 0) {
        filename = filename + " (" + std::to_string(duplicate) + ")";
    }
    return filename;
}

bool Application::guiHitTest(int x, int y) {
    //TODO add all gui elements
    return x > shapeGui->getPosition().x && x < shapeGui->getPosition().x + shapeGui->getWidth() &&
        y > shapeGui->getPosition().y && y < shapeGui->getPosition().y + shapeGui->getHeight();
}
