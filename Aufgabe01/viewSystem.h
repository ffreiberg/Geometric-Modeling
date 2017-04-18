////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#ifndef _VIEWSYSTEM_H_
#define _VIEWSYSTEM_H_

#include "vec.h"
#include "mat.h"
#include "quaternion.h"

typedef enum MatrixMode {VIEW_MATRIX_MODE, VIEW_FORMULA_MODE, VIEW_QUATERNION_MODE};

class viewSystem {
private:
	float F;
	CVec4f     EyePoint, ViewDir, ViewUp;
	CVec4f     ViewHor; // the view horizon: ViewDir x ViewUp
	MatrixMode RotMode; // Rotation mode: matrix multiplication, matrix formula, quaternions

	CMat4f Inverse(CMat4f M);
	CMat4f RotationMatrix(CVec4f axis, float angle);

public:
	// constructors
	viewSystem();
	viewSystem(const float  eye[4], const float  dir[4], const float  up[4], float f);
	viewSystem(const CVec4f eye,    const CVec4f dir,    const CVec4f up,    float f);

	// destructor
	~viewSystem();

	// getter-/setter-methods
	void setData(const float  eye[4], const float  dir[4], const float  up[4], float f);
	void setData(const CVec4f eye,    const CVec4f dir,    const CVec4f up,    float f);
	void setMode(MatrixMode mode);

	// affine transforms of the view system
	void Zoom     (             float f    );
	void RotateX  (             float angle);	// rotate around the x-axis
	void RotateY  (             float angle);	// rotate around the y-axis
	void RotateZ  (             float angle);	// rotate around the z-axis
	void RotateUp (             float angle);	// rotate around ViewUp
	void RotateDir(             float angle);	// rotate around ViewDir
	void RotateHor(             float angle);	// rotate around ViewHor
	void Rotate   (CVec4f axis, float angle);	// rotate around arbitrary axis
	void Rotate   (CMat4f mat              );	// rotate by rotation matrix mat
	void Rotate   (Quaternion quat         );	// rotate by quaternion quat
	void Translate(CVec4f vec              );	// translate by vec

	// projection methods (in the view system coordinates)
	CVec4f Project(CVec4f Point); // projection in the view system coordinates
	
	// view-to-world/world-to-view transformations
	CMat4f getTransform1();  // view to world
	CMat4f getTransform2();  // world to view

	//quaternion interpolations
	void lerp(	Quaternion, Quaternion, float);
	void slerp(	Quaternion, Quaternion, float);
	void nlerp(	Quaternion, Quaternion, float);
};

#endif