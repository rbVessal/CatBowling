#ifndef A5__CUBE
#define A5__CUBE

#include "BoxShape.h"

class Cube : public BoxShape
{
public:
	Cube(void); // Cube at origin
	Cube(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, bool); // Cube at x,y,z; bool = iswall?
	Cube(const Cube&);
	const Cube& operator=(const Cube&);
	~Cube(void);
private:
	void initCube(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
};
#endif
