#include "Triangulator.h"

void Triangulator::addPoint(float x, float y, float z) {
	vec3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.i = vertices.size();
	vertices.push_back(v);
}

int vec3Compare(const void* v1, const void* v2) {
    vec3* p1 = (vec3*)v1;
    vec3* p2 = (vec3*)v2;
    if (p1->x < p2->x)
        return(-1);
    else if (p1->x > p2->x)
        return(1);
    else
        return(0);
}

void Triangulator::triangulate() {

    if (vertices.size() < 3) {
        return;
    }

    int nv = vertices.size();

    vector<vec3> verticesTemp = vertices;
    qsort(&verticesTemp[0], verticesTemp.size(), sizeof(vec3), vec3Compare);

    vector<vec3> verticesvec3;

    for (int i = 0; i < nv; i++) {
        vec3 v;
        v.x = verticesTemp.at(i).x;
        v.y = verticesTemp.at(i).y;
        v.z = verticesTemp.at(i).z;
        verticesvec3.push_back(v);
    }

    verticesvec3.push_back(vec3());
    verticesvec3.push_back(vec3());
    verticesvec3.push_back(vec3());

    int nbTri = 3 * nv;
    triangles.resize(nbTri);

    triangulate(nv, &verticesvec3[0], &triangles[0], nbTri);

    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);

    for (int i = 0; i < nv; i++) {
        mesh.addVertex(ofVec3f(vertices[i].x, vertices[i].y, vertices[i].z));
    }

    for (int i = 0; i < nbTri; i++) {
        mesh.addIndex(verticesTemp.at(triangles[i].v1).i);
        mesh.addIndex(verticesTemp.at(triangles[i].v2).i);
        mesh.addIndex(verticesTemp.at(triangles[i].v3).i);
    }
}

int Triangulator::triangulate(int nv, vec3 pvec3[], triangle v[], int& ntri)
{
    int* complete = NULL;
    edge* edges = NULL;
    edge* edgeTmp;
    int nedge = 0;
    int trimax, emax = 200;
    int status = 0;

    int inside;
    int i, j, k;
    double xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r;
    double xmin, xmax, ymin, ymax, xmid, ymid;
    double dx, dy, dmax;

    trimax = 4 * nv;
    complete = new int[trimax];
    edges = new edge[emax];

    xmin = pvec3[0].x;
    ymin = pvec3[0].y;
    xmax = xmin;
    ymax = ymin;
    for (i = 1; i < nv; i++) {
        if (pvec3[i].x < xmin) xmin = pvec3[i].x;
        if (pvec3[i].x > xmax) xmax = pvec3[i].x;
        if (pvec3[i].y < ymin) ymin = pvec3[i].y;
        if (pvec3[i].y > ymax) ymax = pvec3[i].y;
    }
    dx = xmax - xmin;
    dy = ymax - ymin;
    dmax = (dx > dy) ? dx : dy;
    xmid = (xmax + xmin) / 2.0;
    ymid = (ymax + ymin) / 2.0;

    pvec3[nv + 0].x = xmid - 20 * dmax;
    pvec3[nv + 0].y = ymid - dmax;
    pvec3[nv + 0].z = 0.0;
    pvec3[nv + 1].x = xmid;
    pvec3[nv + 1].y = ymid + 20 * dmax;
    pvec3[nv + 1].z = 0.0;
    pvec3[nv + 2].x = xmid + 20 * dmax;
    pvec3[nv + 2].y = ymid - dmax;
    pvec3[nv + 0].z = 0.0;
    v[0].v1 = nv;
    v[0].v2 = nv + 1;
    v[0].v3 = nv + 2;
    complete[0] = false;
    ntri = 1;


    for (i = 0; i < nv; i++) {
        xp = pvec3[i].x;
        yp = pvec3[i].y;
        nedge = 0;
        for (j = 0; j < ntri; j++) {
            if (complete[j])
                continue;
            x1 = pvec3[v[j].v1].x;
            y1 = pvec3[v[j].v1].y;
            x2 = pvec3[v[j].v2].x;
            y2 = pvec3[v[j].v2].y;
            x3 = pvec3[v[j].v3].x;
            y3 = pvec3[v[j].v3].y;
            inside = circumCircle(xp, yp, x1, y1, x2, y2, x3, y3, xc, yc, r);

            if (xc < xp && ((xp - xc) * (xp - xc)) > r)
                complete[j] = true;

            if (inside) {
                if (nedge + 3 >= emax) {
                    emax += 100;
                    edgeTmp = new edge[emax];
                    for (int i = 0; i < nedge; i++) {
                        edgeTmp[i] = edges[i];
                    }
                    delete[]edges;
                    edges = edgeTmp;
                }

                edges[nedge + 0].v1 = v[j].v1;
                edges[nedge + 0].v2 = v[j].v2;
                edges[nedge + 1].v1 = v[j].v2;
                edges[nedge + 1].v2 = v[j].v3;
                edges[nedge + 2].v1 = v[j].v3;
                edges[nedge + 2].v2 = v[j].v1;
                nedge += 3;
                v[j] = v[ntri - 1];
                complete[j] = complete[ntri - 1];
                ntri--;
                j--;
            }
        }

        for (j = 0; j < nedge - 1; j++) {
            for (k = j + 1; k < nedge; k++) {
                if ((edges[j].v1 == edges[k].v2) && (edges[j].v2 == edges[k].v1)) {
                    edges[j].v1 = -1;
                    edges[j].v2 = -1;
                    edges[k].v1 = -1;
                    edges[k].v2 = -1;
                }
                if ((edges[j].v1 == edges[k].v1) && (edges[j].v2 == edges[k].v2)) {
                    edges[j].v1 = -1;
                    edges[j].v2 = -1;
                    edges[k].v1 = -1;
                    edges[k].v2 = -1;
                }
            }
        }

        for (j = 0; j < nedge; j++) {
            if (edges[j].v1 < 0 || edges[j].v2 < 0)
                continue;

            if (ntri >= trimax) {
                break;
            }

            v[ntri].v1 = edges[j].v1;
            v[ntri].v2 = edges[j].v2;
            v[ntri].v3 = i;
            complete[ntri] = false;
            ntri++;
        }

        if (ntri >= trimax) {
            break;
        }
    }

    for (i = 0; i < ntri; i++) {
        if (v[i].v1 >= nv || v[i].v2 >= nv || v[i].v3 >= nv) {
            v[i] = v[ntri - 1];
            ntri--;
            i--;
        }
    }

    delete[] edges;
    delete[] complete;
    return 0;
}

int Triangulator::circumCircle(double xp, double yp, double x1, double y1, double x2,
    double y2, double x3, double y3, double& xc, double& yc, double& r) {
    double m1, m2, mx1, mx2, my1, my2;
    double dx, dy, rsqr, drsqr;

    if (abs(y1 - y2) < EPSILON && abs(y2 - y3) < EPSILON)
        return(false);

    if (abs(y2 - y1) < EPSILON) {
        m2 = -(x3 - x2) / (y3 - y2);
        mx2 = (x2 + x3) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc = (x2 + x1) / 2.0;
        yc = m2 * (xc - mx2) + my2;
    }
    else if (abs(y3 - y2) < EPSILON) {
        m1 = -(x2 - x1) / (y2 - y1);
        mx1 = (x1 + x2) / 2.0;
        my1 = (y1 + y2) / 2.0;
        xc = (x3 + x2) / 2.0;
        yc = m1 * (xc - mx1) + my1;
    }
    else {
        m1 = -(x2 - x1) / (y2 - y1);
        m2 = -(x3 - x2) / (y3 - y2);
        mx1 = (x1 + x2) / 2.0;
        mx2 = (x2 + x3) / 2.0;
        my1 = (y1 + y2) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        yc = m1 * (xc - mx1) + my1;
    }

    dx = x2 - xc;
    dy = y2 - yc;
    rsqr = dx * dx + dy * dy;
    r = sqrt(rsqr);
    dx = xp - xc;
    dy = yp - yc;
    drsqr = dx * dx + dy * dy;

    return((drsqr <= rsqr) ? true : false);
}
