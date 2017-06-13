#ifndef BEZIER_H
#define BEZIER_H

#include "points.h"

class Bezier
{
public:

    Bezier(Points, float, float);

    Points bezier();
    Points getBezierCurve();
    Points getControlPoints();
    Points addNewSegment(QPointF);

private:

    float epsilonIntersection;
    float epsilonDraw;
    Points bezierCurve;

    Points controlPoints;

    void bezier(Points, int);
    Points deCasteljau(Points, float);
    QPointF differenceQuotient(Points);

};

#endif // BEZIER_H

