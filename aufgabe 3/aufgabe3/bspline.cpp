#include "bspline.h"

BSpline::BSpline()
{

}

BSpline::BSpline(Points controlPoints, Knots knots, float epsilonDraw)
{
    this->controlPoints = controlPoints;
    this->epsilonDraw = epsilonDraw;
    this->knots = knots;
}

BSpline::~BSpline()
{

}

void BSpline::deBoor(){

}
