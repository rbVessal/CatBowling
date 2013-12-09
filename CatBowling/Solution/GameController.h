#ifndef _GCONTROL_
#define _GCONTROL_

#include "Polyhedron.h"

#define TURN_BEGIN 0	// beginning of turn where user input is collected
#define ROLLING	1		// ball rolling down the lane
#define INACTIVE 2		// in an unrelated game-state such as menu 

// Score object represents a bowling frame
struct score
{
	int turnScore;		// the score for this frame
	int totalScore;		// turn score + total up to this point
	bool hasBegun;		// whether this frame has begun or not
};

class GameController
{
public:
	GameController(void);
	GameController(Polyhedron*, Polyhedron**);
	~GameController(void);

	// Standard game functions
	void start();
	void update();

	// Input
	void processInput(unsigned char);

private:
	Polyhedron* ball;
	Polyhedron* pins[10];
	
	int gameState;

	score frames[10];	// all 10 frames with score info
	int gameScore;		// absolute total of game points

	// Event functions from input
	void adjustRotation(float);
	void adjustStrafing(float);
	void launchBall();
};
#endif
