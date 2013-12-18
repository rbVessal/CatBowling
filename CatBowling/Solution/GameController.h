#ifndef _GCONTROL_
#define _GCONTROL_

#include "Polyhedron.h"
#include "TrajectoryCurve.h"
#include <Windows.h>
#include <MMSystem.h>


#define TURN_BEGIN 0	// beginning of turn where user input is collected
#define ROLLING	1		// ball rolling down the lane
#define INACTIVE 2		// state where user interaction with ball is turned off AND ball is not rolling

#define BALL1 0
#define BALL2 1

// Score object represents a bowling frame
struct score
{
	// Both scores for this frame and the total game score
	int turnScore1;
	int turnScore2;
	int totalScore;
};

class GameController
{
public:
	GameController(void);
	GameController(Polyhedron*, Polyhedron**, TrajectoryCurve* trajectoryCurve);
	~GameController(void);

	// Standard game functions
	void start();
	void update();
	void display();
	void endGame();

	void checkBallDone();
	bool checkBallFell();

	// Scoring
	void processPins();
	void processScore(int);
	void printScores();

	// Input
	void processInput(unsigned char);

	int getGameState();

	TrajectoryCurve* trajectoryCurve;

private:

	bool isSoundLooping;
	bool renderTrajectoryCurve;
	Polyhedron* ball;
	Polyhedron* pins[10];
	
	int gameState;
	int turnState;

	score frames[10];	// all 10 frames with score info
	int gameScore;		// absolute total of game points
	int turnNumber;

	// Event functions from input
	void adjustRotation(float);
	void adjustStrafing(float);
	void launchBall();
	
};
#endif
