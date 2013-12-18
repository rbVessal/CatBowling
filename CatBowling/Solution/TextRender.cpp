#include "TextRender.h"


TextRender::TextRender(GLuint program)
{
	sizeOfPolyArray = 6;
	polyArray = new Polyhedron*[sizeOfPolyArray];
	//C
	//Top C
	Cube* topC = new Cube(0.0f, 0.3f, 1.0f, 0.5f, 0.5f, 0.1f, false);
	topC->init(program);
	topC->scale(glm::vec3(0.2f, 1.0f, 1.0f));
	topC->shear(glm::vec2(5.0f, 1.0f));
	//Bottom C
	Cube* bottomC = new Cube(0.0f, -1.0f, 1.0f, 0.5f, 0.5f, 0.1f, false);
	bottomC->init(program);
	bottomC->scale(glm::vec3(0.2f, 1.0f, 1.0f));
	bottomC->shear(glm::vec2(-6.0f, 1.0f));

	//B
	//Vertical B
	Cube* verticalB = new Cube(0.5f, -1.0f, 1.0f, 0.5f, 0.5f, 0.1f, false);
	verticalB->init(program);
	verticalB->scale(glm::vec3(0.2f, 3.0f, 1.0f));
	//bottom right B
	Cube* bottomRightB = new Cube(0.5f, -0.7f, 1.0f, 0.5f, 0.5f, 0.1f, false);
	bottomRightB->init(program);
	bottomRightB->scale(glm::vec3(0.2f, 0.5f, 1.0f));
	bottomRightB->shear(glm::vec2(5.0f, 1.0f));
	//top right B
	Cube* topRightBottomB = new Cube(0.5f, 0.3f, 1.0f, 0.5f, 0.5f, 0.1f, false);
	topRightBottomB->init(program);
	topRightBottomB->scale(glm::vec3(0.2f, 1.0f, 1.0f));
	topRightBottomB->shear(glm::vec2(-6.0f, 1.0f));
	//Middle of B
	Cube* bDivide = new Cube(0.8f, -0.2f, 1.0f, 0.5f, 0.5f, 0.1f, false);
	bDivide->init(program);
	bDivide->scale(glm::vec3(0.6f, 0.02f, 1.0f));

	polyArray[0] = topC;
	polyArray[1] = bottomC;
	polyArray[2] = verticalB;
	polyArray[3] = bottomRightB;
	polyArray[4] = topRightBottomB;
	polyArray[5] = bDivide;
}

//Draw all of the text using cubes for the appropriate screens
void TextRender::display()
{
	for(int i = 0; i < sizeOfPolyArray; i++)
	{
		polyArray[i]->display();
	}
}

TextRender::~TextRender(void)
{
	for(int i = 0; i < sizeOfPolyArray; i++)
	{
		delete polyArray[i];
	}
	delete polyArray;
}
