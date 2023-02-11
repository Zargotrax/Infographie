#include "ofMain.h"
#include "ofxDatGui.h"
#include "2d/renderer2d.h"

class Application2d : public ofBaseApp
{
public:

	Renderer2d renderer;

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

	void onExportEvent(ofxDatGuiButtonEvent e);

	void onImageSelection(ofxDatGuiScrollViewEvent e);

	void onRgbSliderEvent(ofxDatGuiSliderEvent e);

	void onHsbSliderEvent(ofxDatGuiSliderEvent e);

	void onOpacitySliderEvent(ofxDatGuiSliderEvent e);

	void onRotateEvent(ofxDatGuiButtonEvent e);

	void onRemoveEvent(ofxDatGuiButtonEvent e);

	void onAddSquareEvent(ofxDatGuiButtonEvent e);

	void onAddRectangleEvent(ofxDatGuiButtonEvent e);

	void onAddCircleleEvent(ofxDatGuiButtonEvent e);

	void onAddEllipsisEvent(ofxDatGuiButtonEvent e);

	void onAddRegularPolygonEvent(ofxDatGuiButtonEvent e);

	void onAddStarEvent(ofxDatGuiButtonEvent e);

	void onAddArrowEvent(ofxDatGuiButtonEvent e);

	void onUpdateShapeSliderEvent(ofxDatGuiSliderEvent e);

	void onUpdateShapeColorPickerEvent(ofxDatGuiColorPickerEvent e);

	void onUpdateShapeToggleEvent(ofxDatGuiToggleEvent e);

	void onBgColorChangeEvent(ofxDatGuiColorPickerEvent e);

private:

	void import(string path);

	void calculateHistogramData();

	string getElementName(string filename);

	bool guiHitTest(int x, int y);

	void drawUI();

	void addElementToRenderer(TwoDimensionalObject* object);

	void removeCurrentSelectedObject();

	void updateShapeFromUi();

	void updateUiFromShape();

};
