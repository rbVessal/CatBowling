#include "Polyhedron.h"

#define MAX_SPEED 0.01
#define FRICTION 0.0000001


Polyhedron::Polyhedron(void)
{
	rotationAngle = 0;
	isVisible = true;
}

Polyhedron::Polyhedron(const Polyhedron& other)
{
	doCopy(other);
}

const Polyhedron& Polyhedron::operator=(const Polyhedron& other)
{
	if(&other != this)
	{
		doCopy(other);
	}

	return *this;
}

void Polyhedron::doCopy(const Polyhedron& other)
{
	rotationAngle = other.rotationAngle;
	rotationQuaternionMatrix = other.rotationQuaternionMatrix;
	centerX = other.centerX;
	centerY = other.centerY;
	centerZ = other.centerZ;
	halfWidthExtentX = other.halfWidthExtentX;
	halfWidthExtentY = other.halfWidthExtentY;
	halfWidthExtentZ = other.halfWidthExtentZ;
	rotationAngleAxis = other.rotationAngleAxis;
	offsetX = other.offsetX;
	offsetY = other.offsetY;
	offsetZ = other.offsetZ;
	index = other.index;
	NumVertices = other.NumVertices;
	numberOfTriangles = other.numberOfTriangles;
	view = other.view;
	projection = other.projection;
	transformationMatrix = other.transformationMatrix;
	vbo = other.vbo;
	vao = other.vao;
	radius = other.radius;
	theta = other.theta;
	phi = other.phi;
	left = other.left;
	right = other.right;
	bottom = other.bottom;
	top = other.top;
	zNear = other.zNear;
	zFar = other.zFar;

	collider = other.collider;
	physicsComponent = other.physicsComponent;
	
	newRandR = other.newRandR;
	newRandG= other.newRandG;
	newRandB = other.newRandB;
	newRandA= other.newRandA;

	switchColors = other.switchColors;
	timerInterval = other.timerInterval;
	previousTime = other.previousTime;
	currentTime = other.currentTime;

	vColor = other.vColor;

	randomNumberR = other.randomNumberR;
	randomNumberG = other.randomNumberG;
	randomNumberB = other.randomNumberB;
	randomNumberA = other.randomNumberA;

	compositeModelTransformationMatrix = other.compositeModelTransformationMatrix;

	for(int i=0; i<8; i++)
	{
		vertex_colors[i] = other.vertex_colors[i];
	}
}

Polyhedron::~Polyhedron(void)
{
}

//Initialize the values needed for projection
void Polyhedron::initValues()
{
	index = 0;

	//RGBA colors
	vertex_colors[0] = color4( 0.0, 0.0, 0.0, 1.0 );  // black
    vertex_colors[1] = color4( 1.0, 0.0, 0.0, 1.0 );  // red
    vertex_colors[2] = color4( 1.0, 1.0, 0.0, 1.0 );  // yellow
    vertex_colors[3] = color4( 0.0, 1.0, 0.0, 1.0 );  // green
    vertex_colors[4] = color4( 0.0, 0.0, 1.0, 1.0 );  // blue
    vertex_colors[5] = color4( 1.0, 0.0, 1.0, 1.0 );  // magenta
    vertex_colors[6] = color4( 1.0, 1.0, 1.0, 1.0 );  // white
    vertex_colors[7] = color4( 0.0, 1.0, 1.0, 1.0 );  // cyan

	radius = 1.0;
	theta = 0.0;
	phi = 0.0;

	left = -1.0, right = 1.0;
	bottom = -1.0, top = 1.0;
	zNear = 0.4, zFar = 3.0;

	physicsComponent.velocity = glm::vec3(0.0, 0.0, 0.0);
	physicsComponent.acceleration = glm::vec3(0.0, 0.0, 0.0);

	//Initialize composite transformation matrix to indentity matrix
	compositeModelTransformationMatrix = glm::mat4(1.0f);
	rotationQuaternionMatrix = glm::mat4(1.0f);
}

void Polyhedron::init(GLuint program)
{
	//Generate the vertices needed for VBO and VAO
	draw();
	//Setup the VBO
	setupVBO();
	//Setup the VAO
	setupVAO(program);
}

void Polyhedron::setupVAO(GLuint program)
{
	 // Use the vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(NumVertices * sizeof(glm::vec4)) );

    view = glGetUniformLocation( program, "viewMatrix" );
    projection = glGetUniformLocation( program, "projection" );
	transformationMatrix = glGetUniformLocation(program, "transformationMatrix");
}

void Polyhedron::setupVBO()
{
	// Initialize the buffer object
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, NumVertices * sizeof(glm::vec4) + NumVertices * sizeof(color4), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, NumVertices * sizeof(glm::vec4), points );
    glBufferSubData( GL_ARRAY_BUFFER, NumVertices * sizeof(glm::vec4), NumVertices * sizeof(color4), colors );
}

// Setters and Getters
Collider* Polyhedron::getCollider()
{
	return collider;
}

glm::vec3 Polyhedron::getVelocity()
{
	return physicsComponent.velocity;
}

void Polyhedron::setVelocity(float x, float y, float z)
{
	physicsComponent.velocity = glm::vec3(x, y, z);
}

void Polyhedron::setVelocityLocal(float x, float y, float z)
{
	glm::vec4 v = rotationQuaternionMatrix * glm::vec4(x, y, z, 1);
	physicsComponent.velocity = glm::vec3(v.x, v.y, v.z);
}

//Update velocity and position based on euler integration
//see: http://physics2d.com/content/euler-integration
void Polyhedron::eulerIntegrationUpdate()
{
	physicsComponent.velocity += physicsComponent.acceleration;

	offsetX += physicsComponent.velocity.x;
	offsetY += physicsComponent.velocity.y;
	offsetZ += physicsComponent.velocity.z;
}

// Change the polyhedron's offset
// (Used for non-continuous movement)
void Polyhedron::translate(float x, float y, float z)
{
	offsetX += x;
	offsetY += y;
	offsetZ += z;
}

void Polyhedron::rotate(float angle, glm::vec3 axis)
{
	//Note: Angle should be expressed in radians if GLM_FORCE_RADIANS is defined
	//otherwise use degrees
	//x, y, and z should be normalized coordinates as each of them represents the axis
	rotationAngle += angle;
	GLfloat xAxis = 0.0f;
	GLfloat yAxis = 0.0f;
	GLfloat zAxis = 0.0f;
	if(axis.x == 1)
	{
		xAxis = centerX + halfWidthExtentX;
	}
	if(axis.y == 1)
	{
		yAxis = centerY + halfWidthExtentY;
	}
	if(axis.z == 1)
	{
		zAxis = centerZ + halfWidthExtentZ;
	}
	
	//Initialize the angle axis
	rotationAngleAxis = glm::vec3(xAxis, yAxis, zAxis);
	//Normalize it for rotation quaternion
	rotationAngleAxis = glm::normalize(rotationAngleAxis);
	//see: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
	glm::quat quaternion = glm::angleAxis(rotationAngle, rotationAngleAxis);
	//Convert the quarternion to a 4x4 matrix for the shader
	rotationQuaternionMatrix = glm::mat4_cast(quaternion);
	
}

void Polyhedron::resetPolyhedron()
{
	setVelocity(0, 0, 0);
	physicsComponent.acceleration = glm::vec3(0, 0, 0);
	rotationAngle = 0;
	rotationQuaternionMatrix = glm::mat4(1.0f);
	offsetX = 0;
	offsetY = 0;
	offsetZ = 0;
	isVisible = true;
}

void Polyhedron::move(Polyhedron** polyhedronArray, int size)
{	
	// Apply Friction
	physicsComponent.velocity *= 0.99;

	//Update the position and velocity using euler integration
	eulerIntegrationUpdate();

	//Update AABB
	AABB* aabb = dynamic_cast<AABB*>(collider);
	if(aabb)
	{
		aabb->setCenter(glm::vec3(centerX + offsetX, centerY + offsetY, centerZ + offsetZ));
	}
}

void Polyhedron::testCollision(Polyhedron* other)
{
	const float MAX_MASS = 50; // set a high mass to consider "unmovable"
	int type = collider->calculateCollisionType(other->getCollider());	// what type of collision?

	if(isVisible && other->isVisible)
	{
		if(type == AABB_AABB)
		{
			// Use any collider to get a vector that is reflected over the collision normal
			glm::vec3 v1 = collider->collisionResponseVector(other->getCollider(), getVelocity());
			glm::vec3 v2 = other->collider->collisionResponseVector(collider, other->getVelocity());

			// If the velocities changed upon collision
			if(v1 != getVelocity() || v2 != other->getVelocity())
			{
				// If mass of an object is large, assume it should never move
				if(physicsComponent.mass > MAX_MASS || other->physicsComponent.mass > MAX_MASS)
				{
					// Objects keep their own momentum that the colliders calculated
					this->setVelocity(v1.x, v1.y, v1.z);
					other->setVelocity(v2.x, v2.y, v2.z);

					// Check for tunneling
					if(physicsComponent.mass > MAX_MASS)
					{
						if(other->physicsComponent.velocity.length() < 5)
						{
							physicsComponent.acceleration *= -1;
						}
					}
					else if(other->physicsComponent.mass > MAX_MASS)
					{
						if(physicsComponent.velocity.length() < 5)
						{
							physicsComponent.acceleration *= -1;
						}
					}
				}
				// Else, conserve momentum with an elastic collision
				else
				{
					// Initial values
					glm::vec3 u1 = getVelocity();
					glm::vec3 u2 = other->getVelocity();
					float m1 = physicsComponent.mass;
					float m2 = other->physicsComponent.mass;

					// v1 = ( m1-m2 / m1+m2 ) * u1 + ( 2 * m2 / m1 + m2 ) * u2
					glm::vec3 v1final = ( m1-m2 / m1+m2 ) * u1 + ( 2 * m2 / m1 + m2 ) * u2;

					// v2 = ( m2-m1 / m1+m2 ) * u2 + ( 2 * m1 / m1 + m2 ) * u1
					glm::vec3 v2final = ( m2-m1 / m1+m2 ) * u2 + ( 2 * m1 / m1 + m2 ) * u1;

					// Fix stupid math
					if(m1 == m2)
					{
						v1final = ( 2 * m2 / m1 + m2 ) * u2;
						v2final = ( 2 * m1 / m1 + m2 ) * u1;
					}

					/*
					int max = 3;
					if(v1final.x > max || v2final.x > max || v1final.y > max ||  v2final.y > max || v1final.z > max || v2final.z > max)
					{
						// Check for strange values
						std::cout << "x: " << v1final.x << " y: " << v1final.y << " z: " << v1final.z << std::endl;
						std::cout << "x: " << v2final.x << " y: " << v2final.y << " z: " << v2final.z << std::endl;
					}*/
			
					// Set velocities
					this->setVelocity(v1final.x, v1final.y, v1final.z);
					other->setVelocity(v2final.x, v2final.y, v2final.z);
				}
			}
		}
		else if(type == AABB_LINE)
		{
			if(collider->checkCollision(other->getCollider()))
			{
				this->setVisiblity(false);
			}
		}
		else if(type == LINE_AABB)
		{
			if(collider->checkCollision(other->getCollider()))
			{
				other->setVisiblity(false);
			}
		}
	}
}

void Polyhedron::animateColorsOfFaces()
{
	//Animate the colors of the polyhedron
	//Get the current time
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timerInterval = currentTime - previousTime;
	if(timerInterval  > 1000)
	{
		previousTime = currentTime;
		for(int i = 0; i < NumVertices; i++)
		{
			newRandR = static_cast<float>(rand() % 100) / 100;
			newRandG = static_cast<float>(rand() % 100) / 100;
			newRandB = static_cast<float>(rand() % 100) / 100;
			newRandA = static_cast<float>(rand() % 100) / 100;
			colors[i] = color4(newRandR, newRandG, newRandB, newRandA);
		}

	}
}

//For clearing out the composite model transformation matrix after it has been applied to the shader
void Polyhedron::clearCompositeModelTransformationMatrix()
{
	compositeModelTransformationMatrix = glm::mat4(1);
}

void Polyhedron::translateBackToOrigin()
{
	//Translate back the points using the center
	compositeModelTransformationMatrix = glm::translate(compositeModelTransformationMatrix, glm::vec3(-centerX, -centerY, -centerZ));

	// NOTE: this was causing the translation issues. Not sure why.
	//Then translate back using the total offset/velocity
	//compositeModelTransformationMatrix = glm::translate(compositeModelTransformationMatrix, glm::vec3(-offsetX, -offsetY, -offsetZ));
}
void Polyhedron::translateBackToCurrentPosition()
{
	compositeModelTransformationMatrix = glm::translate(compositeModelTransformationMatrix, glm::vec3(offsetX, offsetY, offsetZ));
	compositeModelTransformationMatrix = glm::translate(compositeModelTransformationMatrix, glm::vec3(centerX, centerY, centerZ));
}

void Polyhedron::display( void )
{
 	if(isVisible)
	{
		//Change the colors of the polyhedron faces
		animateColorsOfFaces();
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferSubData( GL_ARRAY_BUFFER, NumVertices * sizeof(glm::vec4), NumVertices * sizeof(glm::vec4), colors );

		// Bind the vao and vbo for multiple objects on screen
		//see:  http://t-machine.org/index.php/2013/10/18/ios-open-gl-es-2-multiple-objects-at-once/
	
		glBindVertexArray( vao );

		//Define the model matrix using transformations
		//Transformations - Scaling, Rotating, Translation, Skewing
		//see: http://stackoverflow.com/questions/12838375/model-matrix-in-glm
		//Clear the composite transformation matrix
		clearCompositeModelTransformationMatrix();
	
		//Scaling example
		//compositeModelTransformationMatrix = glm::scale(compositeModelTransformationMatrix, glm::vec3(0.5, 0.5, 1.0));
	
		//Shearing example
		//compositeModelTransformationMatrix = glm::shearX3D(compositeModelTransformationMatrix, 1.0f, 1.0f);
	
		//Matrix multiplication for OpenGL and GLSL happens in reverse order
		//So call the model transformations in reverse order to achieve this
		translateBackToCurrentPosition();
		compositeModelTransformationMatrix = compositeModelTransformationMatrix * rotationQuaternionMatrix;
		translateBackToOrigin();
	
		//Rotation using Euler angles
		/*translateBackToCurrentPosition();
		compositeModelTransformationMatrix = glm::rotate(compositeModelTransformationMatrix, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		translateBackToOrigin();*/

		//GLM matrices are already transposed, so we can pass in GL_FALSE
		glUniformMatrix4fv( transformationMatrix, 1, GL_FALSE, glm::value_ptr(compositeModelTransformationMatrix));
	

		//Define the view matrix as the eye coordinates
		vec4  eye( radius*sin(theta)*cos(phi),
			 radius*sin(theta)*sin(phi),
			 radius*cos(theta), 
			 1.0f);
		//vec4  at( 0.0f, 0.0f, 0.0f, 1.0f);
		vec4  at( 0.0f, -1.0f, 0.0f, 1.0f);
		vec4    up( 0.0f, 1.0f, 0.0f, 0.0f);

		mat4 mv = LookAt(eye, at, up);
		glUniformMatrix4fv( view, 1, GL_TRUE, mv);

		//Define the prespective projection matrix
		mat4  perspectiveProjection = Frustum( left, right, bottom, top, zNear, zFar );
		glUniformMatrix4fv( projection, 1, GL_TRUE, perspectiveProjection);	

		//Draw those beautiful polyhedrons using GL_TRIANGLES
		glDrawArrays( GL_TRIANGLES, 0, NumVertices );
	}
}
