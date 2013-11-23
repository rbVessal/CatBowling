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
	this->centerX = x;
	this->centerY = y;
	this->centerZ = z;
	offsetX = 0;
	offsetY = 0;
	offsetZ = 0;
	NumVertices = 36;
	numberOfTriangles = NumVertices/3;
	vertices = new glm::vec4[8];
	points = new glm::vec4[NumVertices];
	colors = new glm::vec4[NumVertices];

	float halfWidthExtentX = halfX;
	float halfWidthExtentY = halfY;
	float halfWidthExtentZ = halfZ;

	// Vertices of a unit cube centered at origin, sides aligned with axes
	vertices[0] = glm::vec4( -halfWidthExtentX+x, -halfWidthExtentY+y,  halfWidthExtentZ+z, 1.0 );
	vertices[1] = glm::vec4( -halfWidthExtentX+x,  halfWidthExtentY+y,  halfWidthExtentZ+z, 1.0 );
	vertices[2] = glm::vec4(  halfWidthExtentX+x,  halfWidthExtentY+y,  halfWidthExtentZ+z, 1.0 );
	vertices[3] = glm::vec4(  halfWidthExtentX+x, -halfWidthExtentY+y,  halfWidthExtentZ+z, 1.0 );
	vertices[4] = glm::vec4( -halfWidthExtentX+x, -halfWidthExtentY+y, -halfWidthExtentZ+z, 1.0 );
	vertices[5] = glm::vec4( -halfWidthExtentX+x,  halfWidthExtentY+y, -halfWidthExtentZ+z, 1.0 );
	vertices[6] = glm::vec4(  halfWidthExtentX+x,  halfWidthExtentY+y, -halfWidthExtentZ+z, 1.0 );
	vertices[7] = glm::vec4(  halfWidthExtentX+x, -halfWidthExtentY+y, -halfWidthExtentZ+z, 1.0 );

	collider = new AABB(glm::vec3(x, y, z), halfWidthExtentX, halfWidthExtentY, halfWidthExtentZ);

	initValues();

	if(isWall)
	{
		physicsComponent.velocity = glm::vec3(0, 0, 0);
		physicsComponent.acceleration = glm::vec3(0, 0, 0);
	}
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


// generate 12 triangles: 36 vertices and 36 colors
void Cube::draw()
{
	if(!isWall)
	{
		drawTriangles( 1, 0, 3, 2 );
		drawTriangles( 2, 3, 7, 6 );
		drawTriangles( 3, 0, 4, 7 );
		drawTriangles( 6, 5, 1, 2 );
		drawTriangles( 4, 5, 6, 7 );
		drawTriangles( 5, 4, 0, 1 );
	}
}

void Cube::drawTriangles( int a, int b, int c, int d )
{
	colors[index] = vertex_colors[a]; points[index] = vertices[a]; index++;
	colors[index] = vertex_colors[b]; points[index] = vertices[b]; index++;
	colors[index] = vertex_colors[c]; points[index] = vertices[c]; index++;
	colors[index] = vertex_colors[a]; points[index] = vertices[a]; index++;
	colors[index] = vertex_colors[c]; points[index] = vertices[c]; index++;
	colors[index] = vertex_colors[d]; points[index] = vertices[d]; index++;
}