#include "PolyController.h"


PolyController::PolyController(void)
{
	// Default constructor -> menu has no polyhedrons
	polyArray = NULL;
	sizeOfPolyArray = 0;
}

PolyController::PolyController(Polyhedron** array, int size)
{
	polyArray = array;
	sizeOfPolyArray = size;
}

PolyController::PolyController(const PolyController& other)
{
	sizeOfPolyArray = other.sizeOfPolyArray;
	polyArray = new Polyhedron*[sizeOfPolyArray];

	for(int i = 0; i < sizeOfPolyArray; i++)
	{
		polyArray[i] = new Polyhedron(*other.polyArray[i]);
	}
}

const PolyController& PolyController::operator=(const PolyController& other)
{
	if(&other != this)
	{
		sizeOfPolyArray = other.sizeOfPolyArray;
		polyArray = new Polyhedron*[sizeOfPolyArray];

		for(int i = 0; i < sizeOfPolyArray; i++)
		{
			polyArray[i] = new Polyhedron(*other.polyArray[i]);
		}
	}
	return *this;
}

PolyController::~PolyController(void)
{
	// Delete polyhedrons
	for(int i = 0; i < sizeOfPolyArray; i++)
	{
		delete polyArray[i];
	}
	delete polyArray;
}

void PolyController::init(GLuint program)
{
	// Init polyhedrons
	for(int i = 0; i < sizeOfPolyArray; i++)
	{
		Polyhedron* polyhedron = polyArray[i];
		polyhedron->init(program);
	}
}

void PolyController::display()
{
	// Render polyhedrons
	for(int i = 0; i < sizeOfPolyArray; i++)
	{
		Polyhedron* polyhedron = polyArray[i];
		polyhedron->display();
	}
}