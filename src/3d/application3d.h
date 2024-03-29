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

	void onChangeCameraMode(ofxDatGuiButtonEvent e);

	void onDeleteEvent(ofxDatGuiButtonEvent e);

	void onRandomTextureEvent(ofxDatGuiButtonEvent e);

	void onImportTextureEvent(ofxDatGuiButtonEvent e);

	void onTextureMagFilterDropdownSelection(ofxDatGuiDropdownEvent e);

	void onTextureFilterDropdownSelection(ofxDatGuiDropdownEvent e);

	void onToneMappingEvent(ofxDatGuiSliderEvent e);

	void onMaterialColorChangeEvent(ofxDatGuiColorPickerEvent e);

	void onCurveControlPointPositionChangeEvent(ofxDatGuiSliderEvent e);

	void onAddBezierCurveEvent(ofxDatGuiButtonEvent e);

	void onAddCustomObjectEvent(ofxDatGuiButtonEvent e);

	void onImportNormalMapEvent(ofxDatGuiButtonEvent e);

	void onAddBezierSurfaceEvent(ofxDatGuiButtonEvent e);

	void onSurfaceControlPointPositionChangeEvent(ofxDatGuiSliderEvent e);

	void onSurfacePointControlSelectionEvent(ofxDatGuiDropdownEvent e);

	void onRayTracingRenderEvent(ofxDatGuiButtonEvent e); 
	
	void onMaterialFactorChangeEvent(ofxDatGuiSliderEvent e);

	void onMaterialFactorIorChangeEvent(ofxDatGuiColorPickerEvent e);

	void onImportMetallicMapEvent(ofxDatGuiButtonEvent e);

	void onImportRoughnesslMapEvent(ofxDatGuiButtonEvent e);

	void onImportOcclusionMapEvent(ofxDatGuiButtonEvent e);

	void onLightColorChangeEvent(ofxDatGuiColorPickerEvent e);

	void onLightBrightnessChangeEvent(ofxDatGuiSliderEvent e);

private:

	void import(string path);

	string getElementName(string filename);

	vector<Object*> getAllElementFromScene();

	vector<Object*> getAllElementFromObject(Object* object);

	void undo();

	void redo();

	Renderer3d::RenderMode getRenderMode();

	void addObject(Object* object, string filename);

};