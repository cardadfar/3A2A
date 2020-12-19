#ifndef SIDE_H
#define SIDE_H

#include "Dot.h"
#include "Color.h"
#include "Frame.h"
#include "Transform.h"


#include <time.h>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <list>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

using namespace std;

class Canvas {
public:

    Canvas(int xmin, int ymin, int xmax, int ymax, int windowW, int windowH) {
        x_min = xmin;
        y_min = ymin;
        x_max = xmax;
        y_max = ymax;
        window_w = windowW;
        window_h = windowH;
        hspan = window_w / 2;
	    vspan = window_h / 2;
        frame = new Frame();
        
    }

    const static string strokeDict[];
    static vector<Color> colors;

    static map<int, Color> createColors();

    void display(void);

    void clear();

    void drawDot(int mousex, int mousey);

    void swatchSwitch(int s);

    void mouse(int x, int y);

    void motion(int x, int y);

    void setTransform(Matrix3x3 t);

private:
    Frame *frame;
    Matrix3x3 transform;
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int window_w;
    int window_h;
    int hspan;
    int vspan;

};

#endif