#include "bezierSurface.h"

void BezierSurface::setup(int w, int h, int dim, int res) {
    width = w;
    height = h;
    cx = dim - 1;
    cy = dim - 1;
    rx = res;
    ry = res;

    int i, j;
    for (i = 0; i <= cx; i++) {
        inp.push_back(vector<ofVec3f>());
        for (j = 0; j <= cy; j++) {
            inp[i].push_back(ofVec3f());
        }
    }
    for (i = 0; i < rx; i++) {
        outp.push_back(vector<ofVec3f>());
        for (j = 0; j < ry; j++) {
            outp[i].push_back(ofVec3f());
        }
    }

    for (i = 0; i <= cx; i++) {
        for (j = 0; j <= cy; j++) {
            inp[i][j].x = ofMap(i, 0, cx, 0, width);
            inp[i][j].y = ofMap(j, 0, cy, 0, height);
            inp[i][j].z = ofRandom(-50, 50);
        }
    }

    mesh = ofMesh::plane(width, height, rx, ry, OF_PRIMITIVE_TRIANGLES);
    createSurface();

    vector<glm::vec3> verts = mesh.getVertices();
    for (i = 0; i < verts.size(); i++) {
        mesh.setTexCoord(i, ofVec2f(verts[i].x, verts[i].y));
    }
}

void BezierSurface::modifyControlPoint(int index, ofVec3f newPos) {
    int offsetX = index / inp.size();
    int offsetY = index % inp.size();
    inp[offsetX][offsetY] = newPos;
}

void BezierSurface::draw() {
    mesh.draw();
}

void BezierSurface::drawWireframe() {
    mesh.drawWireframe();
}

void BezierSurface::update() {
        createSurface();
}

void BezierSurface::drawControls() {
    ofSetColor(ofColor::black);
    int index = 0;
    for (vector<ofVec3f> column : inp) {
        for (ofVec3f ctrlPts : column) {
            ofSetColor(ofColor::black);
            ofDrawSphere(ctrlPts, 3);
            ofSetColor(ofColor::white);
            ofDrawBitmapString(to_string(index), ctrlPts);
            index++;
        }
    }
}

ofVec3f BezierSurface::getControlPoint(int index) {
    int offsetX = index / inp.size();
    int offsetY = index % inp.size();
    return inp[offsetX][offsetY];
}

//----------------------------------------------------- bezier.
void BezierSurface::createSurface() {
    outp = calculateSurface(inp, outp, cx, cy, rx, ry);

    int i, j;
    int cnt;
    vector<ofVec3f> verts;
    for (i = 0; i < rx; i++) {
        for (j = 0; j < ry; j++) {
            verts.push_back(outp[i][j]);
        }
    }
    for (i = 0; i < verts.size(); i++) {
        mesh.setVertex(i, verts[i]);
    }
}

vector< vector<ofVec3f> > BezierSurface::calculateSurface(vector< vector<ofVec3f> > ip, vector< vector<ofVec3f> > op,
    int cpx, int cpy, int rpx, int rpy) {
    double mui, muj;
    double bi, bj;
    float x, y, z;

    for (int i = 0; i < rpx; i++) {
        mui = i / (double)(rpx - 1);
        for (int j = 0; j < rpy; j++) {
            muj = j / (double)(rpy - 1);

            op[i][j].x = 0;
            op[i][j].y = 0;
            op[i][j].z = 0;

            for (int ki = 0; ki <= cpx; ki++) {
                bi = bezierBlend(ki, mui, cpx);
                for (int kj = 0; kj <= cpy; kj++) {
                    bj = bezierBlend(kj, muj, cpy);
                    op[i][j].x += (ip[ki][kj].x * bi * bj);
                    op[i][j].y += (ip[ki][kj].y * bi * bj);
                    op[i][j].z += (ip[ki][kj].z * bi * bj);
                }
            }
        }
    }

    return op;
}

double BezierSurface::bezierBlend(int k, double mu, int n) {
    int nn, kn, nkn;
    double blend = 1;

    nn = n;
    kn = k;
    nkn = n - k;

    while (nn >= 1) {
        blend *= nn;
        nn--;
        if (kn > 1) {
            blend /= (double)kn;
            kn--;
        }
        if (nkn > 1) {
            blend /= (double)nkn;
            nkn--;
        }
    }
    if (k > 0)
        blend *= pow(mu, (double)k);
    if (n - k > 0)
        blend *= pow(1 - mu, (double)(n - k));

    return(blend);
}