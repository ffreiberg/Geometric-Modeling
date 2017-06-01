
#include "glwidget.h"
#include <QtGui>
#include <GL/glu.h>
#include "mainwindow.h"
#include "bezier.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    doIntersection       = false;
    doSelfIntersection   = false;
    doAddSegment         = true;
    seg                  = false;
    epsilon_draw         = (float)0.05;
    epsilon_intersection = (float)0.000005;

    // Hier Punkte hinzufügen (schönere Startpositionen!)
    points.addPoint(-1.00, -0.5);
    points.addPoint(-0.75, -0.5);
    points.addPoint(-0.50, -0.5);
    points.addPoint(-0.25, -0.5);
    points.addPoint( 0.00, -0.5);

    points.addPoint( 0.25,  0.5);
    points.addPoint( 0.50,  0.5);
    points.addPoint( 0.75,  0.5);
    points.addPoint( 1.00,  0.5);
}

GLWidget::~GLWidget()
{
}

void GLWidget::paintGL()
{
    // clear
    glClear(GL_COLOR_BUFFER_BIT);

    // Koordinatensystem
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_LINES);
    glVertex2f(-1.0, 0.0);
    glVertex2f( 1.0, 0.0);
    glVertex2f( 0.0,-1.0);
    glVertex2f( 0.0, 1.0);
    glEnd();
    glColor3f(1.0,0.0,0.0);

    // Punkte
    glPointSize(7.0);
    glBegin(GL_POINTS);
    for (int i=0; i<points.getCount(); i++) {
        glVertex2f(points.getPointX(i),points.getPointY(i));
    }

    glEnd();

    // Hüllpolygone zeichnen
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINE_STRIP);
    for (int i=0; i<5; i++) {
        glVertex2f(points.getPointX(i),points.getPointY(i));
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int i=5; i<points.getCount(); i++) {
        glVertex2f(points.getPointX(i),points.getPointY(i));
    }
    glEnd();

    // Kurve
    glColor3f(1.0,0.0,0.5);
    // AUFGABE: Hier Kurve zeichnen
    // dabei epsilon_draw benutzen
/*
    Points polygonLeft, polygonRight;

    for (int i = 0; i < 5; ++i) {
        polygonLeft.addPoint(points.getPointX(i), points.getPointY(i));
    }

    for (int i = 5; i < points.getCount(); ++i) {
        polygonRight.addPoint(points.getPointX(i), points.getPointY(i));
    }

    Points bezierCurveLeft = bezier(polygonLeft);
    Points bezierCurveRight = bezier(polygonRight);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < bezierCurveLeft.getCount(); ++i) {
        glVertex2f(bezierCurveLeft.getPointX(i), bezierCurveLeft.getPointY(i));
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < bezierCurveRight.getCount(); ++i) {
        glVertex2f(bezierCurveRight.getPointX(i), bezierCurveRight.getPointY(i));
    }
    glEnd();
*/
    Bezier bezierCurveLeft(points.getRange(0,5),epsilon_intersection, epsilon_draw);
    Bezier bezierCurveRight(points.getRange(5,points.getCount()),epsilon_intersection, epsilon_draw);

    Points curveLeft = bezierCurveLeft.bezier();
    //glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < curveLeft.getCount(); i++) {
        glVertex2f(curveLeft.getPointX(i), curveLeft.getPointY(i));
    }
    glEnd();

    Points curveRight = bezierCurveRight.bezier();
    glColor3f(0.5, 0.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < curveRight.getCount(); i++) {
        glVertex2f(curveRight.getPointX(i), curveRight.getPointY(i));
    }
    glEnd();

    // Schnittpunkte zeichnen
    if (doIntersection) {
        QList<QPointF> intersections = bezierCurveLeft.intersectBezier(bezierCurveRight.getControlPoints());
        glColor3f(0.0,1.0,0.0);
        glBegin(GL_POINTS);
        for (auto p : intersections) {
            glVertex2f(p.x(),p.y());
        }
        glEnd();
        // AUFGABE: Hier Schnitte zeichnen
        // dabei epsilon_intersection benutzen
    }
    // AUFGABE: Hier Selbstschnitte zeichnen
    // dabei epsilon_intersection benutzen
    if (doSelfIntersection) {
        glColor3f(0.0,1.0,0.0);
        QList<QPointF> bezierLeftSelfIntersections = bezierCurveLeft.selfIntersect();
        glBegin(GL_POINTS);
        for (auto p : bezierLeftSelfIntersections) {
            glVertex2f(p.x(),p.y());
        }
        glEnd();

        QList<QPointF> bezierRightSelfIntersection = bezierCurveRight.selfIntersect();
        glBegin(GL_POINTS);
        for (auto p : bezierRightSelfIntersection) {
            glVertex2f(p.x(),p.y());
        }
        glEnd();
    }

    if (doAddSegment){
        Points newSegment;
        if(seg){
            newSegment = bezierCurveLeft.addNewSegment(newPoint);
            //Bezier new_segment_curve(new_segment, epsilon_intersection, epsilon_draw);
            Points newSegmentCurve = bezierCurveLeft.getBezierCurve();

            glColor3f(0.0,1.0,1.0);
            glBegin(GL_LINE_STRIP);
            for (int i=0; i<newSegmentCurve.getCount(); i++) {
                glVertex2f(newSegmentCurve.getPointX(i),newSegmentCurve.getPointY(i));
            }
            glEnd();
        }else{
            newSegment = bezierCurveRight.addNewSegment(newPoint);
            //Bezier new_segment_curve(new_segment, epsilon_intersection, epsilon_draw);
            Points newSegmentCurve = bezierCurveRight.getBezierCurve();

            glColor3f(0.0,1.0,1.0);
            glBegin(GL_LINE_STRIP);
            for (int i=0; i<newSegmentCurve.getCount(); i++) {
                glVertex2f(newSegmentCurve.getPointX(i),newSegmentCurve.getPointY(i));
            }
            glEnd();
        }

        glPointSize(7.0);
        glBegin(GL_POINTS);
        for (int i=0; i < newSegment.getCount(); i++) {
            glVertex2f(newSegment.getPointX(i),newSegment.getPointY(i));
        }
        glEnd();

        // Hüllpolygone zeichnen
        glColor3f(0.0,0.0,1.0);
        glBegin(GL_LINE_STRIP);
        for (int i=0; i<newSegment.getCount(); i++) {
            glVertex2f(newSegment.getPointX(i),newSegment.getPointY(i));
        }
        glEnd();
    }

}


void GLWidget::initializeGL()
{
    resizeGL(width(),height());
}

void GLWidget::resizeGL(int width, int height)
{
    aspectx=1.0;
    aspecty=1.0;
    if (width>height) {
        aspectx=float(width)/height;
    }
    else {
        aspecty=float(height)/width;
    }
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-aspectx,aspectx,-aspecty,aspecty);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

QPointF GLWidget::transformPosition(QPoint p)
{
    return QPointF((2.0*p.x()/width() - 1.0)*aspectx,-(2.0*p.y()/height() - 1.0)*aspecty);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint pos = event->pos();
        QPointF posF = transformPosition(pos);
        points.setPointX(clickedPoint,posF.x());
        points.setPointY(clickedPoint,posF.y());
        update();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint pos = event->pos();
        QPointF posF = transformPosition(pos);
        clickedPoint = points.getClosestPoint(posF.x(),posF.y());
        points.setPointX(clickedPoint,posF.x());
        points.setPointY(clickedPoint,posF.y());
        update();
    }

    if (event->buttons() & Qt::RightButton) {
        QPoint pos = event->pos();
        QPointF posF = transformPosition(pos);
        newPoint.setX(posF.x());
        newPoint.setY(posF.y());
        seg=true;
        update();
    }

    if (event->buttons() & Qt::MidButton) {
        QPoint pos = event->pos();
        QPointF posF = transformPosition(pos);
        newPoint.setX(posF.x());
        newPoint.setY(posF.y());
        seg = false;
        update();
    }
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *)
{

}

void GLWidget::setIntersection(int state)
{
    doIntersection = (state == Qt::Checked);
    update();
}

void GLWidget::setSelfIntersection(int state)
{
    doSelfIntersection = (state == Qt::Checked);
    update();
}

void GLWidget::setEpsilonDraw(double value)
{
    epsilon_draw = value;
}

void GLWidget::setEpsilonIntersection(double value)
{
    epsilon_intersection = value;
}

Points GLWidget::bezier(Points controlPolygon)
{
    Points bezierPoints;

    for (float t = 0; t <= 1.; t += epsilon_draw) {
        Points p = deCasteljau(controlPolygon, t);
        bezierPoints.addPoint(p.getPointX(0), p.getPointY(0));
    }
    bezierPoints.addPoint(controlPolygon.getPointX(controlPolygon.getCount() - 1), controlPolygon.getPointY(controlPolygon.getCount() - 1));
    return bezierPoints;
}

Points GLWidget::deCasteljau(Points controlPoints, float t)
{
    Points pNext;

    for (int i = 0; i < controlPoints.getCount() - 1; ++i) {
        //bij = (1 - t)bij-1 + tbi+1j-1
        float x = (1 - t) * controlPoints.getPointX(i) + controlPoints.getPointX(i + 1) * t;
        float y = (1 - t) * controlPoints.getPointY(i) + controlPoints.getPointY(i + 1) * t;
        pNext.addPoint(x,y);
    }

    if (pNext.getCount() == 1)
        return pNext;
    else
        return deCasteljau(pNext, t);
}
