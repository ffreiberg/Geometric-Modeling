////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "vec.h"

class Quaternion {
private:
	float x[4];

public:
	// constructors/destructors
	Quaternion();									// default constructor
	Quaternion(const float &, CVec4f &);
	Quaternion(const float &, const float &, const float &, const float &);
	~Quaternion();

	void rotate(Quaternion);
	CVec4f toCVector();

	Quaternion operator+(const Quaternion&);
	Quaternion operator*(const Quaternion&);
	float& operator[](int);
	float operator[](int) const;
};


#endif