#include "application.h"

ofxDatGui* gui;

// fonction appelée à l'initialisation de l'application
void Application::setup()
{
  ofSetWindowTitle("");
  ofLog() << "<app::setup>";

  gui = new ofxDatGui();

  ofxDatGuiFolder* filesFolder = gui->addFolder("Files", ofColor::white);
  ofxDatGuiButton* importBtn = filesFolder->addButton("Import");
  ofxDatGuiButton* exportBtn = filesFolder->addButton("Export");

  importBtn->onButtonEvent(this, &Application::onImportEvent);
  exportBtn->onButtonEvent(this, &Application::onExportEvent);

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
}

// fonction invoquée quand une sélection de fichiers est déposée sur la fenêtre de l'application
void Application::dragEvent(ofDragInfo dragInfo)
{
    ofLog() << "<app::ofDragInfo file[0]: " << dragInfo.files.at(0)
        << " at : " << dragInfo.position << ">";

    //// importer le premier fichier déposé sur la fenêtre si c'est une image (attention : aucune validation du type de fichier)
    //renderer.image.load(dragInfo.files.at(0));

    //// redimensionner la fenêtre selon la résolution de l'image
    //if (renderer.image.getWidth() > 0 && renderer.image.getHeight() > 0) {
    //    ofSetWindowShape(renderer.image.getWidth(), renderer.image.getHeight());
    //}
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