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

bool Bezier::boundingOverlap(Points bezier1, Points bezier2){

    QPointF bezMin1 = bezier1.getMinXY();
    QPointF bezMax1 = bezier1.getMaxXY();
    QPointF bezMin2 = bezier2.getMinXY();
    QPointF bezMax2 = bezier2.getMaxXY();

    if(bezMin1.x() < bezMax2.x() && bezMax1.x() > bezMin2.x() &&
       bezMin1.y() < bezMax2.y() && bezMax1.y() > bezMin2.y()){
        return true;
    }
    return false;

}

bool Bezier::getAngle(Points p) {

    Points differential;
    float diffX, diffY;

    for (int i = 0; i < p.getCount() - 1; i++) {
        diffX = p.getPointX(i + 1) - p.getPointX(i);
        diffY = p.getPointY(i + 1) - p.getPointY(i);
        differential.addPoint(diffX, diffY);
    }

    float angle = 0.0, tmpAngle;
    QLineF l1, l2;
    QPointF zero(0, 0);

    for (int i = 0; i < differential.getCount() - 1; i++) {
        l1 = QLineF(zero, differential.getPoint(i));
        l2 = QLineF(zero, differential.getPoint(i + 1));
        tmpAngle = l1.angleTo(l2);

        if (tmpAngle > 180) {
            tmpAngle = 360 - tmpAngle;
        }

        angle += tmpAngle;
        if(angle > 180){
            return true;
        }
    }
    return false;
}

QList<QPointF> Bezier::selfIntersect()
{
    selfIntersect(controlPoints);
    findIntersections();
    return intersectedPoints;
}

void Bezier::selfIntersect(Points bezier){
    bool isIntersect = getAngle(bezier);
    if(isIntersect == true){
        Points splitBezierCurve = deCasteljau(bezier, 0.5);
        int count = splitBezierCurve.getCount() /2;
        selfIntersect(splitBezierCurve.getRange(0,count +1));
        selfIntersect(splitBezierCurve.getRange(count, splitBezierCurve.getCount()));
    } else {
        selfIntersectionPoints.append(bezier);
    }
}

void Bezier::findIntersections(){

    for(int i = 0; i < selfIntersectionPoints.size() - 1; i++){
        for(int j = i+1; j < selfIntersectionPoints.size(); j++){
            intersectBezier(selfIntersectionPoints[i],selfIntersectionPoints[j]);
        }
    }
    selfIntersectionPoints.clear();
}

QList<QPointF> Bezier::intersectBezier(Points otherBezier) {
    intersectBezier(controlPoints, otherBezier);
    return intersectedPoints;
}

Points Bezier::getControlPoints()
{
    return controlPoints;
}

void Bezier::intersectBezier(Points bezier1, Points bezier2){

    if(boundingOverlap(bezier1, bezier2)){
        int m = bezier1.getCount() - 1;
        int n = bezier2.getCount() - 1;
        QPointF diff1 = differenceQuotient(bezier1);
        QPointF diff2 = differenceQuotient(bezier2);

        if(((m * (m - 1) * diff1.x()) > epsilonIntersection) &&
           ((m * (m - 1) * diff1.y()) > epsilonIntersection)){
            Points a = deCasteljau(bezier1, 0.5);
            int center = a.getCount() /2;
            intersectBezier(a.getRange(0,center + 1), bezier2);
            intersectBezier(a.getRange(center,a.getCount()), bezier2);
        }
        else if(((n * (n - 1) * diff2.x()) > epsilonIntersection) &&
                ((n * (n - 1) * diff2.y()) > epsilonIntersection)){
            Points d = deCasteljau(bezier2, 0.5);
            int center = d.getCount() /2;
            intersectBezier(bezier1, d.getRange(0,center + 1));
            intersectBezier(bezier1, d.getRange(center,d.getCount()));
        }
        else{
            Points test = deCasteljau(bezier1, 0.5);
            QPointF intersection;
            QLineF line1 = QLineF(bezier1.getPoint(0), bezier1.getPoint(bezier1.getCount()-1));
            QLineF line2 = QLineF(bezier2.getPoint(0), bezier2.getPoint(bezier2.getCount()-1));
            QLineF::IntersectType result = line1.intersect(line2, &intersection);

            if (result == QLineF::BoundedIntersection) {
                intersectedPoints.append(intersection);
            }
        }
    }
}

Points Bezier::getBezierCurve()
{
    return bezierCurve;
}
