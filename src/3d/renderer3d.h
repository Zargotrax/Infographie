#include "ofMain.h"
#include "object/scene.h"

class Renderer3d
{
public:

	enum RenderMode { Wireframe, Solid, Shader};
	enum CameraMode { Perspective, Orthographic};

	ofEasyCam* camera;
	CameraMode cameraMode = Perspective;
	Scene* scene;

	ofLight light;
	ofShader* shader;

	void setup();

	void update();

	void draw(Renderer3d::RenderMode renderMode);

	void setCameraToPerspective();

	void setCameraToOrthographic();
};
