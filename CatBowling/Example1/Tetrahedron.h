#ifndef A5__TETRAH
#define A5__TETRAH

#include "Polyhedron.h"
class Tetrahedron : public Polyhedron
{
public:
	Tetrahedron(void);
	Tetrahedron(GLfloat, GLfloat, GLfloat);
	~Tetrahedron(void);

	// Functions
	void draw();
	void drawTriangles(int, int, int);
private:
	void initTetrahedron(GLfloat, GLfloat, GLfloat);
};
#endif
