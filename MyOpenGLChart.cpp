#include "MyOpenGLChart.h"
#include <QOpenGLFunctions>
#include <QtMath>
#include <cmath>

MyOpenGLChart::MyOpenGLChart(const QVector<BarData> &data, QWidget *parent)
    : QOpenGLWidget(parent), m_data(data)
{
}

void MyOpenGLChart::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void MyOpenGLChart::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double aspect = (double)w / (double)h;
    double fovY = 45.0;
    double near = 1.0;
    double far = 100.0;
    double top = tan(fovY * M_PI / 360.0) * near;
    double bottom = -top;
    double left = bottom * aspect;
    double right = top * aspect;
    glFrustum(left, right, bottom, top, near, far);

}

void MyOpenGLChart::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-5.0f, -5.0f, -20.0f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);

    float xPos = 0.0f;
    for (const auto &bar : m_data)
    {
        float height = bar.cost / 10.0f;
        glPushMatrix();
        glTranslatef(xPos, 0.0f, 0.0f);

        glColor3f(qSin(xPos), qCos(xPos), 0.5f);
        glBegin(GL_QUADS);
        // Front
        glVertex3f(-0.5f, 0.0f, 0.5f);
        glVertex3f(0.5f, 0.0f, 0.5f);
        glVertex3f(0.5f, height, 0.5f);
        glVertex3f(-0.5f, height, 0.5f);
        // Back
        glVertex3f(-0.5f, 0.0f, -0.5f);
        glVertex3f(0.5f, 0.0f, -0.5f);
        glVertex3f(0.5f, height, -0.5f);
        glVertex3f(-0.5f, height, -0.5f);
        // Left
        glVertex3f(-0.5f, 0.0f, -0.5f);
        glVertex3f(-0.5f, 0.0f, 0.5f);
        glVertex3f(-0.5f, height, 0.5f);
        glVertex3f(-0.5f, height, -0.5f);
        // Right
        glVertex3f(0.5f, 0.0f, -0.5f);
        glVertex3f(0.5f, 0.0f, 0.5f);
        glVertex3f(0.5f, height, 0.5f);
        glVertex3f(0.5f, height, -0.5f);
        // Top
        glVertex3f(-0.5f, height, -0.5f);
        glVertex3f(0.5f, height, -0.5f);
        glVertex3f(0.5f, height, 0.5f);
        glVertex3f(-0.5f, height, 0.5f);
        glEnd();

        glPopMatrix();
        xPos += 2.0f;
    }
}
