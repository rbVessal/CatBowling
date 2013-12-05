#ifndef A5__POLYH
#define A5__POLYH

#include "AABB.h"

//see: http://www.g-truc.net/project-0016.html
#include "glm\gtc\matrix_transform.hpp" //for both model and view transformations
#include "glm\gtc\type_ptr.hpp"  //for using value_ptr to get an array of floats to pass in to shader
#include "glm\gtx\transform2.hpp" //for shearing see: http://glm.g-truc.net/0.9.0/api/a00193.html

typedef glm::vec4  color4;
//typedef glm::vec4  point4;

struct PhysicsComponent
{
	float mass;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	// TODO: implement forces and linear momentum
	void addForce(GLfloat forceX, GLfloat forceY, GLfloat forceZ)
	{
		
	}
};

class Polyhedron
{
public:
	Polyhedron(void); // Polyhedron at origin
	Polyhedron(const Polyhedron&);
	const Polyhedron& operator=(const Polyhedron&);
	virtual ~Polyhedron(void);

	// Functions
	void init(GLuint);
	void display();

	Collider* getCollider();
	void setVelocity(float x, float y, float z);
	glm::vec3 getVelocity();
	void eulerIntegrationUpdatePosition();
	void move(Polyhedron** polyhedronArray, int size);

protected:
	void doCopy(const Polyhedron&); // used for inherited copy constructors

	GLfloat centerX, centerY, centerZ;
	GLfloat offsetX, offsetY, offsetZ;
	int index;
	int NumVertices;
	int numberOfTriangles;
	GLuint view;
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

	// Components
	Collider* collider;
	PhysicsComponent physicsComponent;

	float newRandR, newRandG, newRandB, newRandA;
	
	bool switchColors;
	int timerInterval;
	int previousTime;
	int currentTime;

	//Composite Model Transformation Matrix
	glm::mat4 compositeModelTransformationMatrix;
	
	virtual void drawTriangles(int indice0, int indice1, int indice2, int){ }
	virtual void draw(){ }

	void setupVBO();
	void setupVAO(GLuint);

	//Use this if you want specific colors changed for a specific polyhedron
	virtual void changeColors();

	//For clearing out the composite model transformation matrix after it has been applied to the shader
	void clearCompositeModelTransformationMatrix();
	
	void initValues();

private:
	GLfloat randomNumberR, randomNumberG, randomNumberB, randomNumberA;
	GLuint vColor;
	float rotationAngle;
	//Use this if you want all of the polyhedron faces changing colors
	void animateColorsOfFaces();

};
#endif


