#ifndef BEZIER_SURFACE_H
#define BEZIER_SURFACE_H

#include "GL/glew.h"
#include "Globals.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "MathFunctions.h"
#include <vector>

class BezierSurface
{
private:
	std::vector<glm::vec4> _controlPoints1;							// The Control Points1 that calculates the Surface
	std::vector<glm::vec4> _controlPoints2;							// The Control Points2 that calculates the Surface

	std::vector<std::vector<glm::vec4>> _pointsOnTheSurface;		// Points that are a part of the Surface

	std::vector<glm::vec4> _vertices;								// Verticies and Color of Surface
	std::vector<glm::vec4> _colors;

	GLuint _curveVertexbuffer;										// Buffers, Note for example Control Points 1 and 2 can be different sizes
	GLuint _controlVertexbuffer1;
	GLuint _controlVertexbuffer2;

	glm::vec4 _bottomLeftColor;										// Colors of the Surface
	glm::vec4 _bottomRightColor;
	glm::vec4 _topLeftColor;
	glm::vec4 _topRightColor;

	unsigned __int8 _numberOfTimeStamps;							// Number of timesteps, Note could have had two for each 'curve'

	static const glm::vec4 _controlPointsColor1;					// Static colors to display control points and lines
	static const glm::vec4 _controlLinesColor1;
	static const glm::vec4 _controlPointsColor2;
	static const glm::vec4 _controlLinesColor2;

	std::vector<glm::vec4>  _colorControlPoints1;					// Colors of the Control Points1
	std::vector<glm::vec4>  _colorControlLines1;

	std::vector<glm::vec4>  _colorControlPoints2;					// Colors of the Control Points2
	std::vector<glm::vec4>  _colorControlLines2;

public:

	// Constructor
	BezierSurface( std::vector<glm::vec4> points1, std::vector<glm::vec4> points2, unsigned __int8 numberOfTimeStamps = 20, 
		glm::vec4 bottomLeftColor = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ), glm::vec4 bottomRightColor = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), 
		glm::vec4 topLeftColor = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ), glm::vec4 topRightColor = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ));

	// Deconstructor
	~BezierSurface(void);

	// Create points on the Curve
	void createPoints();

	void fillBuffer();

	void fillColors();

	// Get the Color based on current location of curve
	// Influenced on corner colors
	glm::vec4 getColor( int widthIndex, int heightIndex, int widthSize, int heightSize );

	// Displays the Curve
	void display();

	// Properties 
	std::vector<glm::vec4>* controlPoints1(){ return &_controlPoints1; }		// Get address of control Points
	std::vector<glm::vec4>* controlPoints2(){ return &_controlPoints2; }

	GLuint program;
	GLuint vao;
	bool debugVisible;
};

#endif



