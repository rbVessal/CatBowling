#pragma once
#ifndef _PCONTROL_
#define _PCONTROL_

#include "Polyhedron.h"

class PolyController
{
public:
	PolyController(void);
	PolyController(Polyhedron**, int);
	PolyController(const PolyController&);
	const PolyController& operator=(const PolyController&);
	~PolyController(void);

	void init(GLuint);
	void display();

private:
	Polyhedron** polyArray;
	int sizeOfPolyArray;
};
#endif
