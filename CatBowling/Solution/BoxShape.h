#ifndef _BOX_SHAPE_
#define _BOX_SHAPE_

#include "polyhedron.h"

// BoxShape is an abstract class that encompases "boxy" polyhedrons, including the Cube and the Line
class BoxShape : public Polyhedron
{
public:
	BoxShape(void);
	BoxShape(const BoxShape&);
	const BoxShape& operator=(const BoxShape&);
	virtual ~BoxShape(void);
	
	// Functions
	void draw();
	void drawTriangles(int, int, int, int);
protected:
	void initBoxShape(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	bool isWall;
};
#endif
