#include "ofMain.h"
#include "ofApp.h"

int main()
{
	ofGLFWWindowSettings windowSettings;
	windowSettings.resizable = true;
	windowSettings.setGLVersion(4, 3);
	windowSettings.numSamples = 4;
	windowSettings.setSize(1250, 750);
	ofCreateWindow(windowSettings);
	ofRunApp(new Application());
}
