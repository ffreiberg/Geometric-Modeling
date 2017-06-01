#include "points.h"

Points::Points()
{
}

Points Points::getRange(int from, int to){
    Points new_list;
    for(int i = from; i < to; i++){
        new_list.addPoint(this->getPointX(i), this->getPointY(i));
    }
    return new_list;
}

void Points::addPoint(float x,float y)
{
    pointlist.append(QPointF(x,y));
}

float Points::getPointX(int i)
{
    return pointlist[i].x();
}

float Points::getPointY(int i)
{
    return pointlist[i].y();
}

void Points::setPointX(int i, float x)
{
    return pointlist[i].setX(x);
}

void Points::setPointY(int i, float y)
{
    return pointlist[i].setY(y);
}

int Points::getCount()
{
    return pointlist.size();
}

QPointF Points::getPoint(int i){
    return pointlist[i];
}

void Points::clear()
{
    pointlist.clear();
}

bool sortByX(const QPointF &pointA, const QPointF &pointB){
    return pointA.x() < pointB.x();
}

void Points::sort()
{
    qSort(pointlist.begin(), pointlist.end(), sortByX);
}

QList<QPointF> Points::getList(){
    return pointlist;
}

QPointF Points::getMinXY(){
    QPointF minXY(1.0,1.0);
    for(int i = 0; i < pointlist.size(); i++){
        minXY.setX(minXY.x() < pointlist[i].x() ? minXY.x() : pointlist[i].x());
        minXY.setY(minXY.y() < pointlist[i].y() ? minXY.y() : pointlist[i].y());
    }
    return minXY;
}

QPointF Points::getMaxXY(){
    QPointF maxXY(-1.0,-1.0);
    for(int i = 0; i < pointlist.size(); i++){
        maxXY.setX(maxXY.x() > pointlist[i].x() ? maxXY.x() : pointlist[i].x());
        maxXY.setY(maxXY.y() > pointlist[i].y() ? maxXY.y() : pointlist[i].y());
    }
    return maxXY;
}

int Points::getClosestPoint(float x, float y)
{
    int result = 0;
    QPointF testpnt = QPointF(x,y);
    QPointF distpnt = pointlist.first()-testpnt;
    float distance = distpnt.x()*distpnt.x() + distpnt.y() * distpnt.y();
    float mindist = distance;
    for (int i = 0; i < pointlist.size(); i++) {
        QPointF& pnt = pointlist[i];
        distpnt = pnt-testpnt;
        distance = distpnt.x()*distpnt.x() + distpnt.y() * distpnt.y();
        if (distance < mindist) {
            mindist = distance;
            result = i;
        }
    }
    return result;
}

void Points::moveClosestPoint(float x, float y, float tox, float toy)
{
    int pntnum = getClosestPoint(x,y);
    pointlist[pntnum].setX(tox);
    pointlist[pntnum].setY(toy);
}
