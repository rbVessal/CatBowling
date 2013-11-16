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
	vertices = new point4[6];
	points = new point4[NumVertices];
	colors = new point4[NumVertices];

	// Vertices of a unit Octahedron centered at origin, sides aligned with axes
	vertices[0] = point4(  0.0 + x,  0.5 + y,  0.0 + z, 1.0 );//top
	vertices[1] = point4( -0.5 + x, 0.0 + y,  0.0 + z, 1.0 );//left
    vertices[2] = point4( 0.0 + x,  0.0 + y,  0.5 + z, 1.0);//front
	vertices[3] = point4( 0.5 + x, 0.0 + y, 0.0 + z, 1.0);//right
    vertices[4] = point4(  0.0 + x, 0.0 + y,  -0.5 + z, 1.0 );//back
	vertices[5] = point4(0.0 + x, -0.5 + y, 0.0 + z, 1.0);//bottom
	
	aabb.centerPoint = vec3(x, y, z);
	aabb.halfWidthExtents[0] = 0.5;
	aabb.halfWidthExtents[1] = 0.5;
	aabb.halfWidthExtents[2] = 0.5;

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