#include "GameController.h"

#define NUMBER_OF_PINS 10

GameController::GameController(void)
{
	gameState = INACTIVE;
	gameScore = 0;
	ball =  NULL;
	renderTrajectoryCurve = true;


	for(int i=0; i<10; i++)
	{
		pins[i] = NULL;
	}
}

GameController::GameController(Polyhedron* ballPoly, Polyhedron** pinArray, TrajectoryCurve* trajectoryCurve)
{
	this->trajectoryCurve = trajectoryCurve;
	gameState = INACTIVE;
	gameScore = 0;
	ball =  ballPoly;

	// Assumes the there are exactly 10 pins in the array
	// Because this game isn't going to be right otherwise.
	if(pinArray != NULL)
	{
		for(int i=0; i<NUMBER_OF_PINS; i++)
		{
			pins[i] = pinArray[i];
		}
	}
	else
	{
		for(int i=0; i<NUMBER_OF_PINS; i++)
		{
			pins[i] = NULL;
		}
	}
}

GameController::~GameController(void)
{
	delete trajectoryCurve;
}

void GameController::start()
{
	gameState = TURN_BEGIN;
}

int GameController::getGameState()
{
	return gameState;
}

void GameController::display()
{
	trajectoryCurve->display();
}

void GameController::update()
{
	if(gameState == ROLLING)
	{
		// Gravity
		ball->addForce(0, -0.00001, 0);
	}
}

void GameController::processInput(unsigned char key)
{
	if(gameState == TURN_BEGIN)
	{
		switch(key) 
		{
			// React to keyboard event
			case 'f': adjustRotation(-1); break;
			case 'd': adjustRotation(1); break;

			case 'j': adjustStrafing(-0.05); break;
			case 'k': adjustStrafing(0.05); break;

			case 'b': launchBall(); break;
		}
	}
}

//Adjust the rotation of the ball using a quaternion
void GameController::adjustRotation(float degree)
{
	ball->rotate(degree, glm::vec3(0, 1, 0));
}

void GameController::adjustStrafing(float degree)
{
	ball->translate(degree, 0, 0);
}

void GameController::launchBall()
{
	ball->setVelocity(0, 0, -0.01);
	gameState = ROLLING;
}
