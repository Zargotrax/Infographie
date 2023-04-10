#include "ofMain.h"
#include "object/scene.h"
#include "./object/light.h"
#include "./3d/util/tess.h"

class Renderer3d
{
public:

	enum RenderMode { Wireframe, Texture, Lambert, Phong, Blinn_Phong, PBR, Tesselation };
	enum CameraMode { Perspective, Orthographic};

	TesselatorUtil* tesselator;

	ofEasyCam* camera;
	CameraMode cameraMode = Perspective;
	Scene* scene;

	Light ambiantLight;
	Light pointLight;
	Light directionalLight;
	Light spotLight;
	ofShader* lambert;
	ofShader* phong;
	ofShader* blinnPhong;
	ofShader* toneMapping;
	ofShader* pbr;
	ofImage cubemap[6];

	void setup();

	void update();

	void draw(Renderer3d::RenderMode renderMode, vector<Object*> selected);

	void setCameraToPerspective();

	void setCameraToOrthographic();
};
