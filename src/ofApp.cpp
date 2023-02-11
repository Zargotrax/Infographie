#include "ofApp.h"
#include <2d/application2d.h>
#include <3d/application3d.h>

Application::ViewType viewType = Application::ViewType::View3D;

Application2d* application2D;
Application3d* application3D;

ofxDatGui* header;
ofxDatGuiLabel* headerLabel;

ofxDatGuiFolder* viewMenuFolder;

// Gui setup /////////////////////////////////////////////////////////////////////////////////////////////////
void Application::setup()
{
    ofLog() << "<app::setup>";

    ofSetWindowTitle("Projet Infographie");
    ofSetCircleResolution(50);

    header = new ofxDatGui(60, 0);
    headerLabel = header->addLabel("Projet Infographie");
    headerLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    headerLabel->setWidth(ofGetWidth());
    headerLabel->setStripeVisible(false);

    ofxDatGui* viewMenu = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    viewMenu->setWidth(60);
    viewMenuFolder = viewMenu->addFolder("View");
    ofxDatGuiButton* view2DButton = viewMenuFolder->addButton("2D Editor");
    view2DButton->onButtonEvent(this, &Application::changeViewTo2dEvent);
    ofxDatGuiButton* view3DButton = viewMenuFolder->addButton("3D Editor");
    view3DButton->onButtonEvent(this, &Application::changeViewTo3dEvent);

    application2D = new Application2d();
    application3D = new Application3d();

    application2D->setup(header);
    application3D->setup(header);

    selectViewType();
}

void Application::draw() {
    if (viewType == Application::ViewType::View2D) {
        application2D->draw();
    } else if (viewType == Application::ViewType::View3D) {
        application3D->draw();
    }
}

void Application::update() {
    if (viewType == Application::ViewType::View2D) {
        application2D->update();
    } else if (viewType == Application::ViewType::View3D) {
        application3D->update();
    }
}

void Application::windowResized(int w, int h) {
    headerLabel->setWidth(ofGetWidth());
    application2D->windowResized(w, h);
}

void Application::dragEvent(ofDragInfo dragInfo) {
    if (viewType == Application::ViewType::View2D) {
        application2D->dragEvent(dragInfo);
    } else if (viewType == Application::ViewType::View3D) {
        application3D->dragEvent(dragInfo);
    }
}

void Application::keyPressed(int key) {
    if (viewType == Application::ViewType::View2D) {
        application2D->keyPressed(key);
    } else if (viewType == Application::ViewType::View3D) {
        application3D->keyPressed(key);
    }
}

void Application::keyReleased(int key) {
    if (viewType == Application::ViewType::View2D) {
        application2D->keyReleased(key);
    } else if (viewType == Application::ViewType::View3D) {
        application3D->keyReleased(key);
    }
}

void Application::mousePressed(int x, int y, int button) {
    if (viewType == Application::ViewType::View2D) {
        application2D->mousePressed(x, y, button);
    } else if (viewType == Application::ViewType::View3D) {
        application3D->mousePressed(x, y, button);
    }
}

void Application::mouseReleased(int x, int y, int button) {
    if (viewType == Application::ViewType::View2D) {
        application2D->mouseReleased(x, y, button);
    } else if (viewType == Application::ViewType::View3D) {
        application3D->mouseReleased(x, y, button);
    }
}

void Application::exit() {
  ofLog() << "<app::exit>";
}

void Application::selectViewType() {
    if (viewType == Application::ViewType::View2D) {
        application2D->showUi();
        application3D->hideUi();
    }
    else if (viewType == Application::ViewType::View3D) {
        application3D->showUi();
        application2D->hideUi();
    }
}

void Application::changeViewTo2dEvent(ofxDatGuiButtonEvent e) {
    viewType = Application::ViewType::View2D;
    selectViewType();
    viewMenuFolder->collapse();
}

void Application::changeViewTo3dEvent(ofxDatGuiButtonEvent e) {
    viewType = Application::ViewType::View3D;
    selectViewType();
    viewMenuFolder->collapse();
}
