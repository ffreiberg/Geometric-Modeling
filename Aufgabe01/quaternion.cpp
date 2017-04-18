////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "quaternion.h"
#include "math.h"
#include <iostream>

Quaternion::Quaternion()
{
	for (int i=0; i<4; i++) x[i]=0;	
}

Quaternion::Quaternion(const float &s, const float &a, const float &b, const float &c)
{
	x[0] = s;
	x[1] = a;
	x[2] = b;
	x[3] = c;
}

Quaternion::Quaternion(const float &s, CVec4f &v)
{
	x[0] = s;
	x[1] = v(0);
	x[2] = v(1);
	x[3] = v(2);
}

Quaternion::~Quaternion()
{
}

void Quaternion::rotate(Quaternion q)
{

	Quaternion qRot;

	//q'
	Quaternion q_;
	q_.x[0] = q.x[0];
	q_.x[1] = -q.x[1];
	q_.x[2] = -q.x[2];
	q_.x[3] = -q.x[3];

	// q * p * q'
	qRot = q * (*this) * q_;

	x[0] = 0;
	x[1] = qRot.x[1];
	x[2] = qRot.x[2];
	x[3] = qRot.x[3];
}

void Quaternion::normalize() {

	float d = sqrt(
		x[0] * x[0] + 
		x[1] * x[1] + 
		x[2] * x[2] + 
		x[3] * x[3]
	);

	x[0] /= d;
	x[1] /= d;
	x[2] /= d;
	x[3] /= d;
}

float Quaternion::dot(Quaternion q) {

	float s = .0;

	s += x[0] * q[0];
	s += x[1] * q[1];
	s += x[2] * q[2];
	s += x[3] * q[3];

	return s;
}

float Quaternion::norm() {
	return sqrt(
		x[0] * x[0] +
		x[1] * x[1] +
		x[2] * x[2] +
		x[3] * x[3]
	);
}


Quaternion Quaternion::operator+(const Quaternion& rhs)
{
	Quaternion q;

	q.x[0] = x[0] + rhs.x[0];
	q.x[1] = x[1] + rhs.x[1];
	q.x[2] = x[2] + rhs.x[2];
	q.x[3] = x[3] + rhs.x[3];

	return q;
}

Quaternion Quaternion::operator-(const Quaternion& rhs)
{
	Quaternion q;

	q.x[0] = x[0] - rhs.x[0];
	q.x[1] = x[1] - rhs.x[1];
	q.x[2] = x[2] - rhs.x[2];
	q.x[3] = x[3] - rhs.x[3];

	return q;
}

Quaternion Quaternion::operator*(const Quaternion& rhs)
{
	Quaternion q;
	
	q.x[0] = x[0] * rhs.x[0] - x[1] * rhs.x[1] - x[2] * rhs.x[2] - x[3] * rhs.x[3];
	q.x[1] = x[0] * rhs.x[1] + rhs.x[0] * x[1] + x[2] * rhs.x[3] - rhs.x[2] * x[3];
	q.x[2] = x[0] * rhs.x[2] + rhs.x[0] * x[2] + x[3] * rhs.x[1] - rhs.x[3] * x[1];
	q.x[3] = x[0] * rhs.x[3] + rhs.x[0] * x[3] + x[1] * rhs.x[2] - rhs.x[1] * x[2];

	return q;
}

Quaternion Quaternion::operator*(const int& rhs) {
	
	Quaternion q;

	q.x[0] = x[0] * rhs;
	q.x[1] = x[1] * rhs;
	q.x[2] = x[2] * rhs;
	q.x[3] = x[3] * rhs;

	return q;
}

Quaternion Quaternion::operator*(const float& rhs) {

	Quaternion q;

	q.x[0] = x[0] * rhs;
	q.x[1] = x[1] * rhs;
	q.x[2] = x[2] * rhs;
	q.x[3] = x[3] * rhs;

	return q;
}

float& Quaternion::operator[](int idx)
{
	return x[idx];
}

float Quaternion::operator[](int idx) const
{
	return x[idx];
}

CVec4f Quaternion::toCVector()
{
	return CVec4f(x[0], x[1], x[2], x[3]);
}