
#include "stdafx.h"
#include "vec.h"
#include "color.h"
#include "quader.h"
#include "glut.h"


void bhamLineH (CVec2f p1, CVec2f p2, Color c) 
{	// Besenham for lines
	glColor3f (c.r, c.g, c.b);
	
	glBegin (GL_POINTS);	
	glVertex2i(p1(0),p1(1));	// erster  Punkt
	glVertex2i(p2(0),p2(1));	// letzter Punkt
	
	bool bx = (p1(0)>p2(0));
	if (bx) { p1(0) = -p1(0); p2(0) =- p2(0); }

	bool by = (p1(1)>p2(1));
	if (by) { p1(1) = -p1(1); p2(1) = -p2(1); }

	bool a = (p2(0)-p1(0) < p2(1)-p1(1));
	if (a) { p1.swap();	p2.swap(); }

	int dx  = p2(0)-p1(0);
	int dy  = p2(1)-p1(1);
	int dne = 2*(dy-dx);
	int de  = 2*dy;
	int d   = 2*dy-dx;
	CVec2f p3;
	
	while(p1(0)<p2(0)){
		if(d>=0){ d += dne; p1(1)++; }
		else    { d += de;          }
		p1(0)++; 
		if (a) {
			if (bx) p3(0) = -p1(1); else p3(0) = p1(1);
			if (by) p3(1) = -p1(0); else p3(1) = p1(0);
		}
		else {
			if (bx) p3(0) = -p1(0); else p3(0) = p1(0);
			if (by) p3(1) = -p1(1); else p3(1) = p1(1);
		}
		glVertex2i(p3(0),p3(1));
	}
	
	glEnd ();
}

Quader::Quader()
{

}

Quader::Quader(CVec4f l, CVec4f u)
{
	for (int i=0, j=0; i<4; i++, j=(i>1)?2:0) {
		q[2*i  ](0) = l(0); q[i+j  ](1) = l(1); q[i  ](2) = l(2);  q[i  ](3) = 1;
		q[2*i+1](0) = u(0); q[i+j+2](1) = u(1); q[i+4](2) = u(2);  q[i+4](3) = 1;
	}
}

Quader::~Quader()
{

}

void Quader::setData(CVec4f l, CVec4f u)
{
	Quader buf(l,u);
	*this = buf;
}

void Quader::draw(viewSystem &view, Color col)
{
	CMat4f mat = view.getTransform2();

	CVec4f Quader[8];
	for (int i=0; i<8;i++){
		Quader[i] = view.Project(mat*q[i]);
	}	
	bhamLineH(Quader[0],Quader[1],col);
	bhamLineH(Quader[0],Quader[2],col);
	bhamLineH(Quader[0],Quader[4],col);
	bhamLineH(Quader[1],Quader[5],col);
	bhamLineH(Quader[1],Quader[3],col);
	bhamLineH(Quader[2],Quader[3],col);
	bhamLineH(Quader[2],Quader[6],col);
	bhamLineH(Quader[3],Quader[7],col);
	bhamLineH(Quader[4],Quader[5],col);
	bhamLineH(Quader[4],Quader[6],col);
	bhamLineH(Quader[5],Quader[7],col);
	bhamLineH(Quader[6],Quader[7],col);

}

