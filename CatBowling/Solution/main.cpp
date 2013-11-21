// Data Structures and Algorithms II
// Final Project - Cat Bowling
// Rebecca Vessal and Jennifer Stanton

#include "Cube.h"
#include "Tetrahedron.h"
#include "Octahedron.h"
#include "PolyController.h"
#include <time.h>
#include "vld.h"

#define MENU 0
#define GAME 1

Polyhedron** polyhedronArray;
int sizeOfPolyhedronArray;

Polyhedron** menuPolys;
int sizeOfMenuPolys;

PolyController* menu;
PolyController* game;

int screenState;
GLuint program;

//----------------------------------------------------------------------------
const GLfloat  dr = 5.0 * DegreesToRadians;
//----------------------------------------------------------------------------

// OpenGL initialization
void init()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "Shaders\\vshader.glsl", "Shaders\\fshader.glsl" );
    glUseProgram( program );

	float speed = 0.01;

	//Make right cube
	Cube* cube = new Cube();

	//Make left cube
	Cube* cube2 = new Cube(-2.0, 0, 0, 0.5, 0.5, 0.5, false);
	cube2->setVelocity(-speed, speed, 0.0);

	// right
	Tetrahedron* tetrahedron = new Tetrahedron(0, -2.5, -0.1);
	tetrahedron->setVelocity(0.0, speed, 0.0);

	// left
	Tetrahedron* tetrahedron2 = new Tetrahedron(-3.2, -2, -0.1);
	tetrahedron2->setVelocity(speed, speed, 0.0);
    
	Octahedron* octahedron = new Octahedron(-1, -3, 0);
	octahedron->setVelocity(speed, 0.0, 0.0);

	Octahedron* octahedron2 = new Octahedron(-2.1, -3, 0);
	octahedron2->setVelocity(speed, speed, 0.0);

	Cube* leftWallCube = new Cube(-5, 0, 0.5, 1.0, 6.0, 1.25, true);
	Cube* rightWallCube = new Cube(2.95, 0, 0.5, 1.0, 16.0, 1.25, true);
	Cube* topWallCube = new Cube(-1, 2.2, 0.5, 16.0, 0.25, 1.25, true);
	Cube* bottomWallCube = new Cube(-1, -3.15, 0.5, 6.0, 0.25, 1.25, true);

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

	sizeOfMenuPolys = 1;
	menuPolys = new Polyhedron*[sizeOfMenuPolys];
	menuPolys[0] = new Cube(-1.0, 0, 0, 0.5, 0.5, 0.5, false); // testing menu
	
	// PolyController init
	game = new PolyController(polyhedronArray, sizeOfPolyhedronArray);
	menu = new PolyController(menuPolys, sizeOfMenuPolys);

	game->init(program);
	menu->init(program);
	

    glEnable( GL_DEPTH_TEST );
}

//----------------------------------------------------------------------------

void display( void )
{
	//Clear the screen before drawing the objects onto the screen
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glUseProgram( program );

	// Render either the Menu polys or the Game polys
	if(screenState==MENU)
	{
		menu->display();
	}
	else if(screenState==GAME)
	{
		game->display();
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
		// Switch between menu and game
		case ' ':
			if(screenState==MENU){screenState=GAME;}
			else if(screenState==GAME){screenState=MENU;}
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

	Polyhedron** otherArray = &polyhedronArray[0];
	polyhedronArray[2]->move(otherArray, 1);

	polyhedronArray[0]->move(polyhedronArray, sizeOfPolyhedronArray);
	polyhedronArray[1]->move(polyhedronArray, sizeOfPolyhedronArray);

	glutPostRedisplay();
}

int main( int argc, char **argv )
{
	screenState = GAME; //MENU;
	srand(time(NULL)); // Random seed

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Cat Bowling" );
	glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc( reshape );
	glutIdleFunc(idle);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION); // Let the program continue after main loop
    glutMainLoop();

	// Deleting game and menu deletes all of the polyhedrons
	delete game;
	delete menu;

    return 0;
}

