// Data Structures and Algorithms II
// Final Project - Cat Bowling
// Rebecca Vessal and Jennifer Stanton

#include "Line.h"
#include "Cube.h"
#include "Tetrahedron.h"
#include "Octahedron.h"
#include "PolyController.h"
#include "GameController.h"
#include "Octree.h"
#include <time.h>
#include "vld.h"

#define MENU 0
#define GAME 1
#define GAME_PAUSE 2
#define TESTSTATE 3

//#define TEST_MODE

// Test objects
Polyhedron** polyhedronArray;
int sizeOfPolyhedronArray;
Octree* testOctree;
PolyController* test;

// Game and Menu objects
Polyhedron** gamePolys;
int sizeOfGamePolys;

Polyhedron** menuPolys;
int sizeOfMenuPolys;

PolyController* menu;
PolyController* game;

static GameController* gameController;

Octree* octree;

static int screenState;
static GLuint program;

//----------------------------------------------------------------------------
const GLfloat  dr = 5.0 * DegreesToRadians;
//----------------------------------------------------------------------------

// The "Geometry Wars" screen state for polygon testing
void initTestState()
{
	float speed = 0.01;

	//Make right cube
	Cube* cube = new Cube();

	//Make left cube
	Cube* cube2 = new Cube(-2.0, 0, 0, 0.5, 0.5, 0.5, false);
	cube2->setVelocity(-speed, speed, 0.0);

	// right
	Tetrahedron* tetrahedron = new Tetrahedron(0.0, 2.25, -0.1);
	tetrahedron->setVelocity(0.0, speed, 0.0);

	// left
	Tetrahedron* tetrahedron2 = new Tetrahedron(-2.0, 2.25, -0.1);
	tetrahedron2->setVelocity(speed, speed, 0.0);
    
	Octahedron* octahedron = new Octahedron(0.0, -1.5, 0.0);
	octahedron->setVelocity(speed, 0.0, 0.0);

	Octahedron* octahedron2 = new Octahedron(2.0, -1.5, 0.0);
	octahedron2->setVelocity(speed, speed, 0.0);

	Cube* leftWallCube = new Cube(-5, 0, 0.5, 1.0, 6.0, 1.25, false);
	Cube* rightWallCube = new Cube(2.95, 0, 0.5, 1.0, 16.0, 1.25, false);
	Cube* topWallCube = new Cube(-1, 2.2, 0.5, 16.0, 0.25, 1.25, false);
	Cube* bottomWallCube = new Cube(-1, -3.15, 0.5, 6.0, 0.25, 1.25, false);

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
	
	// Octree
	testOctree = new Octree();
	testOctree->buildTree(glm::vec3(0, 0, 0), 5, 3);

	// Add each polyhedron
	for(int i = 0; i < sizeOfPolyhedronArray; i++)
	{
		// TODO: figure out something for the radius
		testOctree->insert(polyhedronArray[i], polyhedronArray[i]->getCenter(), 1.0);
	}
	
	test = new PolyController(polyhedronArray, sizeOfPolyhedronArray);
	test->init(program);
}

// OpenGL initialization
void init()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "Shaders\\vshader.glsl", "Shaders\\fshader.glsl" );
    glUseProgram( program );

#ifdef TEST_MODE
	initTestState();
#endif
	// Positioning values
	float pinHeight = -1.75;

	// Game
	sizeOfGamePolys = 13;
	gamePolys = new Polyhedron*[sizeOfGamePolys];
	gamePolys[0] = new Cube(0.0, -2.75, -3.25, 2.0, 0.1, 8.0, false); // bowling lane
	gamePolys[1] = new Octahedron(0.0, -1.25, 1.25, 0.75, 0.75, 0.75); // the "ball" (a cube for now)
	// Back row of pins (4)
	gamePolys[2] = new Tetrahedron(1.0, pinHeight, -4.5);
	gamePolys[3] = new Tetrahedron(-1.0, pinHeight, -4.5);
	gamePolys[4] = new Tetrahedron(2.0, pinHeight, -4.5);
	gamePolys[5] = new Tetrahedron(-2.0, pinHeight, -4.5);
	// Middle row (3)
	gamePolys[6] = new Tetrahedron(0.0, pinHeight, -3.5);
	gamePolys[7] = new Tetrahedron(1.5, pinHeight, -3.5);
	gamePolys[8] = new Tetrahedron(-1.5, pinHeight, -3.5);
	// 2nd row (2)
	gamePolys[9] = new Tetrahedron(1.0, pinHeight, -2.5);
	gamePolys[10] = new Tetrahedron(-1.0, pinHeight, -2.5);
	// Front row (1)
	gamePolys[11] = new Tetrahedron(0.0, pinHeight, -1.5);
	// Back wall
	gamePolys[12] = new Line(glm::vec3(5, pinHeight, -5), glm::vec3(-5, pinHeight, -5));

	// Set the mass of the pins
	for(int i=2; i<sizeOfGamePolys; i++)
	{
		gamePolys[i]->setMass(1);
	}

	// Set the mass of heavier objects
	gamePolys[0]->setMass(99); // lane
	gamePolys[12]->setMass(99); // back wall

	// Set the mass of the ball
	gamePolys[1]->setMass(1.25);

	// Menu
	sizeOfMenuPolys = 1;
	menuPolys = new Polyhedron*[sizeOfMenuPolys];
	menuPolys[0] = new Cube();
	
	// PolyControllers
	game = new PolyController(gamePolys, sizeOfGamePolys);
	menu = new PolyController(menuPolys, sizeOfMenuPolys);
	game->init(program);
	menu->init(program);

	// GameController
	gameController = new GameController(gamePolys[1], &(gamePolys[2]) );
	gameController->start();

	// Octree
	octree = new Octree();
	octree->buildTree(glm::vec3(0, 0, 0), 10, 4);

	for(int i = 0; i < sizeOfGamePolys; i++)
	{
		// TODO: figure out something for the radius
		octree->insert(gamePolys[i], gamePolys[i]->getCenter(), 1.0);
	}

    glEnable( GL_DEPTH_TEST );
}

//----------------------------------------------------------------------------

void restart()
{
	// Set all polyhedrons
	for(int i=0; i<sizeOfGamePolys; i++)
	{
		gamePolys[i]->resetPolyhedron();
	}

	// Reset Game Controller
	gameController->start();
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
	else if(screenState==GAME || screenState==GAME_PAUSE)
	{
		game->display();
	}
	else if(screenState==TESTSTATE)
	{
		test->display();
	}

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
	// For changing velocity
	float speed = 0.01;

	// Gameplay controls
	if(screenState == GAME)
	{
		gameController->processInput(key);
	}

    switch( key ) 
	{
		// Start
		case ' ':
			if(screenState==MENU){screenState=GAME;}
			break;
		// Pause
		case 'p':
			if(screenState==GAME){screenState=GAME_PAUSE;}
			else if(screenState==GAME_PAUSE){screenState=GAME;}
			break;
		// Reset
		case 'r':
			if(screenState==GAME){restart(); screenState=MENU;}
			break;
		// Quit
		case 033: // Escape Key
		case 'q': case 'Q':
			//exit( EXIT_SUCCESS ); // causes memory leaks
			glutLeaveMainLoop(); // exit without memory leaks
			break;

		//Move the 1st cube in the polyhedron array with these keyboard controls
		/*case 'a': polyhedronArray[0]->setVelocity(-speed, 0.0, 0.0); break;
		case 'w': polyhedronArray[0]->setVelocity(0.0, speed, 0.0); break;
		case 'd': polyhedronArray[0]->setVelocity(speed, 0.0, 0.0); break;
		case 's': polyhedronArray[0]->setVelocity(0.0, -speed, 0.0); break;
		*/
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
	if(screenState == GAME)
	{
		gameController->update();

		// Check Octree
		octree->checkCollisions();

		// Move each polyhedron
		for(int i = 0; i < sizeOfGamePolys; i++)
		{
			Polyhedron* polyhedron = gamePolys[i];
			polyhedron->move(gamePolys, sizeOfGamePolys);
		}
	}
	else if(screenState == MENU)
	{
		for(int i = 0; i < sizeOfMenuPolys; i++)
		{
			menuPolys[i]->move(menuPolys, sizeOfMenuPolys);
		}
	}
	else if(screenState == TESTSTATE)
	{
		// Check Octree
		testOctree->checkCollisions();

		//Move each polyhedron
		for(int i = 0; i < sizeOfPolyhedronArray; i++)
		{
			Polyhedron* polyhedron = polyhedronArray[i];
			polyhedron->move(polyhedronArray, sizeOfPolyhedronArray);
		}

		// Are we still using these?
		/*
		Polyhedron** otherArray = &polyhedronArray[0];
		polyhedronArray[2]->move(otherArray, 1);

		polyhedronArray[0]->move(polyhedronArray, sizeOfPolyhedronArray);
		polyhedronArray[1]->move(polyhedronArray, sizeOfPolyhedronArray);
		*/
	}

	glutPostRedisplay();
}

int main( int argc, char **argv )
{
#ifdef TEST_MODE
	screenState = TESTSTATE;
#else
	screenState = MENU;
#endif
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
	delete gameController;
	delete octree;
	
#ifdef TEST_MODE
	delete test;
	delete testOctree;
#endif

    return 0;
}

