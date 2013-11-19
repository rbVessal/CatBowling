#ifndef A5__POLYH
#define A5__POLYH

#include "Angel.h"
#include <cstdlib>
//see: http://www.g-truc.net/project-0016.html
#include "glm\glm.hpp" //for vectors and matrices
#include "glm\gtc\matrix_transform.hpp" //for both model and view transformations
#include "glm\gtc\type_ptr.hpp"  //for using value_ptr to get an array of floats to pass in to shader

typedef glm::vec4  color4;
//typedef glm::vec4  point4;

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

	// Returns the new velocity to use based on the collision
	// (If no collision, return given velocity)
	vec3 collisionResponseVector(AABB other, vec3 velocity)
	{
		// If collision, calculate a new velocity
		if(checkAABB(other))
		{
			// Get the normal (axis)
			vec3 closestPoint = getClosestPoint(other.centerPoint);
			vec3 normal = getNormal(centerPoint - closestPoint);
			
			//std::cout << closestPoint << std::endl;
			
			velocity = glm::reflect(velocity, normal);
		}
		return velocity;
	}

	vec3 getClosestPoint(vec3 otherPoint)
	{
		vec3 point = centerPoint;

		// Distance between aabb centers
		vec3 dist = otherPoint - point;

		// World axes
		vec3 axes[3];
		axes[0] = vec3(1, 0, 0);
		axes[1] = vec3(0, 1, 0);
		axes[2] = vec3(0, 0, 1);
	
		// For each world axis
		for(int i=0; i<3; i++)
		{
			// Use dot product for distance
			float distance = dot(dist, axes[i]);

			// If distance is farther than the box extends, clamp to the box
			if(distance > halfWidthExtents[i])
			{
				distance = halfWidthExtents[i];
			}
			if(distance < halfWidthExtents[i] * -1)
			{
				distance = halfWidthExtents[i] * -1;
			}

			// Step the distance along the world coordinate
			point += distance * axes[i];
		}

		return point;
	}

	vec3 getNormal(vec3 dist)
	{
		vec3 normal;
		float dotProd[6];

		// World axes
		dotProd[0] = dot(dist, vec3(1, 0, 0));
		dotProd[1] = dot(dist, vec3(0, 1, 0));
		dotProd[2] = dot(dist, vec3(0, 0, 1));

		// Reversed axes
		dotProd[3] = dot(dist, vec3(-1, 0, 0));
		dotProd[4] = dot(dist, vec3(0, -1, 0));
		dotProd[5] = dot(dist, vec3(0, 0, -1));

		// Square instead of using absolute value
		for(int i=0; i<6; i++)
		{
			dotProd[i] *= dotProd[i];
		}

		// Find the smallest
		float temp = dotProd[0];
		int index = 0;
		for(int i=0; i<6; i++)
		{
			if(temp > dotProd[i])
			{
				temp = dotProd[i];
				index = i;
			}
		}

		switch(index)
		{
			case 0: normal = vec3(1, 0, 0); break;
			case 1: normal = vec3(0, 1, 0); break;
			case 2: normal = vec3(0, 0, 1); break;
			case 3: normal = vec3(-1, 0, 0); break;
			case 4: normal = vec3(0, -1, 0); break;
			case 5: normal = vec3(0, 0, -1); break;
			default: normal = vec3(0, 0, 0); break;
		}

		return normal;
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

	glm::vec4* points;
	color4* colors;

	// Vertices of a unit cube centered at origin, sides aligned with axes
	glm::vec4* vertices;


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


