#include "application.h"
#include "histogramComponent.h"

ofxDatGui* filesGui;
ofxDatGuiFolder* filesFolder;
ofxDatGui* toolsGui;
ofxDatGuiLabel* headerLabel;
HistogramComponent* histogram;
ofxDatGuiSlider* redChannelSlider;
ofxDatGuiSlider* greenChannelSlider;
ofxDatGuiSlider* blueChannelSlider;

void Application::setup()
{
    ofLog() << "<app::setup>";

    ofSetWindowTitle("Projet Infographie");

    ofxDatGui* headerGui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
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
    toolsGui->setPosition(0, headerGui->getHeight()-3);
    toolsGui->setWidth(256);

    ofxDatGuiFolder* channelSliderFolder = toolsGui->addFolder("Channels", ofColor::black);
    redChannelSlider = channelSliderFolder->addSlider("Red", 0, 255, 255);
    redChannelSlider->onSliderEvent([&](ofxDatGuiSliderEvent e) { renderer.redIntensity = redChannelSlider->getValue(); });
    greenChannelSlider = channelSliderFolder->addSlider("Green", 0, 255, 255);
    greenChannelSlider->onSliderEvent([&](ofxDatGuiSliderEvent e) { renderer.greenIntensity = greenChannelSlider->getValue(); });
    blueChannelSlider = channelSliderFolder->addSlider("Blue", 0, 255, 255);
    blueChannelSlider->onSliderEvent([&](ofxDatGuiSliderEvent e) { renderer.blueIntensity = blueChannelSlider->getValue(); });

    histogram = new HistogramComponent("Histogram", ofColor::black);
    toolsGui->addFolder(histogram);

    renderer.offsetX = toolsGui->getWidth();
    renderer.offsetY = headerGui->getHeight();
    renderer.setup();
}

void Application::draw()
{

  renderer.draw();
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

    renderer.image.load(dragInfo.files.at(0));
}

void Application::keyReleased(int key)
{
  ofLog() << "<app::keyReleased: " << key << ">";
}

void Application::mousePressed(int x, int y, int button)
{
    ofLog() << "<app::mouse pressed at: (" << x << ", " << y << ")>";
}

void Application::mouseReleased(int x, int y, int button)
{
  ofLog() << "<app::mouse released at: (" << x << ", " << y << ")>";
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
       ofImage image;
       image.load(openFileResult.getPath());
       renderer.image = image;
       calculateHistogramData();
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
}

void Application::calculateHistogramData()
{
    ofLog() << "<app::calculateHistogramData>";

    ofImage img = renderer.image;
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