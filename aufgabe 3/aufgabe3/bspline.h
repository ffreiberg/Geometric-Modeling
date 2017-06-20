#ifndef BSPLINE_H
#define BSPLINE_H

#include "knots.h"
// d_0, ..., d_m        = control points
// x_0, ..., x_m+n+1    = knots
// m + 1                = number control points
// n                    = degree

class BSpline
{
public:
    BSpline             ();
    BSpline             (Points, Knots, int, float);
    ~BSpline            ();

    void toBezier       (Points&, Knots&, QList<QList<QPointF>>&);
    int insertKnot      (Points&, Knots&, float);

private:
    void deBoor         (Points&, Knots&, float, int);
    int findInterval    (Points&, Knots&, float);
    Points              bSpline, controlPoints;
    Knots               knots;
    int                 degree;
    float               epsilonDraw;
};

#endif // BSPLINE_H
