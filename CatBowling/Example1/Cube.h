#ifndef A5__CUBE
#define A5__CUBE

#include "Polyhedron.h"

class Cube : public Polyhedron
{
public:
	Cube(void); // Cube at origin
	Cube(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, bool); // Cube at x,y,z; bool = iswall?
	Cube(const Cube&);
	const Cube& operator=(const Cube&);
	~Cube(void);

	// Functions
	void draw();
	void drawTriangles(int, int, int, int);
private:
	void initCube(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	bool isWall;
};
#endif
