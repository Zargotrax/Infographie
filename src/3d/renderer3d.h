#include "ofMain.h"
#include "object/scene.h"

class Renderer3d
{
public:

	enum RenderMode { Wireframe, Solid, Shader};

	ofEasyCam* camera;
	Scene* scene;

	ofLight light;
	ofShader* shader;

	void setup();

	void update();

	void draw(Renderer3d::RenderMode renderMode);


};
