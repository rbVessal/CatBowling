#include "BezierSurfaceController.h"


BezierSurfaceController::BezierSurfaceController(void)
{
	curvesDisplay = true;
	surfacesDisplay = true;

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	//Create the curves and surfaces for the bezier surface
	createCurves(curves);
	createSurfaces(surfaces);
}

BezierSurfaceController::BezierSurfaceController(TrajectoryCurve* trajectoryCurve, GLuint program)
{
	this->program = program;
	this->trajectoryCurve = trajectoryCurve;
	curvesDisplay = true;
	surfacesDisplay = true;

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	//Create the curves and surfaces for the bezier surface
	createCurves(curves);
	createSurfaces(surfaces);
	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

//Taken from Matt's example of bezier curves and surfaces
#pragma region Create Curves
void BezierSurfaceController::createCurves( std::vector<BezierCurve *>& curves )
{
	std::vector<glm::vec4> controlPoints;
	controlPoints.push_back( glm::vec4( -2.0f, 0.0f, 0.0f, 1.0f ) );
	controlPoints.push_back( glm::vec4( 0.0f, 5.0f, 5.0f, 1.0f ) );
	controlPoints.push_back( glm::vec4( 2.0f, 5.0f, 0.0f, 1.0f ) );
	controlPoints.push_back( glm::vec4( 2.0f, 2.0f, 0.0f, 1.0f ) );
	BezierCurve* bezierCurve = new BezierCurve(controlPoints);
	bezierCurve->program = program;
	curves.push_back( new BezierCurve(controlPoints));


	std::vector<glm::vec4> controlPoints1;
	controlPoints1.push_back( glm::vec4( -2.0f, -2.0f, 0.0f, 1.0f ) );
	controlPoints1.push_back( glm::vec4( 3.0f, 5.0f, 5.0f, 1.0f ) );
	controlPoints1.push_back( glm::vec4( 3.0f, 2.0f, 1.0f, 1.0f ) );
	controlPoints1.push_back( glm::vec4( 5.0f, 2.0f, 0.0f, 1.0f ) );
	BezierCurve* bezierCurve2 = new BezierCurve(controlPoints1);
	bezierCurve2->program = program;
	curves.push_back( new BezierCurve(controlPoints1));
}
#pragma endregion

#pragma region Remove Curves
void BezierSurfaceController::removeCurves( std::vector<BezierCurve *>& curves )
{
	int curvesLength = curves.size();
	for( int i = curvesLength -1; i > -1; i--)
	{
		delete curves[i];
	}

	// empty vector
	curves.clear();
}
#pragma endregion

#pragma region Create Surfaces
void BezierSurfaceController::createSurfaces( std::vector<BezierSurface *>& surfaces )
{
	std::vector<glm::vec4> control1Points;
	control1Points.push_back( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	control1Points.push_back( glm::vec4( 3.0f, 0.0f, 1.0f, 1.0f ) );
	control1Points.push_back( glm::vec4( 6.0f, 1.0f, 0.0f, 1.0f ) );
	control1Points.push_back( glm::vec4( 10.0f, 0.0f, 0.0f, 1.0f ) );

	std::vector<glm::vec4> control2Points;
	control2Points.push_back( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	control2Points.push_back( glm::vec4( 1.0f, 3.0f, 0.0f, 1.0f ) );
	control2Points.push_back( glm::vec4( 0.0f, 6.0f, 1.0f, 1.0f ) );
	control2Points.push_back( glm::vec4( 0.0f, 10.0f, 0.0f, 1.0f ) );

	BezierSurface* bezierSurface = new BezierSurface(control1Points, control2Points);
	bezierSurface->program = program;
	surfaces.push_back( bezierSurface);

}
#pragma endregion

#pragma region Remove Surfaces
void BezierSurfaceController::removeSurfaces( std::vector<BezierSurface *>& surfaces )
{
	int surfacesLength = surfaces.size();
	for( int i = surfacesLength -1; i > -1; i--)
	{
		delete surfaces[i];
	}

	// empty vector
	surfaces.clear();


}
#pragma endregion

void BezierSurfaceController::display()
{
	//Render the bezier surfaces and curves	
	//Code taken from Matt's Bezier Surface example
	curves[0]->createPoints();
	curves[1]->createPoints();
	surfaces[0]->createPoints();
	trajectoryCurve->display();
	
#pragma region Curves
	if( curvesDisplay )
	{
		int curvesLength = curves.size();
		for( int i = 0; i < curvesLength; ++i )
		{
			curves[i]->display();
		}
	}
#pragma endregion

#pragma region Surface
	if( surfacesDisplay )
	{	
		int surfaceLength = surfaces.size();
		for( int i = 0; i < surfaceLength; ++i )
		{
			surfaces[i]->display();
		}
	}
#pragma endregion

}

BezierSurfaceController::~BezierSurfaceController(void)
{
	removeCurves(curves);
	removeSurfaces(surfaces);
}
