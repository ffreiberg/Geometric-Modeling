////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "viewSystem.h"
#include <iostream>

viewSystem::viewSystem()
{
	RotMode = VIEW_MATRIX_MODE;
}

viewSystem::viewSystem(const float eye[4], const float dir[4], const float up[4], float f)
{
	F = f;

	EyePoint.setData(eye);
	ViewDir .setData(dir);
	ViewUp  .setData(up);

	RotMode = VIEW_MATRIX_MODE;
}

viewSystem::viewSystem(const CVec4f eye, const CVec4f dir, const CVec4f up, float f)
{
	F        = f;
	EyePoint = eye;
	ViewDir  = dir;
	ViewUp   = up;
	RotMode  = VIEW_MATRIX_MODE;
}

viewSystem::~viewSystem()
{
}

void viewSystem::setData(const float eye[4], const float dir[4], const float up[4], float f)
{
	viewSystem buf(eye,dir,up,f);
	*this = buf;
}

void viewSystem::setData(const CVec4f eye, const CVec4f dir, const CVec4f up, float f)
{
	viewSystem buf(eye,dir,up,f);
	*this = buf;
}

void viewSystem::setMode(MatrixMode mode)
{
	RotMode = mode;
}
void viewSystem::Zoom(float f)
{
	F+=f;
}

void viewSystem::RotateX(float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		{
			CMat4f Rot;
			float c = cos(angle);
			float s = sin(angle);

			Rot(1,1) = c; Rot(1,2) = -s; 
			Rot(2,1) = s; Rot(2,2) =  c;

			Rotate(Rot);break;
		}
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02
			float c = cos(angle / 2);
			float s = sin(angle / 2);

			Quaternion Rot(c, CVec4f(s, 0, 0, 0));
			Rotate(Rot);

			break;
		}
	}	
}

void viewSystem::RotateY(float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		{
			CMat4f Rot;
			float c = cos(angle);
			float s = sin(angle);

			Rot(0,0) =  c; Rot(0,2) = s; 
			Rot(2,0) = -s; Rot(2,2) = c; 
			
			Rotate(Rot);break;
		}
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02
			float c = cos(angle / 2);
			float s = sin(angle / 2);

			Quaternion Rot(c, CVec4f(0, s, 0, 0));
			Rotate(Rot);
			break;
		}
	}	
}

void viewSystem::RotateZ(float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		{
			CMat4f Rot;
			float c = cos(angle);
			float s = sin(angle);

			Rot(0,0) = c; Rot(0,1) = -s; 
			Rot(1,0) = s; Rot(1,1) =  c; 
			
			Rotate(Rot);
			break;
		}
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02
			float c = cos(angle / 2);
			float s = sin(angle / 2);

			Quaternion Rot(c, CVec4f(0, 0, s, 0));
			Rotate(Rot);
			break;
		}
	}	
}

void viewSystem::RotateDir(float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		ViewUp = RotationMatrix(ViewDir,angle)*ViewUp;
		break;
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02
			float c = cos(angle / 2);
			float s = sin(angle / 2);
			Quaternion d(c, ViewDir * s);
			Quaternion u(0, ViewUp);
			u.rotate(d);
			ViewUp = u.toCVector();
			break;
		}
	}	
	ViewUp.normalize();
}

void viewSystem::RotateUp(float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		ViewDir = RotationMatrix(ViewUp,angle)*ViewDir;
		break;
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02
			float c = cos(angle / 2);
			float s = sin(angle / 2);
			Quaternion u(c, ViewUp * s);
			Quaternion d(0, ViewDir);
			d.rotate(u);
			ViewDir = d.toCVector();
			break;
		}
	}	
	ViewDir.normalize();
}

void viewSystem::RotateHor(float angle)
{
	ViewHor = ViewDir.cross(ViewUp);
	ViewHor.normalize();
	
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		{
			CMat4f Rot = RotationMatrix(ViewHor,angle);
			ViewUp     = Rot*ViewUp;  
			ViewDir    = Rot*ViewDir; 
		}
	case VIEW_QUATERNION_MODE: 
		{
			// AUFGABE02
			float c = cos(angle / 2);
			float s = sin(angle / 2);
			Quaternion h(c, ViewHor * s);
			Quaternion d(0, ViewDir);
			Quaternion u(0, ViewUp);
			d.rotate(h);
			u.rotate(h);
			ViewDir = d.toCVector();
			ViewUp = u.toCVector();
			break;
		}
	}
	ViewUp .normalize();
	ViewDir.normalize();
}

void viewSystem::Rotate(CMat4f mat)
{
	EyePoint = mat * EyePoint;
	ViewDir  = mat * ViewDir;
	ViewUp   = mat * ViewUp;

	ViewDir.normalize();
	ViewUp .normalize();
}

void viewSystem::Rotate(Quaternion q) {
	
	Quaternion qRotE, qRotU, qRotD, q_;

	Quaternion pD(0, ViewDir);
	Quaternion pE(0, EyePoint);
	Quaternion pU(0, ViewUp);

	// q'
	q_[0] = q[0];
	q_[1] = -q[1];
	q_[2] = -q[2];
	q_[3] = -q[3];

	// q * p * q'
	qRotD = q * pD * q_;
	qRotE = q * pE * q_;
	qRotU = q * pU * q_;

	EyePoint(0) = qRotE[1]; EyePoint(1) = qRotE[2]; EyePoint(2) = qRotE[3];
	ViewDir(0) = qRotD[1]; ViewDir(1) = qRotD[2]; ViewDir(2) = qRotD[3];
	ViewUp(0) = qRotU[1]; ViewUp(1) = qRotU[2]; ViewUp(2) = qRotU[3];
}

void viewSystem::Rotate(CVec4f axis, float angle)
{
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 	
	case VIEW_MATRIX_MODE:  
		Rotate(RotationMatrix(axis,angle));
		break;
	case VIEW_QUATERNION_MODE: 
		// AUFGABE02
		break;
	}
}

void viewSystem::Translate(CVec4f vec)
{
	EyePoint += vec;
}

CVec4f viewSystem::Project(CVec4f Point)
{	// central projection of Point
	CMat4f M;
	M(2,2) = 0.0;
	M(3,2) = -(1/F);
	Point  = M*(Point*(1/(1-Point(2)/F)));	// re-normalization
	return Point;
}

CMat4f viewSystem::getTransform1()
{
	CMat4f M; 
	// AUFGABE01
	
	CVec4f rx, ry, rz;

	rx = ViewUp.cross(ViewDir) * (1 / (ViewUp.cross(ViewDir)).norm());

	ry = ViewUp * (1 / ViewUp.norm());

	rz = ViewDir * (1 / ViewDir.norm());

	float trans[4][4] = {	
		{ 1, 0, 0, -EyePoint(0) },
		{ 0, 1, 0, -EyePoint(1) },
		{ 0, 0, 1, -EyePoint(2) },
		{ 0, 0, 0, 1 } 
	};

	float rot[4][4] = {
		{ rx(0), rx(1), rx(2), 0 },
		{ ry(0), ry(1), ry(2), 0 },
		{ rz(0), rz(1), rz(2), 0 },
		{ 0, 0, 0, 1 } 
	};

	CMat4f transMat(trans);
	CMat4f transRot(rot);

	M = transRot * transMat;

	return M;
}

CMat4f viewSystem::getTransform2()
{
	CMat4f M; 
	// AUFGABE01
	M = Inverse(getTransform1());

	return M;
}

CMat4f viewSystem::RotationMatrix(CVec4f axis, float angle)
{
	CMat4f Rot;
	
	switch (RotMode) {
	case VIEW_FORMULA_MODE: 
		{
			float c = cos(angle);
			float s = sin(angle);

			Rot(0,0) = axis(0)*axis(0)*(1-c) +         c; 
			Rot(0,1) = axis(0)*axis(1)*(1-c) - axis(2)*s; 
			Rot(0,2) = axis(0)*axis(2)*(1-c) + axis(1)*s; 
			Rot(1,0) = axis(1)*axis(0)*(1-c) + axis(2)*s;
			Rot(1,1) = axis(1)*axis(1)*(1-c) +         c;
			Rot(1,2) = axis(1)*axis(2)*(1-c) - axis(0)*s;
			Rot(2,0) = axis(2)*axis(0)*(1-c) - axis(1)*s; 
			Rot(2,1) = axis(2)*axis(1)*(1-c) + axis(0)*s; 
			Rot(2,2) = axis(2)*axis(2)*(1-c) +         c;
		
			break;
		}
	case VIEW_MATRIX_MODE: 
		{
			float d = sqrt(axis(0)*axis(0)+axis(1)*axis(1));
			float a = axis(0)/d;
			float b = axis(1)/d;

			CMat4f Rot1;
			if (d) {
				Rot1(0,0) = a; Rot1(1,0) = -b;
				Rot1(0,1) = b; Rot1(1,1) =  a;
			}

			CMat4f Rot2;
			Rot2(0,0) =  axis(2); Rot2(2,0) =  d;
			Rot2(0,2) = -d;       Rot2(2,2) =  axis(2);

			CMat4f Rot3;
			Rot3(0,0) =  cos(angle); Rot3(1,0) =  sin(angle);
			Rot3(0,1) = -sin(angle); Rot3(1,1) =  cos(angle);
	
			CMat4f Rot4(Rot2);
			Rot4(2,0) = Rot2(0,2); Rot4(0,2) = Rot2(2,0);

			CMat4f Rot5(Rot1);
			Rot5(1,0) = Rot1(0,1); Rot5(0,1) = Rot1(1,0); 
	
			Rot = Rot5*Rot4*Rot3*Rot2*Rot1;
			break;
		}
	case VIEW_QUATERNION_MODE:
		std::cerr << "Error in rotation mode: For quaternions no rotation matrix required.\n";
		break;
	}
	
	return Rot;
}

CMat4f viewSystem::Inverse(CMat4f M)
// Berechnet die Inverse einer 4x4 Matrix M der Art 
//             | A a |
//             | 0 1 |
// mit einer orthonormalen 3x3 Matrix A. 
// Die Inverse ist dann gegeben durch
//             | A^T -A^t*a |
//             |  0     1   |
{
	float Mat[4][4];
	CVec4f a;
	for (int i=0; i<4; i++) for (int j=0; j<4; j++) Mat[i][j]=M(i,j);
	for (int i=0; i<3; i++) { Mat[i][3]=0; a(i)=M(i,3); }
	for (int i=0; i<3; i++) for (int j=0; j<i; j++) { float x=Mat[i][j]; Mat[i][j]=Mat[j][i]; Mat[j][i]=x; }
	CMat4f tmp(Mat);
	a = tmp*a;
	for (int i=0; i<3; i++) tmp(i,3)=-a(i);

	return tmp;
}
