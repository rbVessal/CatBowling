#include "Octahedron.h"

Octahedron::Octahedron(void)
{
	initOctahedron(0, 0, 0);
}


void Octahedron::initOctahedron(GLfloat x, GLfloat y, GLfloat z)
{
	this->centerX = x;
	this->centerY = y;
	this->centerZ = z;
	offsetX = 0;
	offsetY = 0;
	offsetZ = 0;
	NumVertices = 24;
	numberOfTriangles = NumVertices/3;
	vertices = new glm::vec4[6];
	points = new glm::vec4[NumVertices];
	colors = new glm::vec4[NumVertices];
	this->halfWidthExtentX = 0.5;
	this->halfWidthExtentY = 0.5;
	this->halfWidthExtentZ = 0.5;

	// Vertices of a unit Octahedron centered at origin, sides aligned with axes
	vertices[0] = glm::vec4(  0.0 + centerX,  halfWidthExtentY + centerY,  0.0 + centerZ, 1.0 );//top
	vertices[1] = glm::vec4( -halfWidthExtentX + centerX, 0.0 + centerY,  0.0 + centerZ, 1.0 );//left
	vertices[2] = glm::vec4( 0.0 + centerX,  0.0 + centerY,  halfWidthExtentZ + centerZ, 1.0);//front
	vertices[3] = glm::vec4( halfWidthExtentX + centerX, 0.0 + centerY, 0.0 + centerZ, 1.0);//right
	vertices[4] = glm::vec4(  0.0 + centerX, 0.0 + centerY,  -halfWidthExtentZ + centerZ, 1.0 );//back
	vertices[5] = glm::vec4(0.0 + centerX, -halfWidthExtentY + centerY, 0.0 + centerZ, 1.0);//bottom
	
	collider = new AABB(glm::vec3(x, y, z), 0.5, 0.5, 0.5);

	initValues();
}

Octahedron::Octahedron(GLfloat x, GLfloat y, GLfloat z)
{
	initOctahedron(x, y, z);
}

Octahedron::Octahedron(const Octahedron& other)
{
	doCopy(other);
}

const Octahedron& Octahedron::operator=(const Octahedron& other)
{
	if(&other != this)
	{
		doCopy(other);
	}
	return *this;
}

Octahedron::~Octahedron(void)
{
	delete vertices;
	delete points;
	delete colors;
	delete collider;
}

void Octahedron::draw()
{
    drawTriangles(0, 1, 2);//left face
    drawTriangles(2, 3, 0);//right face
    drawTriangles(0, 3, 4);//back right face
    drawTriangles(4, 1, 0);//back left face
	drawTriangles(1, 5, 2);//bottom front left face
	drawTriangles(2, 5, 3);//bottom front right face
	drawTriangles(3, 5, 4);//bottom back right face
	drawTriangles(4, 5, 1);//bottom back left face
}

void Octahedron::drawTriangles( int a, int b, int c )
{
    colors[index] = vertex_colors[a]; points[index] = vertices[a]; index++;
    colors[index] = vertex_colors[b]; points[index] = vertices[b]; index++;
    colors[index] = vertex_colors[c]; points[index] = vertices[c]; index++;
}