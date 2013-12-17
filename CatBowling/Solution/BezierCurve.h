#ifndef BEZIER_CURVE_H
#define BEZIER_CURVE_H

#include "GL/glew.h"
#include "Globals.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "MathFunctions.h"						// Factorial Function
#include <vector>
#include <iostream>

class BezierCurve
{

private:
	std::vector<glm::vec4> _controlPoints;							// The Control Points that calculate the Curve
	std::vector<glm::vec4> _pointsOnTheCurve;						// Points that are a part of the curve

	std::vector<glm::vec4>  _colorCurvePoints;						// Colors of the Curve
	std::vector<glm::vec4>  _colorControlPoints;					// Colors of the Control Points
	std::vector<glm::vec4>  _colorControlLines;						// Color of the lines connecting the Control Points

	unsigned __int8 _numberOfTimeStamps;							// Number of TimeSteps to generate Curve

	GLuint _curveVertexbuffer;										// Buffers for Data
	GLuint _controlVertexbuffer;	

	glm::vec4 _startColor;											// Color at timestep 0
	glm::vec4 _endColor;											// Color at last timestep

	static const glm::vec4 _controlPointsColor;						// Control Points Colors of Points and Lines
	static const glm::vec4 _controlLinesColor;

public:

	// Constructor
	BezierCurve( std::vector<glm::vec4> points, unsigned __int8 numberOfTimeStamps = 20, glm::vec4 startColor = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), glm::vec4 endColor = glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );

	// Deconstructor
	~BezierCurve(void);

	// Create points on the Curve from Control Points
	// Explicit definition -- http://en.wikipedia.org/wiki/B%C3%A9zier_curve
	void createPoints();

	// Fill Colors from start and end color
	void fillColor();

	// Display the Curve
	void display();

	// Properties
	std::vector<glm::vec4>* controlPoints(){ return &_controlPoints; }		// Return the address of the control points

	GLuint program;
	GLuint vao;
	bool debugVisible;
};

#endif

