#ifndef _GCONTROL_
#define _GCONTROL_

#include "Polyhedron.h"
#include "TrajectoryCurve.h"

#define TURN_BEGIN 0	// beginning of turn where user input is collected
#define ROLLING	1		// ball rolling down the lane
#define INACTIVE 2		// in an unrelated game-state such as 

#define BALL1 0
#define BALL2 1

// Score object represents a bowling frame
struct score
{
	int turnScore;		// the score for this frame
	int totalScore;		// turn score + total up to this point
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

	void checkBallDone();
	bool checkBallFell();

	// Scoring
	void processScore(int);
	void printScores();

	// Input
	void processInput(unsigned char);

	int getGameState();

	TrajectoryCurve* trajectoryCurve;

private:
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
