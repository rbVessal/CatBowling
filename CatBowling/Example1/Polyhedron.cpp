#include "Polyhedron.h"

#define MAX_SPEED 0.01
#define FRICTION 0.0000001


Polyhedron::Polyhedron(void)
{
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

	velocity = vec3(0.0, 0.0, 0.0);
	acceleration = vec3(0.01, 0.01, 0.0);
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
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(NumVertices * sizeof(point4)) );

	newX = glGetUniformLocation(program, "newX");
	newY = glGetUniformLocation(program, "newY");
	newZ = glGetUniformLocation(program, "newZ");

	randomNumberR = glGetUniformLocation(program, "randomNumberR");
	randomNumberG = glGetUniformLocation(program, "randomNumberG");
	randomNumberB = glGetUniformLocation(program, "randomNumberB");
	randomNumberA = glGetUniformLocation(program, "randomNumberA");

    model_view = glGetUniformLocation( program, "model_view" );
    projection = glGetUniformLocation( program, "projection" );
	//transformationMatrix = glGetUniformLocation(program, "transformationMatrix");
	
}

void Polyhedron::setupVBO()
{
	// Initialize the buffer object
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, NumVertices * sizeof(point4) + NumVertices * sizeof(color4), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, NumVertices * sizeof(point4), points );
    glBufferSubData( GL_ARRAY_BUFFER, NumVertices * sizeof(point4), NumVertices * sizeof(color4), colors );
}

//Getter for AABB
AABB Polyhedron::getAABB()
{
	return aabb;
}

vec3 Polyhedron::getVelocity()
{
	return velocity;
}

void Polyhedron::setVelocity(float x, float y, float z)
{
	velocity = vec3(x, y, z);
}

// generate 12 triangles: 36 vertices and 36 colors
void Polyhedron::draw()
{
   
}

void Polyhedron::drawTriangles(int indice0, int indice1, int indice2, int)
{
	
}

//Update the position based osn euler integration
//see: http://physics2d.com/content/euler-integration
void Polyhedron::eulerIntegrationUpdatePosition()
{
	offsetX += velocity.x;
	offsetY += velocity.y;
}

void Polyhedron::move(Polyhedron** polyhedronArray, int size)
{
	for(int i = 0; i < size; i++)
	{
		Polyhedron* other = polyhedronArray[i];

		if(this != other)
		{
			//Check to see if the polyhedrons collided using AABB
			bool isCollided = aabb.checkAABB(other->getAABB());
			//If collided then set the velocity to its inverse
			if(isCollided)
			{
				this->setVelocity(-1 * (this->getVelocity().x), -1 * (this->getVelocity().y),  -1 * (this->getVelocity().z));
				acceleration.x *= -1;
				acceleration.y *= -1;
			}
		}
	}
	//Update the position using euler integration
	eulerIntegrationUpdatePosition();

	//Update AABB
	aabb.centerPoint = vec3(centerX + offsetX, centerY + offsetY, centerZ + offsetZ);
}

void Polyhedron::changeColors()
{
	
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

void Polyhedron::display( void )
{
	//Change the colors of the polyhedron faces
	animateColorsOfFaces();
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferSubData( GL_ARRAY_BUFFER, NumVertices * sizeof(point4), NumVertices * sizeof(color4), colors );

	// Bind the vao and vbo for multiple objects on screen
	//see:  http://t-machine.org/index.php/2013/10/18/ios-open-gl-es-2-multiple-objects-at-once/
	
	glBindVertexArray( vao );

	//Possibly use for clearing matrices
	//glLoadIdentity();

	glUniform1f(newX, offsetX);
	glUniform1f(newY, offsetY);
	glUniform1f(newZ, offsetZ);

	//TODO:Finish this
	//Define the model matrix using transformations
	//Transformations - Scaling, Rotating, Translation, Skewing
	//mat4 scalingMatrix = Scale(1.0, 1.0, 1.0);
	//glUniformMatrix4fv( transformationMatrix, 1, GL_TRUE, scalingMatrix );


	//Define the view matrix as the eye coordinates
	point4  eye( radius*sin(theta)*cos(phi),
		 radius*sin(theta)*sin(phi),
		 radius*cos(theta),
		 1.0 );
    point4  at( 0.0, 0.0, 0.0, 1.0 );
    vec4    up( 0.0, 1.0, 0.0, 0.0 );

    mat4  mv = LookAt( eye, at, up );
	//mat4 modelViewMatrix = matrixCompMult(scalingMatrix, mv);
	glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );

	//Define the prespective projection matrix
    mat4  p = Frustum( left, right, bottom, top, zNear, zFar );
    glUniformMatrix4fv( projection, 1, GL_TRUE, p );

	//Draw those beautiful polyhedrons using GL_TRIANGLES
	glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}
