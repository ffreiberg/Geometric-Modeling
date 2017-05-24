#ifndef BEZIER_H
#define BEZIER_H

#include "points.h"

class Bezier
{
public:
    Bezier(Points, float, float);

    bool boundingOverlap(Points, Points);

    void findIntersections();
    void intersectBezier(Points, Points);
    void selfIntersect(Points);

    Points bezier();
    Points getBezierCurve();
    Points getControlPoints();

    QList<QPointF> selfIntersect();
    QList<QPointF> intersectBezier(Points);

private:

    float epsilonIntersection;
    float epsilonDraw;
    Points bezierCurve;
    QList<QPointF> intersectedPoints;
    QList<Points> selfIntersectionPoints;
    Points controlPoints;

    bool getAngle(Points);
    void bezier(Points, int);
    Points deCasteljau(Points, float);
    QPointF differenceQuotient(Points);

};

#endif // BEZIER_H
