#include "GameController.h"


GameController::GameController(void)
{
	gameState = INACTIVE;
	turnState = BALL1;
	gameScore = 0;
	ball =  NULL;

	for(int i=0; i<10; i++)
	{
		pins[i] = NULL;
	}
}

GameController::GameController(Polyhedron* ballPoly, Polyhedron** pinArray)
{
	gameState = INACTIVE;
	turnState = BALL1;
	gameScore = 0;
	ball =  ballPoly;

	// Assumes the there are exactly 10 pins in the array
	// Because this game isn't going to be right otherwise.
	if(pinArray != NULL)
	{
		for(int i=0; i<10; i++)
		{
			pins[i] = pinArray[i];
		}
	}
	else
	{
		for(int i=0; i<10; i++)
		{
			pins[i] = NULL;
		}
	}

	// Print welcome message
	std::cout << "\n-----------------------------" << std::endl;
	std::cout << "-------- CAT BOWLING --------\n" << std::endl;
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
}

void GameController::start()
{
	gameState = TURN_BEGIN;
	turnNumber = 0;

	for(int i=0; i<10; i++)
	{
		frames[i].totalScore = 0;
		frames[i].turnScore = 0;
	}
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
		// TODO: game end
		// ...

		// Reset everything
		ball->resetPolyhedron();
		for(int i=0; i<10; i++)
		{
			pins[i]->resetPolyhedron();
		}
		start();
	}
}

void GameController::checkBallDone()
{
	// Visiblity detection if ball has reached end
	if(checkBallFell() || ball->getVisiblity() == false)
	{
		// Restart ball, update score, display score
		gameState = TURN_BEGIN;
		processScore(1);
		printScores();

		// Next ball in turn
		if(turnState == BALL1)
		{
			turnState = BALL2;

			// reset ball but not pins
			ball->resetPolyhedron();

		}
		// Next turn
		else if(turnState == BALL2)
		{
			turnState = BALL1;
			turnNumber++;

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

void GameController::processScore(int n)
{
	frames[turnNumber].turnScore += n;

	// Update total score
	if(turnState == BALL2)
	{
		frames[turnNumber].totalScore += frames[turnNumber].turnScore;

		// Chain along the total score to the next frame
		if(turnNumber < 9)
		{
			frames[turnNumber+1].totalScore = frames[turnNumber].totalScore;
		}
	}
}

void GameController::printScores()
{
	std::cout << "Frame " << turnNumber+1 << std::endl;
	std::cout << "Turn Score: " << frames[turnNumber].turnScore << std::endl;
	std::cout << "Total Score: " << frames[turnNumber].totalScore << std::endl;
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
	ball->setVelocityLocal(0, 0, -0.075);
	gameState = ROLLING;
}
