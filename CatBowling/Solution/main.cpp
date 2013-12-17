// Data Structures and Algorithms II
// Final Project - Cat Bowling
// Rebecca Vessal and Jennifer Stanton

#include "TrajectoryCurve.h"
#include "Cube.h"
#include "Tetrahedron.h"
#include "Octahedron.h"
#include "PolyController.h"
#include "GameController.h"
#include "BezierSurfaceController.h"
#include "Octree.h"
#include <time.h>
#include "vld.h"

#define MENU 0
#define GAME 1
#define GAME_PAUSE 2
#define TESTSTATE 3

//f#define TEST_MODE

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
static BezierSurfaceController* bezierSurfaceController;

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

	// Game
	sizeOfGamePolys = 5;
	gamePolys = new Polyhedron*[sizeOfGamePolys];
	gamePolys[0] = new Cube(0.0, -3.25, -4.0, 2.0, 0.1, 6.0, false); // bowling lane
	gamePolys[1] = new Cube(0.0, -1.0, 0.25, 0.5, 0.5, 0.5, false); // the "ball" (a cube for now)
	gamePolys[2] = new Tetrahedron(0.0, -2.0, -4.5); // pins
	gamePolys[3] = new Tetrahedron(1.5, -2.0, -4.5);
	gamePolys[4] = new Tetrahedron(-1.5, -2.0, -4.5);

	//Create the trajectory curve for the game controller
	//beginning point - ball's center point
	//center point - mid point between ball and 1st pin (most front)
	//end point - pin's center point
	TrajectoryCurve* trajectoryCurve = new TrajectoryCurve(glm::vec3(gamePolys[1]->getCenter().x, gamePolys[1]->getCenter().y, gamePolys[1]->getCenter().z), 
		glm::vec3(gamePolys[1]->getCenter().x - gamePolys[2]->getCenter().x, gamePolys[1]->getCenter().y - gamePolys[2]->getCenter().y, gamePolys[1]->getCenter().z - gamePolys[2]->getCenter().z), 
		glm::vec3(gamePolys[2]->getCenter().x, gamePolys[2]->getCenter().y, gamePolys[2]->getCenter().z));
	trajectoryCurve->init(program);

	// Set the mass of heavier objects
	gamePolys[0]->setMass(99);

	// Set the mass of the pins
	for(int i=1; i<sizeOfGamePolys; i++)
	{
		gamePolys[i]->setMass(1);
	}

	// Menu
	sizeOfMenuPolys = 1;
	menuPolys = new Polyhedron*[sizeOfMenuPolys];
	menuPolys[0] = new Cube(); // testing menu
	
	// PolyControllers
	game = new PolyController(gamePolys, sizeOfGamePolys);
	menu = new PolyController(menuPolys, sizeOfMenuPolys);
	game->init(program);
	menu->init(program);

	// GameController
	gameController = new GameController(gamePolys[1], NULL, trajectoryCurve);
	gameController->start();

	bezierSurfaceController = new BezierSurfaceController(trajectoryCurve, program);

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
		//Render the bezier surface
		bezierSurfaceController->display();
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
			if(screenState==MENU)
			{
				screenState=GAME;
			}
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
	delete bezierSurfaceController;
	delete octree;
	
#ifdef TEST_MODE
	delete test;
	delete testOctree;
#endif

    return 0;
}

