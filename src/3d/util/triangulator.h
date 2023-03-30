#pragma once

#include "ofMain.h"

struct vec3 {
	double x, y, z;
	int i;
};

struct triangle {
	int v1, v2, v3;
};

struct edge {
	int v1, v2;
};

const double EPSILON = 0.000001;

class Triangulator {

public:

	ofMesh mesh;

	void addPoint(float x, float y, float z);
	void triangulate();

private:
	vector<vec3> vertices; //only input of triangulate();
	vector<triangle> triangles;

	int triangulate(int nv, vec3 pxyz[], triangle v[], int& ntri);
	int circumCircle(double xp, double yp, double x1, double y1, double x2,
		double y2, double x3, double y3, double& xc, double& yc, double& r);
};
