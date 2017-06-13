#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "knots.h"

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
    float                       aspectx, aspecty;
    Points                      points;
    Knots                       knots;
    int                         clickedPoint;
    int                         clickedKnot;
    float                       epsilon_draw;
};



#endif // GLWIDGET_H
