#include "Tetrahedron.h"

Tetrahedron::Tetrahedron(void)
{
	initTetrahedron(0, 0, 0);
}

void Tetrahedron::initTetrahedron(GLfloat x, GLfloat y, GLfloat z)
{
	this->centerX = x;
	this->centerY = y;
	this->centerZ = z;
	offsetX = 0;
	offsetY = 0;
	offsetZ = 0;
	NumVertices = 12;
	numberOfTriangles = NumVertices/3;
	vertices = new glm::vec4[4];
	points = new glm::vec4[NumVertices];
	colors = new glm::vec4[NumVertices];
	this->halfWidthExtentX = 0.5;
	this->halfWidthExtentY = 0.5;
	this->halfWidthExtentZ = 0.5;

	// Vertices of a unit Tetrahedron centered at origin, sides aligned with axes
	vertices[0] = glm::vec4( -halfWidthExtentX + centerX, -halfWidthExtentY + centerY,  halfWidthExtentZ + centerZ, 1.0 );//left
	vertices[1] = glm::vec4( halfWidthExtentX + centerX,  -halfWidthExtentY + centerY,  halfWidthExtentZ + centerZ, 1.0 );//right
	vertices[2] = glm::vec4(  0.0 + centerX,  halfWidthExtentY + centerY,  0.0 + centerZ, 1.0 );//top
	vertices[3] = glm::vec4(  0.0 + centerX, -halfWidthExtentY + centerY,  halfWidthExtentZ + centerZ, 1.0 );//back

	collider = new AABB(glm::vec3(x, y, z), halfWidthExtentX * 0.5, halfWidthExtentY, halfWidthExtentZ * 0.5);

	initValues();
}

Tetrahedron::Tetrahedron(GLfloat x, GLfloat y, GLfloat z)
{
	initTetrahedron(x, y, z);
}

Tetrahedron::Tetrahedron(const Tetrahedron& other)
{
	doCopy(other);
}

const Tetrahedron& Tetrahedron::operator=(const Tetrahedron& other)
{
	if(&other != this)
	{
		doCopy(other);
	}
	return *this;
}

Tetrahedron::~Tetrahedron(void)
{
	delete vertices;
	delete points;
	delete colors;
	delete collider;
}

// generate 12 triangles: 36 vertices and 36 colors
void Tetrahedron::draw()
{
    drawTriangles(2, 0, 1);//front face
    drawTriangles(2, 3, 0);//left face
    drawTriangles(2, 3, 1);//right face
    drawTriangles(1, 3, 0);//bottom face
}

void Tetrahedron::drawTriangles( int a, int b, int c )
{
    colors[index] = vertex_colors[a]; points[index] = vertices[a]; index++;
    colors[index] = vertex_colors[b]; points[index] = vertices[b]; index++;
    colors[index] = vertex_colors[c]; points[index] = vertices[c]; index++;
}