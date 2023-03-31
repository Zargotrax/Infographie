#pragma once

#include "ofVboMesh.h"
#include "ofMain.h"

class BezierSurface {
public:
    void setup(int w, int h, int dim, int res);
    void update();
    void draw();
    void drawWireframe();
    void drawControls();
    void modifyControlPoint(int index, ofVec3f newPos);
    ofVec3f getControlPoint(int index);

private:
    int width;
    int height;
    int cx;
    int cy;
    int rx;
    int ry;
    vector<vector<ofVec3f>> inp;
    vector<vector<ofVec3f>> outp;
    void createSurface();
    vector< vector<ofVec3f>> calculateSurface(vector< vector<ofVec3f> > ip, vector< vector<ofVec3f> > op, int cpx, int cpy, int rpx, int rpy);
    double bezierBlend(int k, double mu, int n);

    ofVboMesh mesh;
};