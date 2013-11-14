#ifndef A5__POLYH
#define A5__POLYH

#include "Angel.h"
#include <cstdlib>

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

struct AABB
{
	vec3 centerPoint;
	float halfWidthExtents[3];

	//AABB collison detection
	//see: Real Time Collision book 4.2 Axis-Aligned Bounding Boxes (AABBs)
	bool checkAABB(AABB other)
	{
		if(abs(this->centerPoint.x - other.centerPoint.x) > (this->halfWidthExtents[0] + other.halfWidthExtents[0]))
		{
			return false;
		}
		if(abs(this->centerPoint.y - other.centerPoint.y) > (this->halfWidthExtents[1] + other.halfWidthExtents[1]))
		{
			return false;
		}
		if(abs(this->centerPoint.z - other.centerPoint.z) > (this->halfWidthExtents[2] + other.halfWidthExtents[2]))
		{
			return false;
		}
		return true;
	}
};

class Polyhedron
{
public:
	Polyhedron(void); // Polyhedron at origin
	Polyhedron(const Polyhedron&);
	const Polyhedron& operator=(const Polyhedron&);
	~Polyhedron(void);

	// Functions
	void init(GLuint);
	void display();

	AABB getAABB();
	void setVelocity(float x, float y, float z);
	vec3 getVelocity();
	void eulerIntegrationUpdatePosition();
	void move(Polyhedron** polyhedronArray, int size);

protected:
	void doCopy(const Polyhedron&); // used for inherited copy constructors

	GLfloat centerX, centerY, centerZ;
	GLfloat offsetX, offsetY, offsetZ;
	int index;
	int NumVertices;
	int numberOfTriangles;
	GLuint model_view;
	GLuint projection;
	GLuint transformationMatrix;
	GLuint vbo;
	GLuint vao;

	GLfloat radius;
	GLfloat theta;
	GLfloat phi;

	GLfloat  left, right;
	GLfloat  bottom, top;
	GLfloat  zNear, zFar;

	point4* points;
	color4* colors;

	// Vertices of a unit cube centered at origin, sides aligned with axes
	point4* vertices;


	// RGBA colors
	color4 vertex_colors[8];

	//Struct for AABB
	AABB aabb;
	vec3 velocity;
	vec3 acceleration;

	float newRandR, newRandG, newRandB, newRandA;
	
	bool switchColors;
	int timerInterval;
	int previousTime;
	int currentTime;
	
	virtual void drawTriangles(int indice0, int indice1, int indice2, int);
	virtual void draw();

	void setupVBO();
	void setupVAO(GLuint);

	//Use this if you want specific colors changed for a specific polyhedron
	virtual void changeColors();
	
	void initValues();

private:
	GLfloat newX, newY, newZ;
	GLfloat randomNumberR, randomNumberG, randomNumberB, randomNumberA;
	GLuint vColor;
	//Use this if you want all of the polyhedron faces changing colors
	void animateColorsOfFaces();

};
#endif


