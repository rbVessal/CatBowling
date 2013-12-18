#include "GameController.h"

#define NUMBER_OF_PINS 10

GameController::GameController(void)
{
	gameState = INACTIVE;
	turnState = BALL1;
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
	turnState = BALL1;
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

	// Print welcome message
	std::cout << "\n-----------------------------" << std::endl;
	std::cout << "-------- CAT BOWLING --------\n" << std::endl;
	std::cout << "Authors:" << std::endl;
	std::cout << "Rebecca Vessal and Jennifer Stanton\n" << std::endl;
	std::cout << "Controls:" << std::endl;
	std::cout << "space - start game (from menu)" << std::endl;
	std::cout << "r - reset game" << std::endl;
	std::cout << "p - pause game\n" << std::endl;
	std::cout << "j/k - move ball left/right" << std::endl;
	std::cout << "d/f - rotate ball left/right" << std::endl; 
	std::cout << "b - bowl" << std::endl;
	std::cout << "-----------------------------" << std::endl;
}

GameController::~GameController(void)
{
	delete trajectoryCurve;
}

void GameController::start()
{
	gameState = TURN_BEGIN;
	turnNumber = 0;

	for(int i=0; i<10; i++)
	{
		frames[i].totalScore = 0;
		frames[i].turnScore1 = 0;
		frames[i].turnScore2 = 0;
	}
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

		// Rolling
		ball->rotate(-5, glm::vec3(1, 0, 0));

		// Do next turn?
		checkBallDone();
	}

	// Visiblity detection for incorrect ball invisibility
	if(gameState != ROLLING && ball->getVisiblity() == false)
	{
		ball->setVisiblity(true);
	}

	if(turnNumber == 10)
	{
		// Reset everything
		ball->resetPolyhedron();
		for(int i=0; i<10; i++)
		{
			pins[i]->resetPolyhedron();
		}
		start(); // reset values
		endGame(); // go to credits screen
	}
}

void GameController::endGame()
{
	// Print credits
	std::cout << "\n-----------------------------" << std::endl;
	std::cout << "----------- CREDITS ----------\n" << std::endl;
	std::cout << "Real Time Collision Detection by Christer Ericson" << std::endl;
	std::cout << "Math Primer for Graphics and Game Development by Fletcher Dunn and Ian Parberry" << std::endl;
	std::cout << "gamedev.net" << std::endl;
	std::cout << "stackoverflow.com" << std::endl;
	std::cout << "physics2d.com" << std::endl;
	std::cout << "www.opengl-tutorial.org" << std::endl; 
	std::cout << "t-machine.org" << std::endl;
	std::cout << "g-truc.net" << std::endl;
	std::cout << "-----------------------------\n" << std::endl;
	std::cout << "Press 'r' for the menu." << std::endl;
}

void GameController::checkBallDone()
{
	// Visiblity detection if ball has reached end
	if(checkBallFell() || ball->getVisiblity() == false)
	{
		// Restart ball, update score, display score
		gameState = TURN_BEGIN;
		//processScore(1);
		processPins();
		
		printScores();

		// Next ball in turn
		if(turnState == BALL1)
		{
			turnState = BALL2;
			std::cout << "Frame " << turnNumber+1 << std::endl;

			// reset ball but not pins
			ball->resetPolyhedron();
		}
		// Next turn
		else if(turnState == BALL2)
		{
			turnState = BALL1;
			turnNumber++;

			if(turnNumber < 10)
			{
				std::cout << "Frame " << turnNumber+1 << std::endl;
			}

			// reset ball and pins
			ball->resetPolyhedron();
			for(int i=0; i<10; i++)
			{
				pins[i]->resetPolyhedron();
			}
		}
	}
}

bool GameController::checkBallFell()
{
	// If the ball fell far
	if(ball->getPosition().y < -10 || ball->getPosition().z < -8)
	{
		return true;
	}
	return false;
}

void GameController::processPins()
{
	int fallenPins = 0;

	for(int i=0; i<10; i++)
	{
		glm::vec3 v = pins[i]->getVelocity();
		if(v.x > 0 || v.y > 0 || v.z > 0)
		{
			fallenPins++;
			pins[i]->setVelocity(0, 0, 0);
			pins[i]->setVisiblity(false);
		}
	}

	processScore(fallenPins);
}

void GameController::processScore(int n)
{
	if(turnState == BALL1)
	{
		frames[turnNumber].turnScore1 += n;
	}
	else
	{
		frames[turnNumber].turnScore2 += n;
	
		// Update total score
		frames[turnNumber].totalScore += frames[turnNumber].turnScore1;
		frames[turnNumber].totalScore += frames[turnNumber].turnScore2;

		// Chain along the total score to the next frame
		if(turnNumber < 9)
		{
			frames[turnNumber+1].totalScore = frames[turnNumber].totalScore;
		}
	}
}

void GameController::printScores()
{
	std::cout << "Turn 1: " << frames[turnNumber].turnScore1 << std::endl;
	std::cout << "Turn 2: " << frames[turnNumber].turnScore2 << std::endl;
	std::cout << "Total: " << frames[turnNumber].totalScore << std::endl;
	std::cout << "-----------------------------" << std::endl;
}

void GameController::processInput(unsigned char key)
{
	if(gameState == TURN_BEGIN)
	{
		switch(key) 
		{
			// React to keyboard event
			case 'f': adjustRotation(1); break;
			case 'd': adjustRotation(-1); break;

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
	if(turnNumber == 0 && turnState == BALL1)
	{
		std::cout << "Frame " << turnNumber+1 << std::endl;
	}

	ball->setVelocityLocal(0, 0, -0.075);
	gameState = ROLLING;
}
