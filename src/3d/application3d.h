#include "ofMain.h"
#include "renderer3d.h"
#include "ofxDatGui.h"
#include "ofxAssimpModelLoader.h"

class Application3d : public ofBaseApp
{
public:

	Renderer3d renderer;

	void setup(ofxDatGui* header);

	void draw();

	void update();

	void showUi();

	void hideUi();

	void keyPressed(int key);

	void keyReleased(int key);

	void mousePressed(int x, int y, int button);

	void mouseReleased(int x, int y, int button);

	void windowResized(int w, int h);

	void dragEvent(ofDragInfo dragInfo);

	void onImportEvent(ofxDatGuiButtonEvent e);

	void onObjectSelection(ofxDatGuiScrollViewEvent e);

	void onUndoEvent(ofxDatGuiButtonEvent e);

	void onRedoEvent(ofxDatGuiButtonEvent e);

	void onApplyTransformationEvent(ofxDatGuiButtonEvent e);

	void onAddCylinderEvent(ofxDatGuiButtonEvent e);

	void onAddSphereEvent(ofxDatGuiButtonEvent e);

	void onEnableTurntable(ofxDatGuiButtonEvent e);

	void onEnableTranslationAnimation(ofxDatGuiButtonEvent e);

private:

	void import(string path);

	string getElementName(string filename);

	vector<Object*> getAllElementFromScene();

	vector<Object*> getAllElementFromObject(Object* object);

	void undo();

	void redo();

	Renderer3d::RenderMode getRenderMode();

	void loadShader(string path);

};