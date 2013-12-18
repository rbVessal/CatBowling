#include "Angel.h"
#include "glm\gtc\type_ptr.hpp"  //for using value_ptr to get an array of floats to pass in to shader
#include "Polyhedron.h"
#include "Cube.h"

class TextRender
{
public:
	//parameterized constructor
	TextRender(GLuint program);
	//Use this to init the text
	void init(GLuint program);
	//Call this to keep redrawing the text
	void display();
	
	~TextRender(void);
private:
	bool isDisplayed;
	Polyhedron** polyArray;
	int sizeOfPolyArray;
	
};

