#include "BoxShape.h"

BoxShape::BoxShape(void)
{
}

void BoxShape::initBoxShape(GLfloat x, GLfloat y, GLfloat z, GLfloat halfX, GLfloat halfY, GLfloat halfZ)
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

	this->halfWidthExtentX = halfX;
	this->halfWidthExtentY = halfY;
	this->halfWidthExtentZ = halfZ;

	// Vertices of a unit BoxShape centered at origin, sides aligned with axes
	vertices[0] = glm::vec4( -halfWidthExtentX + centerX, -halfWidthExtentY + centerY,  halfWidthExtentZ + centerZ, 1.0 );
	vertices[1] = glm::vec4( -halfWidthExtentX + centerX,  halfWidthExtentY + centerY,  halfWidthExtentZ + centerZ, 1.0 );
	vertices[2] = glm::vec4(  halfWidthExtentX + centerX,  halfWidthExtentY + centerY,  halfWidthExtentZ + centerZ, 1.0 );
	vertices[3] = glm::vec4(  halfWidthExtentX + centerX, -halfWidthExtentY + centerY,  halfWidthExtentZ + centerZ, 1.0 );
	vertices[4] = glm::vec4( -halfWidthExtentX + centerX, -halfWidthExtentY + centerY, -halfWidthExtentZ + centerZ, 1.0 );
	vertices[5] = glm::vec4( -halfWidthExtentX + centerX,  halfWidthExtentY + centerY, -halfWidthExtentZ + centerZ, 1.0 );
	vertices[6] = glm::vec4(  halfWidthExtentX + centerX,  halfWidthExtentY + centerY, -halfWidthExtentZ + centerZ, 1.0 );
	vertices[7] = glm::vec4(  halfWidthExtentX + centerX, -halfWidthExtentY + centerY, -halfWidthExtentZ + centerZ, 1.0 );

	initValues();

	if(isWall)
	{
		physicsComponent.velocity = glm::vec3(0, 0, 0);
		physicsComponent.acceleration = glm::vec3(0, 0, 0);
	}
}

BoxShape::BoxShape(const BoxShape& other)
{
	doCopy(other);
	isWall = other.isWall;
}

const BoxShape& BoxShape::operator=(const BoxShape& other)
{
	if(&other != this)
	{
		doCopy(other);
		isWall = other.isWall;
	}
	return *this;
}

BoxShape::~BoxShape()
{
}

// generate 12 triangles: 36 vertices and 36 colors
void BoxShape::draw()
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

void BoxShape::drawTriangles( int a, int b, int c, int d )
{
	colors[index] = vertex_colors[a]; points[index] = vertices[a]; index++;
	colors[index] = vertex_colors[b]; points[index] = vertices[b]; index++;
	colors[index] = vertex_colors[c]; points[index] = vertices[c]; index++;
	colors[index] = vertex_colors[a]; points[index] = vertices[a]; index++;
	colors[index] = vertex_colors[c]; points[index] = vertices[c]; index++;
	colors[index] = vertex_colors[d]; points[index] = vertices[d]; index++;
}