#include "application.h"
#include "./ui/histogramComponent.h"

ofPoint position;
bool isDragging;
ofPoint dragStart = {0, 0, 0};
ofPoint originalImagePosition = { 0, 0, 0 };

ofxDatGui* filesGui;
ofxDatGuiFolder* filesFolder;
ofxDatGui* toolsGui;
ofxDatGui* headerGui;
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

// Gui setup /////////////////////////////////////////////////////////////////////////////////////////////////
void Application::setup()
{
    ofLog() << "<app::setup>";

    ofSetWindowTitle("Projet Infographie");

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

    histogram = new HistogramComponent("Histogram", ofColor::black);
    toolsGui->addFolder(histogram);

    imgScrollView = new ofxDatGuiScrollView("My scroll vierw", 100);
    imgScrollView->setWidth(255);
    imgScrollView->setPosition(ofGetWidth() - 255, headerGui->getHeight() - 1);
    imgScrollView->setHeight(ofGetHeight());
    imgScrollView->onScrollViewEvent(this, &Application::onImageSelection);

    renderer.offsetX1 = toolsGui->getWidth();
    renderer.offsetY1 = headerGui->getHeight();
    renderer.offsetX2 = ofGetWidth() - 225;
    renderer.offsetY2 = ofGetHeight();
    renderer.setup();
}

void Application::draw()
{
    renderer.draw();
    imgScrollView->draw();
}

void Application::update() {
    imgScrollView->update();
    if (isDragging && renderer.activeImage != nullptr) {
        renderer.activeImage->coordinates.x = ofGetMouseX() - dragStart.x + originalImagePosition.x;
        renderer.activeImage->coordinates.y = ofGetMouseY() - dragStart.y + originalImagePosition.y;
    }
}

void Application::windowResized(int w, int h)
{
    ofLog() << "<app::windowResized to: (" << w << ", " << h << ")>";
    renderer.offsetX2 = ofGetWidth() - 225;
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

void Application::keyReleased(int key)
{
  ofLog() << "<app::keyReleased: " << key << ">";
}

void Application::mousePressed(int x, int y, int button)
{
    ofLog() << "<app::mouse pressed at: (" << x << ", " << y << ")>";
    ofLog() << "<app::dragging is true" << renderer.offsetX1 << ":" << renderer.offsetX2 << ":" << renderer.offsetY1 << ";" << renderer.offsetY2;

    if (renderer.activeImage != nullptr) {
        if (x > renderer.offsetX1 && x < renderer.offsetX2 && y > renderer.offsetY1 && y < renderer.offsetY2) {
            isDragging = true;
            dragStart = { static_cast<float>(x), static_cast<float>(y), 0 };
            originalImagePosition = { renderer.activeImage->coordinates.x, renderer.activeImage->coordinates.y, 0 };
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
    renderer.activeImage = renderer.images.at(e.target->getIndex());
    renderer.activeImageIndex = e.target->getIndex();
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
    if (renderer.activeImage != nullptr)
        renderer.activeImage->opacity = opacitySlider->getValue() * 2.55;
}

void Application::onRotateEvent(ofxDatGuiButtonEvent e) {
    if (renderer.activeImage != nullptr)
        renderer.activeImage->imageData.rotate90(1);
}

void Application::onRemoveEvent(ofxDatGuiButtonEvent e) {
    if (renderer.activeImage != nullptr) {
        renderer.images.erase(renderer.images.begin() + renderer.activeImageIndex);
        imgScrollView->remove(renderer.activeImageIndex);
        if (renderer.images.size() > 0) {
            renderer.activeImage = renderer.images.at(0);
            renderer.activeImageIndex = 0;
        }
        else {
            renderer.activeImage = nullptr;
            renderer.activeImageIndex = -1;
        }
    }
}

// Privates utility methods /////////////////////////////////////////////////////////////////////////////////////////////////

void Application::import(string path) {
    ofImage imageData;
    imageData.load(path);

    std::string fileName;
    std::string::size_type idx = path.rfind('\\');
    if (idx != std::string::npos) {
        fileName = path.substr(idx + 1);
    }

    Image* image = new Image();
    image->imageData = imageData;
    image->originalName = fileName;
    image->path = path;

    int duplicate = 0;
    for (Image* image : renderer.images) {
        if (image->originalName == fileName) {
            duplicate++;
        }
    }

    if (duplicate > 0) {
        fileName = fileName + " (" + std::to_string(duplicate) + ")";
    }

    imgScrollView->add(fileName);

    image->name = fileName;

    renderer.images.push_back(image);
    renderer.activeImage = image;
    renderer.activeImageIndex++;
    calculateHistogramData();
}

void Application::calculateHistogramData()
{
    ofLog() << "<app::calculateHistogramData>";

    ofImage img = renderer.activeImage->imageData;
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