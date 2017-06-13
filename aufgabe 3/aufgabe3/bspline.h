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
    BSpline         ();
    BSpline         (Points, Knots, float);
    ~BSpline        ();

    void deBoor     ();
private:
    Points          bSpline, controlPoints;
    Knots           knots;
    float           epsilonDraw;
};

#endif // BSPLINE_H
