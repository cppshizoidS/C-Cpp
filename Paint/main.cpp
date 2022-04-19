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

void display(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(2);
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < dots.size(); i++)
	{
		glColor3f(dots[i].getR(), dots[i].getG(), dots[i].getB());
		glVertex2i(dots[i].getX(), dots[i].getY());
	}
	glEnd();
	glutSwapBuffers();
}

void clear()
{
	dots.clear();
	undoHistory.clear();
	redoDots.clear();
	redoHistory.clear();
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	std::cout << asctime(timeinfo)
			  << "[Info] The window and the history are cleared successfully.\n";
}

void quit()
{
	std::cout << "Thank you for using this Paint tool! Goodbye!" << std::endl;
	exit(0);
}

void undo()
{
	if (undoHistory.size() > 0)
	{
		if (undoHistory.back() != dots.size() && redoHistory.back() != dots.size())
		{
			redoHistory.push_back(dots.size());
		}
		int numRemove = dots.size() - undoHistory.back();
		for (int i = 0; i < numRemove; i++)
		{
			redoDots.push_back(dots.back());
			dots.pop_back();
		}
		redoHistory.push_back(undoHistory.back());
		undoHistory.pop_back();
	}
	else
	{
		time_t rawtime;
		struct tm *timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		std::cout << asctime(timeinfo)
				  << "[Warning] Cannot undo. This is the first record in the history.\n";
	}
}
