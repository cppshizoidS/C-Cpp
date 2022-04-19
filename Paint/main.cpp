#include <time.h>
#include "Dot.h"
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <list>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

using namespace std;

float red = 1.0, green = 0.0, blue = 0.0;
int tmpx, tmpy; // store the first point when shape is line, rectangle or circle
int brushSize = 4;
int eraserSize = 1;
bool isSecond = false;
bool isRandom = false;
bool isEraser = false;
bool isRadial = false;
float window_w = 500;
float window_h = 500;

int shape = 1; // 1:point, 2:line, 3:rectangle, 4:circle, 5:brush

std::vector<Dot> dots;		// store all the points until clear
std::list<int> undoHistory; // record for undo, maximum 20 shapes in history
std::list<int> redoHistory; // record for redo, maximum 20 shapes in history
std::vector<Dot> redoDots;  // store the dots after undo temporaly
