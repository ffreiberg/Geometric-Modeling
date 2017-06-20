#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "knots.h"
#include "bspline.h"
#include "bezier.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    GLWidget                    (QWidget *parent=0);
    ~GLWidget                   ();
public slots:
    void setEpsilonDraw         (double value);
protected:
    void paintGL                ();
    void initializeGL           ();
    void resizeGL               (int width, int height);
    void mouseMoveEvent         (QMouseEvent *event);
    void mousePressEvent        (QMouseEvent *event);
private:
    QPointF                     transformPosition(QPoint p);
    BSpline                     *bSpline;
    float                       aspectx, aspecty;
    Points                      points, bezier;
    Knots                       knots;
    int                         clickedPoint;
    int                         clickedKnot;
    float                       epsilon_draw;
    int                         degree;
};



#endif // GLWIDGET_H
