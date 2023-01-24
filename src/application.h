#include "ofMain.h"
#include "renderer.h"
#include "ofxDatGui.h"

class Application : public ofBaseApp
{
public:

  Renderer renderer;

  void setup();

  void draw();

  void update();

  void keyReleased(int key);

  void mousePressed(int x, int y, int button);

  void mouseReleased(int x, int y, int button);

  void windowResized(int w, int h);

  void dragEvent(ofDragInfo dragInfo);

  void exit();

  void onImportEvent(ofxDatGuiButtonEvent e);

  void onExportEvent(ofxDatGuiButtonEvent e);

  void onImageSelection(ofxDatGuiScrollViewEvent e);

  void onRgbSliderEvent(ofxDatGuiSliderEvent e);

  void onHsbSliderEvent(ofxDatGuiSliderEvent e);

  void onOpacitySliderEvent(ofxDatGuiSliderEvent e);

  void onRotateEvent(ofxDatGuiButtonEvent e);

  void onRemoveEvent(ofxDatGuiButtonEvent e);

private:

	void import(string path);

	void calculateHistogramData();

};
