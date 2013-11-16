#ifndef A5__OCTAH
#define A5__OCTAH

#include "Polyhedron.h"
class Octahedron : public Polyhedron
{
public:
	Octahedron(void);
	Octahedron(GLfloat, GLfloat, GLfloat);
	Octahedron(const Octahedron&);
	const Octahedron& operator=(const Octahedron&);
	~Octahedron(void);

	// Functions
	void draw();
	void drawTriangles(int, int, int);
private:
	void initOctahedron(GLfloat, GLfloat, GLfloat);
};
#endif
