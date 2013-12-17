#include "LineSegment.h"


LineSegment::LineSegment(void)
{
}

LineSegment::LineSegment(glm::vec3 p1, glm::vec3 p2)
{
	endPoint1 = p1;
	endPoint2 = p2;
}

LineSegment::~LineSegment(void)
{
}


bool LineSegment::checkCollision(Collider* other)
{
	//glm::vec3 point = other->getClosestPointToPoint(endPoint1, endPoint2);
	if(this != other)
	{
		float squaredDist = other->getDistanceToLine(endPoint1, endPoint2);

		if(squaredDist < 2.5)
		{
			std::cout << "line collision!" << std::endl;
			return true;
		}
	}

	return false;
}