#include "bspline.h"
#include <QDebug>

BSpline::BSpline()
{

}

BSpline::BSpline(int degree, float epsilonDraw)
{
    this->degree = degree;
    this->epsilonDraw = epsilonDraw;
}

BSpline::~BSpline()
{

}

void BSpline::toBezier(Points& _points, Knots& _knots, QList<QList<QPointF>>& _bezier)
{
    for (int i = _points.getCount(); i >= this->degree; --i) {
        for (int j = 0; j < this->degree - 1; ++j) {
            insertKnot(_points, _knots, _knots.getValue(i));
            _knots.insertKnot(_knots.getValue(i));
        }
    }

    for (int i = this->degree - 1; i < _points.getCount() - this->degree; i += this->degree) {
        QList<QPointF> bezierPart;
        for (int j = 0; j <= this->degree; ++j) {
            bezierPart.append(QPointF(_points.getPointX(i + j), _points.getPointY(i + j)));
        }
        _bezier.append(bezierPart);
    }
}

int BSpline::insertKnot(Points& _points, Knots& _knots, float t)
{
    int r = findInterval(_points, _knots, t);

    if(r == -1) {
        return r;
    }

    int d = deBoor(_points, _knots, t, r);

    return d;
}

int BSpline::findInterval(Points& _points, Knots& _knots, float t)
{

    int r = _knots.getCount() - 1;

    while(r > 0 && _knots.getValue(r) > t) {
        --r;
    }
    while(r > _points.getCount() && _knots.getValue((r - 1)) == _knots.getValue(r)) {
        --r;
    }

    return r;
}

int BSpline::deBoor(Points& _points, Knots& _knots, float t, int r)
{
    Points newControlPoints;

    int n = this->degree;

    for(int _i = r - n; _i < r; ++_i) {
        int i = _i + 1;
        if(i <= 0 || (i + n + 1) >= _knots.getCount() - 1 /*|| i >= _points.getCount()*/) {
            return -1;
        }
        float x_i = _knots.getValue(i);
        float x_ni = _knots.getValue(n + i);
        float a_i = .0;

        if((x_ni - x_i) > this->epsilonDraw) {
            a_i = (t - x_i) / (x_ni - x_i);
        }

        if(_i >= (_points.getCount() - 1)) {
            newControlPoints.addPoint(_points.getPointX(_i), _points.getPointY(_i));
        } else {
            float x = _points.getPointX(_i) * (1 - a_i) + _points.getPointX(i) * a_i;
            float y = _points.getPointY(_i) * (1 - a_i) + _points.getPointY(i) * a_i;
            newControlPoints.addPoint(x,y);
        }
    }

    int rn1 = r - n + 1;
    for(int i = rn1; i < r; ++i) {
        _points.removeAt(rn1);
    }

    int j = 0;
    for(int i = rn1; j < newControlPoints.getCount(); ++i, ++j) {
        _points.insert(i, QPointF(newControlPoints.getPointX(j), newControlPoints.getPointY(j)));
    }

//    qDebug() << "##### DEBOOR: #####";
//    for (int i = 0; i < _points.getCount(); ++i) {
//        qDebug() << _points.getPoint(i);
//    }
    return 0;
}
