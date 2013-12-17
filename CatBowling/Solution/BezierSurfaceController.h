#ifndef BEZIER_SURFACE_CONTROLLER_H
#define BEZIER_SURFACE_CONTROLLER_H

#include <vector>
#include "BezierCurve.h"
#include "BezierSurface.h"
#include "TrajectoryCurve.h"

class BezierSurfaceController
{
public:
	BezierSurfaceController(void);
	BezierSurfaceController(TrajectoryCurve* trajectoryCurve, GLuint program);
	~BezierSurfaceController(void);

	void display();
private:
	TrajectoryCurve* trajectoryCurve;
	bool curvesDisplay, surfacesDisplay;		// Whether to display the objects or not
	std::vector<BezierCurve *> curves;
	std::vector<BezierSurface *> surfaces;
	GLuint program;

	//Taken from Matt's example of bezier surface and curves
	// Curves
	void createCurves( std::vector<BezierCurve *>& curves );
	void removeCurves( std::vector<BezierCurve *>& curves );

	// Surfaces
	void createSurfaces( std::vector<BezierSurface *>& surfaces );
	void removeSurfaces( std::vector<BezierSurface *>& surfaces );
};
#endif

