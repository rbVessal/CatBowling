#include "Cube.h"

Cube::Cube(void)
{
	isWall = false;
	initCube(0, 0, 0, 0.5, 0.5, 0.5);
}

Cube::Cube(GLfloat x, GLfloat y, GLfloat z, GLfloat halfX, GLfloat halfY, GLfloat halfZ, bool wall)
{
	isWall = wall;
	initCube(x, y, z, halfX, halfY, halfZ);
}

void Cube::initCube(GLfloat x, GLfloat y, GLfloat z, GLfloat halfX, GLfloat halfY, GLfloat halfZ)
{
	initBoxShape(x, y, z, halfX, halfY, halfZ);
	collider = new AABB(glm::vec3(centerX, centerY, centerZ), halfWidthExtentX, halfWidthExtentY, halfWidthExtentZ);
}

Cube::Cube(const Cube& other)
{
	doCopy(other);
	isWall = other.isWall;
}

const Cube& Cube::operator=(const Cube& other)
{
	if(&other != this)
	{
		doCopy(other);
		isWall = other.isWall;
	}
	return *this;
}

Cube::~Cube(void)
{
	delete vertices;
	delete points;
	delete colors;
	delete collider;
}
