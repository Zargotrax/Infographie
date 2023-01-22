#include "ofMain.h"
#include "renderer.h"
#include "ofxDatGui.h"

class Application : public ofBaseApp
{
public:

  Renderer renderer;
  std::map<string, ofImage> images;

  void setup();

  void draw();

  void keyReleased(int key);

  void mousePressed(int x, int y, int button);

  void mouseReleased(int x, int y, int button);

  void windowResized(int w, int h);

  void dragEvent(ofDragInfo dragInfo);

  void exit();

  void import(string path);
  void onImportEvent(ofxDatGuiButtonEvent e);
  void onExportEvent(ofxDatGuiButtonEvent e);
  void calculateHistogramData();
  void onImageSelection(ofxDatGuiButtonEvent e);

};
