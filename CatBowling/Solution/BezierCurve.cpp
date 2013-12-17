#include "BezierCurve.h"
#include <vector>

using namespace std;
using namespace glm;

// Static variables
const vec4 BezierCurve::_controlPointsColor = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
const vec4 BezierCurve::_controlLinesColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f );

#pragma region Constructor
BezierCurve::BezierCurve( vector<vec4> points, unsigned __int8 numberOfTimeStamps, vec4 startColor, vec4 endColor )
	: _controlPoints( points ), _numberOfTimeStamps( numberOfTimeStamps ), _startColor( startColor ), _endColor( endColor )
{
	debugVisible = true;
	// Allocate enough space
	_pointsOnTheCurve = vector<vec4>( _numberOfTimeStamps + 1 );		// Start and End control points with intermediate influencing curve
	_colorCurvePoints = vector<vec4>( _numberOfTimeStamps + 1 );

	// Setup Control Points
	int controlPointsSize = _controlPoints.size();
	_colorControlPoints = vector<vec4>( controlPointsSize );
	_colorControlLines = vector<vec4>( controlPointsSize );
	// Make sure control points have a w
	for( int i = 0; i < controlPointsSize; ++i )
	{
		_controlPoints[i].w = 1.0f;
	}
	
	fillColor();

#pragma region Setup Points of the Curve
	glGenBuffers( 1, &_curveVertexbuffer );
	glBindBuffer( GL_ARRAY_BUFFER, _curveVertexbuffer );
	glBufferData( GL_ARRAY_BUFFER, _pointsOnTheCurve.size() * sizeof(vec4) + _colorCurvePoints.size() * sizeof(vec4),
			NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, _pointsOnTheCurve.size() * sizeof(vec4), &_pointsOnTheCurve[0] );
	glBufferSubData( GL_ARRAY_BUFFER, _pointsOnTheCurve.size() * sizeof(vec4), _colorCurvePoints.size() * sizeof(vec4), &_colorCurvePoints[0] );
#pragma endregion

#pragma region Setup Control Points and Lines
	glGenBuffers( 1, &_controlVertexbuffer );
	glBindBuffer( GL_ARRAY_BUFFER, _controlVertexbuffer );
	glBufferData( GL_ARRAY_BUFFER, ( _controlPoints.size() + _colorControlPoints.size() + _colorControlLines.size() ) * sizeof(vec4),
			NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, _controlPoints.size() * sizeof(vec4), &_controlPoints[0] );
	glBufferSubData( GL_ARRAY_BUFFER, _controlPoints.size() * sizeof(vec4), _colorControlPoints.size() * sizeof(vec4), &_colorControlPoints[0] );
	glBufferSubData( GL_ARRAY_BUFFER, ( _controlPoints.size() + _colorControlPoints.size() ) * sizeof(vec4), _colorControlLines.size() * sizeof(vec4), &_colorControlLines[0] );
#pragma endregion
	createPoints();
}

#pragma endregion

#pragma region Deconstructor
BezierCurve::~BezierCurve(void)
{
	std::cout << "curve deleted" << std::endl;
	glDeleteBuffers(1, &_curveVertexbuffer);
	glDeleteBuffers(1, &_controlVertexbuffer);
}
#pragma endregion

// Create points on the Curve
#pragma region CreatePoints
void BezierCurve::createPoints()
{
	unsigned __int8 CONTROL_POINT_COUNT = _controlPoints.size();
	unsigned __int8 n = CONTROL_POINT_COUNT - 1;

	unsigned __int8 i,j;			

	// Time Step through the Curve
	const float timeStepIncrement = 1.0f / _numberOfTimeStamps;
	float currentTimeStep = 0;	
	
	// Factorials 
	float nFactorial = Factorial( n );

	glm::vec4 tempVector;

	for( i = 0; i < ( _numberOfTimeStamps + 1 ); ++i )
	{
		tempVector = glm::vec4( 0, 0, 0, 1 );

		// Iterate through all control Points
		for( j = 0; j < CONTROL_POINT_COUNT; ++j )
		{					// n! / !i * (n - i)!
			tempVector += _controlPoints[j] * ( ( nFactorial / ( Factorial( j ) * Factorial( n - j ) ) ) * 
							static_cast<float>( pow( ( 1 - currentTimeStep ),  n - j ) * pow( currentTimeStep, j ) ) );
		}

		tempVector.w = 1;
		_pointsOnTheCurve[i] = tempVector;
		currentTimeStep += timeStepIncrement;
	}

	// Reupdate for every time we 'Move'(recalculate) the vertices
	glBindBuffer( GL_ARRAY_BUFFER, _curveVertexbuffer );
	glBufferSubData( GL_ARRAY_BUFFER, 0, _pointsOnTheCurve.size() * sizeof(vec4), &_pointsOnTheCurve[0] );
	glBindBuffer( GL_ARRAY_BUFFER, _controlVertexbuffer );
	glBufferSubData( GL_ARRAY_BUFFER, 0, _controlPoints.size() * sizeof(vec4), &_controlPoints[0] );
}

#pragma endregion

#pragma region FillColor
void BezierCurve::fillColor()
{
	// Color Bezier Curve
	vec4 iteration	= _endColor - _startColor;
	iteration /= _numberOfTimeStamps;

	// Translate from StartColor to EndColor
	for( int i = 0; i < (_numberOfTimeStamps + 1); ++i )
	{
		_colorCurvePoints[i] = _startColor;
		_startColor += iteration;
	}

	// Color Control Points and Lines
	int controlPointSize = _controlPoints.size();
	for( int i = 0; i < controlPointSize; ++i )
	{
		_colorControlPoints[i] = _controlPointsColor;
		_colorControlLines[i]  = _controlLinesColor;
	}
}
#pragma endregion

// Display the Curve
#pragma region Display
void BezierCurve::display() 
{
	
#pragma region Render Curve
	glBindBuffer( GL_ARRAY_BUFFER, _curveVertexbuffer );				// Bind buffer of the curve

	 // Use the vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(0) );

	GLuint vColor = glGetAttribLocation( program, "vColor" ); 
	glEnableVertexAttribArray( vColor );

	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(_pointsOnTheCurve.size() * sizeof(glm::vec4)) );

	glDrawArrays( GL_LINE_STRIP, 0, _pointsOnTheCurve.size() );

	//glDisableVertexAttribArray( vPosition );
	//glDisableVertexAttribArray( vColor );
#pragma endregion

	if( debugVisible )
	{
#pragma region Render Control  Lines
	glBindBuffer( GL_ARRAY_BUFFER, _controlVertexbuffer );

	 // Use the vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

	// set up vertex arrays
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(0) );

	glEnableVertexAttribArray( vColor );

	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET( ( _controlPoints.size() + _colorControlPoints.size() ) * sizeof(glm::vec4)) );

	glDrawArrays( GL_LINE_STRIP, 0, _controlPoints.size() );

	//glDisableVertexAttribArray( vPosition );
	//glDisableVertexAttribArray( vColor );
#pragma endregion

#pragma region Render Control Points
	glBindBuffer( GL_ARRAY_BUFFER, _controlVertexbuffer );

	 // Use the vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

	// set up vertex arrays
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET(0) );

	glEnableVertexAttribArray( vColor );

	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
				BUFFER_OFFSET( _controlPoints.size() * sizeof(glm::vec4)) );

	glDrawArrays( GL_POINTS, 0, _controlPoints.size() );

	//glDisableVertexAttribArray( vPosition );
	//glDisableVertexAttribArray( vColor );
#pragma endregion
	}
}
#pragma endregion
