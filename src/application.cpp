#include "application.h"
#include "histogramComponent.h"

ofxDatGui* filesGui;
ofxDatGui* toolsGui;
ofxDatGuiLabel* headerLabel;
HistogramComponent* histogram;

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

    ofxDatGuiFolder* filesFolder = filesGui->addFolder("Files", ofColor::black);
    ofxDatGuiButton* importBtn = filesFolder->addButton("Import");
    importBtn->setBackgroundColor(ofColor::darkGrey);
    ofxDatGuiButton* exportBtn = filesFolder->addButton("Export");
    exportBtn->setBackgroundColor(ofColor::darkGrey);

    importBtn->onButtonEvent(this, &Application::onImportEvent);
    exportBtn->onButtonEvent(this, &Application::onExportEvent);

    toolsGui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    toolsGui->setPosition(0, headerGui->getHeight()-3);
    toolsGui->setWidth(256);
    ofxDatGuiSlider* luminositySlider = toolsGui->addSlider("Luminosity", 0, 100);

    ofxDatGuiFolder* channelSliderFolder = toolsGui->addFolder("Channels", ofColor::black);
    ofxDatGuiSlider* redChannelSlider = channelSliderFolder->addSlider("Red", 0, 256);

    histogram = new HistogramComponent("Histogram", ofColor::black);
    toolsGui->addFolder(histogram);

    renderer.setup();
}

// fonction appelée à chaque mise à jour du rendu graphique de l'application
void Application::draw()
{
  renderer.draw();
}

void Application::windowResized(int w, int h)
{
    ofLog() << "<app::windowResized to: (" << w << ", " << h << ")>";
    headerLabel->setWidth(ofGetWidth());
}

// fonction invoquée quand une sélection de fichiers est déposée sur la fenêtre de l'application
void Application::dragEvent(ofDragInfo dragInfo)
{
    ofLog() << "<app::ofDragInfo file[0]: " << dragInfo.files.at(0)
        << " at : " << dragInfo.position << ">";

    renderer.image.load(dragInfo.files.at(0));
}

// fonction appelée quand une touche du clavier est relachée
void Application::keyReleased(int key)
{
  ofLog() << "<app::keyReleased: " << key << ">";
}

// fonction appelée quand un bouton d'un périphérique de pointage est relâché
void Application::mouseReleased(int x, int y, int button)
{
  ofLog() << "<app::mouse released at: (" << x << ", " << y << ")>";
}

// fonction appelée juste avant de quitter l'application
void Application::exit()
{
  ofLog() << "<app::exit>";
}

void Application::onImportEvent(ofxDatGuiButtonEvent e) {
    ofLog() << "<app::import>";
    ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an image");
    if (openFileResult.bSuccess) {
       ofImage image;
       image.load(openFileResult.getPath());
       renderer.image = image;
       calculateHistogramData();
       //ofSetWindowShape(image.getWidth(), image.getHeight());
       ofLog() << "<app::import - success>";
    }
    else {
        ofLog() << "<app::import - failed>";
    }
}

void Application::onExportEvent(ofxDatGuiButtonEvent e) 
{
    ofLog() << "<app::export>";

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