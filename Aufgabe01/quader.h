////////////////////////////////////////////////////////////////////
//
//	Georg Umlauf, (c) 2012
//
////////////////////////////////////////////////////////////////////
#ifndef _QUADER_H
#define _QUADER_H

#include "color.h"
#include "viewSystem.h"

void bhamLineH(CVec2f, CVec2f, Color);

class Quader {
private:
	CVec4f q[8];

public:
	Quader();
	Quader(CVec4f l, CVec4f u); // Lower left and upper right corner
	~Quader();

	void setData(CVec4f l, CVec4f u);
	void draw(viewSystem &view, Color col);
};


#endif