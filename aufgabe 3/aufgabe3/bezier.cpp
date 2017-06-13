#include "bezier.h"
#include <math.h>
#include <QtGui>

Bezier::Bezier(Points controlPoints, float epsilonIntersection, float epsilonDraw)
{
    this->controlPoints = controlPoints;
    this->epsilonDraw = epsilonDraw;
    this->epsilonIntersection = epsilonIntersection;
}

void Bezier::bezier(Points controlPoints, int k)
{
    int n = controlPoints.getCount() - 1;
    QPointF diff = differenceQuotient(controlPoints);
    // Check if line is straight or k is 0
    if((((n * (n - 1) * diff.x()) < epsilonDraw) &&
       ((n * (n - 1) * diff.y()) < epsilonDraw)) ||
       k == 0){
        bezierCurve.addPoint(controlPoints.getPointX(0),controlPoints.getPointY(0));
        bezierCurve.addPoint(controlPoints.getPointX(n),controlPoints.getPointY(n));

    } else {
        // split curve in 2 curves
        Points newPoints = deCasteljau(controlPoints, 0.5);
        int count = newPoints.getCount() / 2;
        bezier(newPoints.getRange(0, count + 1), k - 1);
        bezier(newPoints.getRange(count, newPoints.getCount()), k - 1);
    }
}

Points Bezier::bezier()
{
    this->bezier(controlPoints, controlPoints.getCount());
    return bezierCurve;
}

QPointF Bezier::differenceQuotient(Points bezier)
{
    QPointF result(0.0,0.0);

    for(int i = 0; i < bezier.getCount() - 2; i++){
        float x = fabs(bezier.getPointX(i + 2) - 2 * bezier.getPointX(i + 1) + bezier.getPointX(i));
        float y = fabs(bezier.getPointY(i + 2) - 2 * bezier.getPointY(i + 1) + bezier.getPointY(i));
        result.setX(result.x() > x ? result.x() : x);
        result.setY(result.y() > y ? result.y() : y);
    }

    return result;
}

Points Bezier::deCasteljau(Points polygon, float t)
{
    Points nextPoly;

    for(int i = 0; i < polygon.getCount() - 1; i++){
        float x = polygon.getPointX(i) * (1 - t) + polygon.getPointX(i + 1) * t;
        float y = polygon.getPointY(i) * (1 - t) + polygon.getPointY(i + 1) * t;
        nextPoly.addPoint(x,y);
    }

    if(polygon.getCount() == 1){
        return polygon;
    } else {
        Points result = deCasteljau(nextPoly, t);
        if (t == 0.5 || t == 2){
            Points finalResult;

            finalResult.addPoint(polygon.getPointX(0), polygon.getPointY(0));

            for(int i = 0; i < result.getCount(); i++){
                finalResult.addPoint(result.getPointX(i), result.getPointY(i));
            }

            int count = polygon.getCount()-1;

            finalResult.addPoint(polygon.getPointX(count), polygon.getPointY(count));

            return finalResult;
        }
        return result;
    }
}

Points Bezier::getControlPoints()
{
    return controlPoints;
}

Points Bezier::getBezierCurve()
{
    return bezierCurve;
}

Points Bezier::addNewSegment(QPointF newPoint){
    Points curve = deCasteljau(controlPoints, 2);
    Points newSegment;
    QPointF point = controlPoints.getPoint(controlPoints.getCount()-1);
    newSegment.addPoint(point.x(), point.y());
    int count = curve.getCount() / 2;
    for(int i = curve.getCount() - 2; i > count; i--){
        newSegment.addPoint(curve.getPointX(i), curve.getPointY(i));
    }
    newSegment.addPoint(newPoint.x(), newPoint.y());

    this->bezierCurve.clear();
    bezier(newSegment, 5);

    return newSegment;
}
