#include "ofMain.h"
#include "ofApp.h"

int main()
{
  ofGLFWWindowSettings windowSettings;
  windowSettings.resizable = true;
  windowSettings.setSize(1250, 750);
  windowSettings.setGLVersion(4, 6);

  ofCreateWindow(windowSettings);

  Application* application = new Application();

  ofRunApp(application);

}
