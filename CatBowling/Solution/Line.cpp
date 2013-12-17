#include "Line.h"


Line::Line(void)
{
	isWall = false;
	initBoxShape(0, 0, 0, 0, 0, 0);
	collider = new LineSegment(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
}

Line::Line(const Line& other)
{
	doCopy(other);
	isWall = other.isWall;
}

const Line& Line::operator=(const Line& other)
{
	if(&other != this)
	{
		doCopy(other);
		isWall = other.isWall;
	}
	return *this;
}

Line::Line(glm::vec3 p1, glm::vec3 p2)
{
	isWall = false;

	// Midpoint formula for center
	GLfloat x = (p1.x + p2.x) * 0.5;
	GLfloat y = (p1.y + p2.y) * 0.5;
	GLfloat z = (p1.z + p2.z) * 0.5;

	// Subtract for half-widths
	GLfloat halfX = abs(p1.x - x);
	GLfloat halfY = abs(p1.y - y);
	GLfloat halfZ = abs(p1.z - z);

	// If any of them are small, make them a bit bigger
	if(halfX < 0.1)
	{
		halfX = 0.1;
	}
	if(halfY < 0.1)
	{
		halfY = 0.1;
	}
	if(halfZ < 0.1)
	{
		halfZ = 0.1;
	}

	initBoxShape(x, y, z, halfX, halfY, halfZ);
	collider = new LineSegment(p1, p2);
}

Line::~Line(void)
{
	delete vertices;
	delete points;
	delete colors;
	delete collider;
}
