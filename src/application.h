#include "ofMain.h"
#include "renderer.h"
#include "ofxDatGui.h"

class Application : public ofBaseApp
{
public:

  Renderer renderer;

  void setup();

  void draw();

  void keyReleased(int key);

  void mouseReleased(int x, int y, int button);

  void windowResized(int w, int h);

  void dragEvent(ofDragInfo dragInfo);

  void exit();

  void onImportEvent(ofxDatGuiButtonEvent e);
  void onExportEvent(ofxDatGuiButtonEvent e);
};
