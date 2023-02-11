#include "ofMain.h"
#include "ofxDatGui.h"

#ifndef OFMAIN_H
#define OFMAIN_H

class Application : public ofBaseApp
{
public:

	enum ViewType { View2D, View3D };

	void setup();

	void draw();

	void update();

	void keyPressed(int key);

	void keyReleased(int key);

	void mousePressed(int x, int y, int button);

	void mouseReleased(int x, int y, int button);

	void windowResized(int w, int h);

	void dragEvent(ofDragInfo dragInfo);

	void exit();

	void selectViewType();

	void changeViewTo2dEvent(ofxDatGuiButtonEvent e);

	void changeViewTo3dEvent(ofxDatGuiButtonEvent e);
};

#endif // !1
