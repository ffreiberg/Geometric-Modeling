#ifndef POINTS_H
#define POINTS_H
#include <QList>
#include <QPointF>

class Points
{
public:
    Points();
    void  addPoint              (float, float);
    float getPointX             (int);
    float getPointY             (int);
    void  setPointX             (int, float);
    void  setPointY             (int, float);
    int   getCount              ();
    float getDistance           (int, float, float);
    int   getClosestPoint       ( float, float);
    void  moveClosestPoint      (float, float, float, float);

    Points getRange             (int, int);
    QPointF getPoint            (int);
    void clear                  ();

protected:
    QList<QPointF> pointlist;
};

#endif // POINTS_H
