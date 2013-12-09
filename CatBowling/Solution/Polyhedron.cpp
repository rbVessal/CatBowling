#include "Polyhedron.h"

#define MAX_SPEED 0.01
#define FRICTION 0.0000001


Polyhedron::Polyhedron(void)
{
	rotationAngle = 0;
	translateOnce = false;
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
	rotationAngle = 0;
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
	zNear = 0.5, zFar = 3.0;

	physicsComponent.velocity = glm::vec3(0.0, 0.0, 0.0);
	physicsComponent.acceleration = glm::vec3(0.01, 0.01, 0.0);

	//Initialize composite transformation matrix to indentity matrix
	compositeModelTransformationMatrix = glm::mat4(1.0f);
	//Initialize the angle axis
	rotationAngleAxis = glm::vec3(0.0f, 0.0f, centerZ + halfWidthExtentZ);
	//Normalize it for rotation quaternion
	rotationAngleAxis = glm::normalize(rotationAngleAxis);
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

/*
void Polyhedron::draw()
{
   
}

void Polyhedron::drawTriangles(int indice0, int indice1, int indice2, int)
{
	
}*/

//Update the position based on euler integration
//see: http://physics2d.com/content/euler-integration
void Polyhedron::eulerIntegrationUpdatePosition()
{
	offsetX += physicsComponent.velocity.x;
	offsetY += physicsComponent.velocity.y;
}

void Polyhedron::move(Polyhedron** polyhedronArray, int size)
{
	for(int i = 0; i < size; i++)
	{
		Polyhedron* other = polyhedronArray[i];

		// Collision response
		if(this != other)
		{
			// METHOD 1:
			//Check to see if the polyhedrons collided using AABB
			/*AABB* aabb = dynamic_cast<AABB*>(collider);
			AABB* aabb_other = dynamic_cast<AABB*>(other->getCollider());
			if(aabb && aabb_other)
			{
				bool isCollided = aabb->checkAABB(aabb_other);
				
				//If collided then set the velocity to its inverse
				if(isCollided)
				{
					this->setVelocity(-1 * (this->getVelocity().x), -1 * (this->getVelocity().y),  -1 * (this->getVelocity().z));
					physicsComponent.acceleration.x *= -1;
					physicsComponent.acceleration.y *= -1;
				}
			}*/
			
			// - - - - - - - - 

			// Method 2:
			// Use any collider to get a vector that is reflected over the collision normal
			//glm::vec3 v1 = collider->collisionResponseVector(other->getCollider(), getVelocity());
			//glm::vec3 v2 = collider->collisionResponseVector(other->getCollider(), other->getVelocity());

			/*
			if(v1.x != physicsComponent.velocity.x)
				physicsComponent.acceleration.x *= -1;
			if(v1.y != physicsComponent.velocity.y)
				physicsComponent.acceleration.y *= -1;
			if(v1.z != physicsComponent.velocity.z)
				physicsComponent.acceleration.z *= -1;
				*/
				
			// Update this and other's velocities from the collision
			//this->setVelocity(v1.x, v1.y, v1.z);
			//other->setVelocity(v2.x, v2.y, v2.z);

			// - - - - - - - - - - -

			// Method 3:
			// Use the octree. See testCollision()
			// ...

		}
	}
	//Update the position using euler integration
	eulerIntegrationUpdatePosition();

	//Update AABB
	AABB* aabb = dynamic_cast<AABB*>(collider);
	if(aabb)
	{
		aabb->setCenter(glm::vec3(centerX + offsetX, centerY + offsetY, centerZ + offsetZ));
	}
}

void Polyhedron::testCollision(Polyhedron* other)
{
	// Use any collider to get a vector that is reflected over the collision normal
	glm::vec3 v1 = collider->collisionResponseVector(other->getCollider(), getVelocity());
	glm::vec3 v2 = collider->collisionResponseVector(other->getCollider(), other->getVelocity());

	if(v1.x != physicsComponent.velocity.x)
		physicsComponent.acceleration.x *= -1;
	if(v1.y != physicsComponent.velocity.y)
		physicsComponent.acceleration.y *= -1;
	if(v1.z != physicsComponent.velocity.z)
		physicsComponent.acceleration.z *= -1;
				
	// Update this and other's velocities from the collision
	this->setVelocity(v1.x, v1.y, v1.z);
	other->setVelocity(v2.x, v2.y, v2.z);
}

/*void Polyhedron::changeColors()
{
	
}*/

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
		rotationAngle += 10;
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
	//Then translate back using the total offset/velocity
	//compositeModelTransformationMatrix = glm::translate(compositeModelTransformationMatrix, glm::vec3(-offsetX, -offsetY, -offsetZ));
}
void Polyhedron::translateBackToCurrentPosition()
{
	//compositeModelTransformationMatrix = glm::translate(compositeModelTransformationMatrix, glm::vec3(offsetX, offsetY, offsetZ));
	compositeModelTransformationMatrix = glm::translate(compositeModelTransformationMatrix, glm::vec3(centerX, centerY, centerZ));
}

void Polyhedron::display( void )
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
	//First translate back to origin to ensure the other model transformations are applied correctly
	//if(!translateOnce)
	//{
		translateBackToOrigin();
		//translateOnce = true;
	//}
	
	//Scaling example
	//compositeModelTransformationMatrix = glm::scale(compositeModelTransformationMatrix, glm::vec3(0.5, 0.5, 1.0));
	
	//Shearing example
	//compositeModelTransformationMatrix = glm::shearX3D(compositeModelTransformationMatrix, 1.0f, 1.0f);

	//Note: Angle should be expressed in radians if GLM_FORCE_RADIANS is defined
	//otherwise use degrees
	//x, y, and z should be normalized coordinates as each of them represents
	//the axis
	//see: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
	//glm::quat quaternion = glm::angleAxis(rotationAngle, rotationAngleAxis);
	//Convert the quarternion to a 4x4 matrix for the shader
	//glm::mat4 rotationQuaternionMatrix = glm::mat4_cast(quaternion);
	//compositeModelTransformationMatrix = rotationQuaternionMatrix * compositeModelTransformationMatrix;
	//compositeModelTransformationMatrix =  glm::rotate(compositeModelTransformationMatrix, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	translateBackToCurrentPosition();
	//GLM matrices are already transposed, so we can pass in GL_FALSE
	glUniformMatrix4fv( transformationMatrix, 1, GL_FALSE, glm::value_ptr(compositeModelTransformationMatrix));
	

	//Define the view matrix as the eye coordinates
	vec4  eye( radius*sin(theta)*cos(phi),
		 radius*sin(theta)*sin(phi),
		 radius*cos(theta), 
		 1.0f);
    vec4  at( 0.0f, 0.0f, 0.0f, 1.0f);
    vec4    up( 0.0f, 1.0f, 0.0f, 0.0f);

	mat4 mv = LookAt(eye, at, up);
	glUniformMatrix4fv( view, 1, GL_TRUE, mv);

	//Define the prespective projection matrix
    mat4  perspectiveProjection = Frustum( left, right, bottom, top, zNear, zFar );
	glUniformMatrix4fv( projection, 1, GL_TRUE, perspectiveProjection);	

	//Draw those beautiful polyhedrons using GL_TRIANGLES
	glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}
