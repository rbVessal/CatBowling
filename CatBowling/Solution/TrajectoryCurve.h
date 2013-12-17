#ifndef TRAJECTORY_CURVE_H
#define TRAJECTORY_CURVE_H


/*#include <GL/glew.h>//Have to include these three for use modern OpenGL for Windows see: Angel.h
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>*/
#include "Angel.h"
#include "glm\gtc\type_ptr.hpp"  //for using value_ptr to get an array of floats to pass in to shader
#include <math.h>//For pow see:http://www.cplusplus.com/reference/cmath/pow/

typedef glm::vec4  color4;
class TrajectoryCurve
{
public:
	//default constructor
	TrajectoryCurve(void);
	//parameterized constructor
	TrajectoryCurve(glm::vec3 beginningPoint, glm::vec3 centerPoint, glm::vec3 endPoint);
	//Use this to init the bezier curve
	void init(GLuint program);
	//Generate points and colors for the bezier curve
	void draw();
	//Call this to keep redrawing the bezier curve
	void display();
	

	~TrajectoryCurve(void);
private:
	glm::vec3 beginningPoint;//static control point
	glm::vec3 centerPoint;//control point you manipulate with mouse
	glm::vec3 endPoint;//static control point
	//t is the parameter for the bezier equation and ranges from 0 - 1
	//Used to generate the points on the bezier curve
	//Vertices of bezier curve
	glm::vec4* colors;
	glm::vec4* points;
	GLuint vbo;
	GLuint vao;
	GLuint vColor;
	GLuint vPosition;
	GLuint view;
	GLuint projection;
	GLuint transformationMatrix;
	GLfloat radius;
	GLfloat theta;
	GLfloat phi;

	GLfloat  left, right;
	GLfloat  bottom, top;
	GLfloat  zNear, zFar;
	//Composite Model Transformation Matrix
	glm::mat4 compositeModelTransformationMatrix;
	
	void TrajectoryCurve::setupVAO(GLuint program);
	void TrajectoryCurve::setupVBO();


};
#endif

