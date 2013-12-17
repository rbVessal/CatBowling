#include "TrajectoryCurve.h"

#define NUMBER_OF_POINTS 2

//Default constructor
TrajectoryCurve::TrajectoryCurve(void)
{
	TrajectoryCurve(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.0f, 1.0f, 1.0f));
}

//Parameterized constructor
TrajectoryCurve::TrajectoryCurve(glm::vec3 beginningPoint, glm::vec3 centerPoint, glm::vec3 endPoint)
{
	this->beginningPoint = beginningPoint;
	this->centerPoint = centerPoint;
	this->endPoint = endPoint;
	points = new glm::vec4[NUMBER_OF_POINTS];
	colors = new glm::vec4[NUMBER_OF_POINTS];

	radius = 1.0;
	theta = 0.0;
	phi = 0.0;

	left = -1.0, right = 1.0;
	bottom = -1.0, top = 1.0;
	zNear = 0.5, zFar = 3.0;

	compositeModelTransformationMatrix = glm::mat4(1.0f);
}

void TrajectoryCurve::init(GLuint program)
{
	draw();
	setupVBO();
	setupVAO(program);
	
}

//Draw the bezier curve based on the points given
//and the bezier curve formula for [x,y]=(1–t)2P0+2(1–t)tP1+t2P2
//where P0 is beginning point, P1 is  the center point, and P2 is the end point
//see: http://devmag.org.za/2011/04/05/bzier-curves-a-tutorial/
void TrajectoryCurve::draw()
{
	for(int i = 0; i < NUMBER_OF_POINTS; i++)
	{
		/*GLfloat x = pow(1 - i, 2) * beginningPoint.x + 2 * (1 - i) * i * centerPoint.x + pow(i, 2) * endPoint.x;
		GLfloat y = pow(1 - i, 2) * beginningPoint.y + 2 * (1 - i) * i * centerPoint.y + pow(i, 2) * endPoint.y;
		GLfloat z = pow(1 - i, 2) * beginningPoint.z + 2 * (1 - i) * i * centerPoint.z + pow(i, 2) * endPoint.z;*/
		if(i == 0)
		{
			points[i] = glm::vec4(beginningPoint.x, beginningPoint.y, beginningPoint.z, 1.0f);
		}
		else
		{
			points[i] = glm::vec4(endPoint.x, endPoint.y, endPoint.z, 1.0f);
		}
		colors[i] = color4( 1.0, 0.0, 0.0, 1.0 );  // yellow
	}
}

void TrajectoryCurve::setupVBO()
{
	// Initialize the buffer object
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, NUMBER_OF_POINTS * sizeof(glm::vec4) + NUMBER_OF_POINTS * sizeof(color4), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, NUMBER_OF_POINTS * sizeof(glm::vec4), points );
	glBufferSubData( GL_ARRAY_BUFFER, NUMBER_OF_POINTS * sizeof(glm::vec4), NUMBER_OF_POINTS * sizeof(color4), colors );
}

void TrajectoryCurve::setupVAO(GLuint program)
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
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(NUMBER_OF_POINTS * sizeof(glm::vec4)) );

    view = glGetUniformLocation( program, "viewMatrix" );
    projection = glGetUniformLocation( program, "projection" );
	transformationMatrix = glGetUniformLocation(program, "transformationMatrix");
}

void TrajectoryCurve::display()
{
	// Bind the vao and vbo for multiple objects on screen
	//see:  http://t-machine.org/index.php/2013/10/18/ios-open-gl-es-2-multiple-objects-at-once/
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBindVertexArray( vao );

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

	//Draw that bezier curve!
	glDrawArrays( GL_LINE_STRIP, 0, NUMBER_OF_POINTS );
}

//Default constructor
TrajectoryCurve::~TrajectoryCurve(void)
{
	delete points;
	delete colors;
}
