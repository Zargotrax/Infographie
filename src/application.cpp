#include "application.h"
#include "histogramComponent.h"

ofPoint position;
bool isDragging;

ofxDatGui* filesGui;
ofxDatGuiFolder* filesFolder;
ofxDatGui* toolsGui;
ofxDatGui* headerGui;
ofxDatGuiLabel* headerLabel;
ofxDatGui* imagesGui;
HistogramComponent* histogram;
ofxDatGuiSlider* redChannelSlider;
ofxDatGuiSlider* greenChannelSlider;
ofxDatGuiSlider* blueChannelSlider;
ofxDatGuiSlider* hueSlider;
ofxDatGuiSlider* saturationSlider;
ofxDatGuiSlider* brightnessSlider;
ofxDatGuiSlider* opacitySlider;

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

    auto rgbColorUpdate = [&](ofxDatGuiSliderEvent e)
    {
        renderer.color = ofColor::ofColor_(redChannelSlider->getValue(), greenChannelSlider->getValue(), blueChannelSlider->getValue());
        hueSlider->setValue(renderer.color.getHue());
        saturationSlider->setValue(renderer.color.getSaturation());
        brightnessSlider->setValue(renderer.color.getBrightness());
    };

    redChannelSlider->onSliderEvent(rgbColorUpdate);
    greenChannelSlider->onSliderEvent(rgbColorUpdate);
    blueChannelSlider->onSliderEvent(rgbColorUpdate);

    ofxDatGuiFolder* hsbSliderFolder = toolsGui->addFolder("HSB", ofColor::black);
    hueSlider = hsbSliderFolder->addSlider("Hue", 0, 360, 360);
    saturationSlider = hsbSliderFolder->addSlider("Saturation", 0, 100, 100);
    brightnessSlider = hsbSliderFolder->addSlider("Brightness", 0, 100, 100);

    auto hsbColorUpdate = [&](ofxDatGuiSliderEvent e)
    {
        renderer.color = ofColor::fromHsb(hueSlider->getValue(), saturationSlider->getValue(), brightnessSlider->getValue());
        redChannelSlider->setValue(renderer.color.r);
        greenChannelSlider->setValue(renderer.color.g);
        blueChannelSlider->setValue(renderer.color.b);
    };

    hueSlider->onSliderEvent(hsbColorUpdate);
    saturationSlider->onSliderEvent(hsbColorUpdate);
    brightnessSlider->onSliderEvent(hsbColorUpdate);

    opacitySlider = toolsGui->addSlider("Opacity", 0, 100, 100);
    opacitySlider->onSliderEvent([&](ofxDatGuiSliderEvent e) { 
        if (renderer.activeImage != nullptr) 
            renderer.activeImage->opacity = opacitySlider->getValue() * 2.55;
    });

    ofxDatGuiButton* rotateBtn = toolsGui->addButton("Rotate");
    rotateBtn->onButtonEvent([&](ofxDatGuiButtonEvent e) {
        if (renderer.activeImage != nullptr)
            renderer.activeImage->imageData.rotate90(1);
    });

    histogram = new HistogramComponent("Histogram", ofColor::black);
    toolsGui->addFolder(histogram);

    imagesGui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
    imagesGui->setWidth(255);
    imagesGui->setPosition(ofGetWidth()-255, headerGui->getHeight()-1);
    imagesGui->addLabel("Images : ");

    renderer.offsetX1 = toolsGui->getWidth();
    renderer.offsetY2 = headerGui->getHeight();
    renderer.offsetX2 = ofGetWidth() - 225;
    renderer.setup();
}

void Application::draw()
{
  renderer.draw();
}

void Application::update() {
    if (isDragging) {
        renderer.activeImage->coordinates.x = ofGetMouseX();
        renderer.activeImage->coordinates.y = ofGetMouseY();
    }
}

void Application::windowResized(int w, int h)
{
    ofLog() << "<app::windowResized to: (" << w << ", " << h << ")>";
    headerLabel->setWidth(ofGetWidth());
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

    //if(renderer.activeImage->imageData.)
    isDragging = true;
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

void Application::import(string path) {
    ofImage imageData;
    imageData.load(path);

    std::string fileName;
    std::string::size_type idx = path.rfind('\\');
    if (idx != std::string::npos) {
        fileName = path.substr(idx + 1);
    }

    ofxDatGuiButton* button = imagesGui->addButton(fileName);
    button->onButtonEvent(this, &Application::onImageSelection);

    Image* image = new Image();
    image->imageData = imageData;

   

    renderer.images.insert({ fileName, image });

    renderer.activeImage = image;
    calculateHistogramData();
}

void Application::onExportEvent(ofxDatGuiButtonEvent e) 
{
    ofLog() << "<app::export>";
    filesFolder->collapse();
    ofImage image;
    image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    image.save("render.png");
}

void Application::calculateHistogramData()
{
    ofLog() << "<app::calculateHistogramData>";

    ofImage img = renderer.activeImage->imageData;
    ofPixels pixels = img.getPixels();

    int width = img.getWidth();
    int height = img.getHeight();

    // Create arrays to store the histograms
    int redHist[256] = { 0 };
    int greenHist[256] = { 0 };
    int blueHist[256] = { 0 };

    // Iterate through the pixels and update the histograms
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

void Application::onImageSelection(ofxDatGuiButtonEvent e) {
    Image* image = renderer.images.at(e.target->getLabel());
    renderer.activeImage = image;
    calculateHistogramData();
}