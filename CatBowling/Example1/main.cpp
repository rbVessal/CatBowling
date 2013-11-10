// Data Structures and Algorithms II
// A5 - Geometry Wars
// Rebecca Vessal and Jennifer Stanton

#include "Cube.h"
#include "Tetrahedron.h"
#include "Octahedron.h"
#include <time.h>

Cube* cube;
Cube* cube2;
Tetrahedron* tetrahedron;
Tetrahedron* tetrahedron2;
Octahedron* octahedron;
Octahedron* octahedron2;

//For the frustum walls
Cube* leftWallCube;
Cube* rightWallCube;
Cube* topWallCube;
Cube* bottomWallCube;

Polyhedron** polyhedronArray;
int sizeOfPolyhedronArray;

GLuint program;

//----------------------------------------------------------------------------
const GLfloat  dr = 5.0 * DegreesToRadians;
//----------------------------------------------------------------------------

// OpenGL initialization
void init()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

	float speed = 0.01;
	//Make right cube
	cube = new Cube();
	cube->init(program);
	//cube->setVelocity(speed, 0.0, 0.0);

	//Make left cube
	cube2 = new Cube(-2.0, 0, 0, 0.5, 0.5, 0.5, false);
	cube2->init(program);
	cube2->setVelocity(-speed, speed, 0.0);

	// right
	tetrahedron = new Tetrahedron(0, -2, -0.1);
	tetrahedron->init(program);
	tetrahedron->setVelocity(0.0, speed, 0.0);

	// left
	tetrahedron2 = new Tetrahedron(-3, -2, -0.1);
	tetrahedron2->init(program);
	tetrahedron2->setVelocity(speed, speed, 0.0);
    
	octahedron = new Octahedron(-1, -4.5, 0);
	octahedron->init(program);
	octahedron->setVelocity(speed, 0.0, 0.0);

	octahedron2 = new Octahedron(-3, -4.5, 0);
	octahedron2->init(program);
	octahedron2->setVelocity(speed, speed, 0.0);

	/*leftWallCube = new Cube(0, 0, 0, -0.5, 0.5, 1.75, true);
	leftWallCube->init(program);*/

	leftWallCube = new Cube(-5, 0, 0.5, 1.0, 6.0, 1.25, true);
	leftWallCube->init(program);

	rightWallCube = new Cube(2.95, 0, 0.5, 1.0, 16.0, 1.25, true);
	rightWallCube->init(program);

	topWallCube = new Cube(-1, 2.2, 0.5, 16.0, 0.25, 1.25, true);
	topWallCube->init(program);

	bottomWallCube = new Cube(-1, -3.15, 0.5, 6.0, 0.25, 1.25, true);
	bottomWallCube->init(program);

	sizeOfPolyhedronArray = 10;
	polyhedronArray = new Polyhedron*[sizeOfPolyhedronArray];
	polyhedronArray[0] = cube;
	polyhedronArray[1] = cube2;
	polyhedronArray[2] = tetrahedron;
	polyhedronArray[3] = tetrahedron2;
	polyhedronArray[4] = octahedron;
	polyhedronArray[5] = octahedron2;
	polyhedronArray[6] = leftWallCube;
	polyhedronArray[7] = rightWallCube;
	polyhedronArray[8] = topWallCube;
	polyhedronArray[9] = bottomWallCube;

    glEnable( GL_DEPTH_TEST );
}

//----------------------------------------------------------------------------

void display( void )
{
	//Clear the screen before drawing the objects onto the screen
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glUseProgram( program );

	// Render each object
	for(int i = 0; i < sizeOfPolyhedronArray; i++)
	{
		Polyhedron* polyhedron = polyhedronArray[i];
		polyhedron->display();
	}

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
	// For changing velocity
	float speed = 0.01;

    switch( key ) 
	{
		//keyboard controls for quitting
		case 033: // Escape Key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break;
		//Move the 1st cube in the polyhedron array with these keyboard controls
		case 'a': polyhedronArray[0]->setVelocity(-speed, 0.0, 0.0); break;
		case 'w': polyhedronArray[0]->setVelocity(0.0, speed, 0.0); break;
		case 'd': polyhedronArray[0]->setVelocity(speed, 0.0, 0.0); break;
		case 's': polyhedronArray[0]->setVelocity(0.0, -speed, 0.0); break;
    }

    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void reshape( int width, int height )
{
    glViewport( 0, 0, width, height );
}

//----------------------------------------------------------------------------

//Update loop
void idle()
{
	//Move each polyhedron
	for(int i = 0; i < sizeOfPolyhedronArray; i++)
	{
		Polyhedron* polyhedron = polyhedronArray[i];
		polyhedron->move(polyhedronArray, sizeOfPolyhedronArray);
	}

	glutPostRedisplay();
}

int main( int argc, char **argv )
{
	// Random seed
	srand(time(NULL));

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cube" );
	glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc( reshape );
	glutIdleFunc(idle);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION); // Let the program continue after main loop
    glutMainLoop();

	delete cube;
	delete cube2;
	delete tetrahedron;
	delete tetrahedron2;
	delete octahedron;
	delete octahedron2;
	delete leftWallCube;
	delete rightWallCube;
	delete topWallCube;
	delete bottomWallCube;

	delete polyhedronArray; // Contents of array are already deleted

    return 0;
}

