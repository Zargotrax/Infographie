#include "ofMain.h"
#include "object/scene.h"
#include "./object/light.h"

class Renderer3d
{
public:

	enum RenderMode { Wireframe, Texture, Lambert, Phong, Blinn_Phong, PBR };
	enum CameraMode { Perspective, Orthographic};

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
