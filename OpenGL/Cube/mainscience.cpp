#include <QtGui>
#include "mainscene.h"
#include <math.h>
#include <gl/glext.h>
#include <gl/gl.h>

MainScene::MainScene(QWidget *parent):QGLWidget(parent)
{
    xAxisRotation = yAxisRotation = 0;
}

MainScene::~MainScene()
{

}

void MainScene::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

void MainScene::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glOrtho(-0.5, 1.5, -0.5, 1.5, -10.0, 10.0);

    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);

    currentWidth = nWidth;
    currentHeight = nHeight;
}

void MainScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glRotatef(yAxisRotation, 0.0, 1.0, 0.0);
    glRotatef(xAxisRotation, 1.0, 0.0, 0.0);

    cubeVertexArray[0][0] = 0.0;
    cubeVertexArray[0][1] = 0.0;
    cubeVertexArray[0][2] = 1.0;

    cubeVertexArray[1][0] = 0.0;
    cubeVertexArray[1][1] = 1.0;
    cubeVertexArray[1][2] = 1.0;

    cubeVertexArray[2][0] = 1.0;
    cubeVertexArray[2][1] = 1.0;
    cubeVertexArray[2][2] = 1.0;

    cubeVertexArray[3][0] = 1.0;
    cubeVertexArray[3][1] = 0.0;
    cubeVertexArray[3][2] = 1.0;

    cubeVertexArray[4][0] = 0.0;
    cubeVertexArray[4][1] = 0.0;
    cubeVertexArray[4][2] = 0.0;

    cubeVertexArray[5][0] = 0.0;
    cubeVertexArray[5][1] = 1.0;
    cubeVertexArray[5][2] = 0.0;

    cubeVertexArray[6][0] = 1.0;
    cubeVertexArray[6][1] = 1.0;
    cubeVertexArray[6][2] = 0.0;

    cubeVertexArray[7][0] = 1.0;
    cubeVertexArray[7][1] = 0.0;
    cubeVertexArray[7][2] = 0.0;

    cubeColorArray[0][0] = 0.0;
    cubeColorArray[0][1] = 0.0;
    cubeColorArray[0][2] = 1.0;

    cubeColorArray[1][0] = 0.6;
    cubeColorArray[1][1] = 0.98;
    cubeColorArray[1][2] = 0.6;

    cubeColorArray[2][0] = 1.0;
    cubeColorArray[2][1] = 0.84;
    cubeColorArray[2][2] = 0.8;

    cubeColorArray[3][0] = 0.8;
    cubeColorArray[3][1] = 0.36;
    cubeColorArray[3][2] = 0.36;

    cubeColorArray[4][0] = 1.0;
    cubeColorArray[4][1] = 0.27;
    cubeColorArray[4][2] = 0.0;

    cubeColorArray[5][0] = 0.82;
    cubeColorArray[5][1] = 0.13;
    cubeColorArray[5][2] = 0.56;

    cubeColorArray[6][0] = 0.54;
    cubeColorArray[6][1] = 0.17;
    cubeColorArray[6][2] = 0.89;

    cubeColorArray[7][0] = 0.0;
    cubeColorArray[7][1] = 1.0;
    cubeColorArray[7][2] = 1.0;

    cubeIndexArray[0][0] = 0;
    cubeIndexArray[0][1] = 3;
    cubeIndexArray[0][2] = 2;
    cubeIndexArray[0][3] = 1;

    cubeIndexArray[1][0] = 0;
    cubeIndexArray[1][1] = 1;
    cubeIndexArray[1][2] = 5;
    cubeIndexArray[1][3] = 4;

    cubeIndexArray[2][0] = 7;
    cubeIndexArray[2][1] = 4;
    cubeIndexArray[2][2] = 5;
    cubeIndexArray[2][3] = 6;

    cubeIndexArray[3][0] = 3;
    cubeIndexArray[3][1] = 7;
    cubeIndexArray[3][2] = 6;
    cubeIndexArray[3][3] = 2;

    cubeIndexArray[4][0] = 1;
    cubeIndexArray[4][1] = 2;
    cubeIndexArray[4][2] = 6;
    cubeIndexArray[4][3] = 5;

    cubeIndexArray[5][0] = 0;
    cubeIndexArray[5][1] = 4;
    cubeIndexArray[5][2] = 7;
    cubeIndexArray[5][3] = 3;

    glVertexPointer(3, GL_FLOAT, 0, cubeVertexArray);
    glColorPointer(3, GL_FLOAT, 0, cubeColorArray);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndexArray);
}

void MainScene::mousePressEvent(QMouseEvent *event)
{
    pressPosition = event->pos();
}

void MainScene::mouseMoveEvent(QMouseEvent *event)
{
    xAxisRotation += (180 * ((GLfloat)event->y() - (GLfloat)pressPosition.y())) / (currentHeight);
    yAxisRotation += (180 * ((GLfloat)event->x() - (GLfloat)pressPosition.x())) / (currentWidth);

    pressPosition = event->pos();

    updateGL();
}

void MainScene::generateTextures()
{
    glGenTextures(6, textures);

    QImage texture1;
    texture1.load(":/cubeOne.jpg");
    texture1 = QGLWidget::convertToGLFormat(texture1);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)texture1.width(), (GLsizei)texture1.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture1.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage texture2;
    texture2.load(":/cubeTwo.jpg");
    texture2 = QGLWidget::convertToGLFormat(texture2);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)texture2.width(), (GLsizei)texture2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage texture3;
    texture3.load(":/cubeThree.jpg");
    texture3 = QGLWidget::convertToGLFormat(texture3);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)texture3.width(), (GLsizei)texture3.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture3.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage texture4;
    texture4.load(":/cubeFour.jpg");
    texture4 = QGLWidget::convertToGLFormat(texture4);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)texture4.width(), (GLsizei)texture4.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture4.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage texture5;
    texture5.load(":/cubeFive.jpg");
    texture5 = QGLWidget::convertToGLFormat(texture5);
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)texture5.width(), (GLsizei)texture5.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture5.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage texture6;
    texture6.load(":/cubeSix.jpg");
    texture6 = QGLWidget::convertToGLFormat(texture6);
    glBindTexture(GL_TEXTURE_2D, textures[5]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)texture6.width(), (GLsizei)texture6.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture6.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}
