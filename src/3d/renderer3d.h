#include "ofMain.h"
#include "object/scene.h"

class Renderer3d
{
public:

	enum RenderMode { Wireframe, Texture, Lambert, Phong, Blinn_Phong};
	enum CameraMode { Perspective, Orthographic};

	ofEasyCam* camera;
	CameraMode cameraMode = Perspective;
	Scene* scene;

	ofLight light;
	ofShader* lambert;
	ofShader* phong;
	ofShader* blinnPhong;
	ofShader* toneMapping;
	ofImage cubemap[6];

	void setup();

	void update();

	void draw(Renderer3d::RenderMode renderMode, vector<Object*> selected);

	void setCameraToPerspective();

	void setCameraToOrthographic();
};
