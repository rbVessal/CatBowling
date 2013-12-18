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

// Check the type of collision
int LineSegment::calculateCollisionType(Collider* other)
{
	AABB* otherAABB = dynamic_cast<AABB*>(other);
	if(otherAABB)
	{
		return LINE_AABB;
	}
	else
	{
		return LINE_LINE;
	}
}

bool LineSegment::checkCollision(Collider* other)
{
	if(this != other)
	{
		if(calculateCollisionType(other) == LINE_AABB)
		{
			// Line to AABB (line to point) intersection
			AABB* aabb = dynamic_cast<AABB*>(other);

			float dist = checkAABB(aabb);
			if(dist < 1.0)
			{
				//std::cout << "line collision! " << dist << std::endl;
				return true;
			}
		}
	}

	return false;
}


// 3D Math Primer for Graphics and Game Development
// A.18, Intersection of a Ray and an AABB
float LineSegment::checkAABB(AABB* other)
{
	glm::vec3 centerPoint = other->getCenter();
	float* halfWidthExtents = other->gethalfWidths();
	float noIntersection = 0xff; // Max value to return if no intersection

	// Check for point inside box & determine distance to each face
	bool inside = true;

	// Treat the line segment like a ray with origin at endPoint1
	glm::vec3 rayDelta = endPoint2 - endPoint1;

	// - X -
	float xt, xn;
	// Min x
	if(endPoint1.x < centerPoint.x - halfWidthExtents[0])
	{
		// xt = min.x - p1.x
		xt = centerPoint.x - halfWidthExtents[0] - endPoint1.x;

		if(xt > rayDelta.x)
		{
			return noIntersection;
		}

		xt /= rayDelta.x;
		inside = false;
		xn = -1.0;
	}
	// Max x
	else if(endPoint1.x > centerPoint.x + halfWidthExtents[0])
	{
		// xt = max.x - p1.x
		xt = centerPoint.x + halfWidthExtents[0] - endPoint1.x;

		if(xt < rayDelta.x)
		{
			return noIntersection;
		}

		xt /= rayDelta.x;
		inside = false;
		xn = 1.0;
	}
	else
	{
		xt = -1.0;
	}


	// - Y -
	float yt, yn;
	// Min y
	if(endPoint1.y < centerPoint.y - halfWidthExtents[1] )
	{
		// yt = min.y - p1.y
		yt = centerPoint.y - halfWidthExtents[1] - endPoint1.y;

		if(yt > rayDelta.y)
		{
			return noIntersection;
		}

		yt /= rayDelta.y;
		inside = false;
		yn = -1.0;
	}
	// Max y
	else if(endPoint1.y > centerPoint.y + halfWidthExtents[1])
	{
		// yt = max.y - p1.y
		yt = centerPoint.y + halfWidthExtents[1] - endPoint1.y;

		if(yt < rayDelta.y)
		{
			return noIntersection;
		}

		yt /= rayDelta.y;
		inside = false;
		yn = 1.0;
	}
	else
	{
		yt = -1.0;
	}


	// - Z -
	float zt, zn;
	// Min z
	if(endPoint1.z < centerPoint.z - halfWidthExtents[2] )
	{
		// zt = min.z - p1.z
		zt = centerPoint.z - halfWidthExtents[2] - endPoint1.z;

		if(zt > rayDelta.z)
		{
			return noIntersection;
		}

		zt /= rayDelta.z;
		inside = false;
		zn = -1.0;
	}
	// Max z
	else if(endPoint1.z > centerPoint.z + halfWidthExtents[2])
	{
		// zt = max.z - p1.z
		zt = centerPoint.z + halfWidthExtents[2] - endPoint1.z;

		if(zt < rayDelta.z)
		{
			return noIntersection;
		}

		zt /= rayDelta.z;
		inside = false;
		zn = 1.0;
	}
	else
	{
		zt = -1.0;
	}

	// P1 inside box?
	if(inside)
	{
		return 0.0f;
	}

	// Select the farthest plane, which is the plane of intersection
	int which = 0;
	float t = xt;
	if(yt > t)
	{
		which = 1;
		t = yt;
	}
	if(zt > t)
	{
		which = 2;
		t = zt;
	}

	// Used in switch statement
	float x = endPoint1.x + rayDelta.x * t;
	float y = endPoint1.y + rayDelta.y * t;
	float z = endPoint1.z + rayDelta.z * t;

	switch(which)
	{
	// intersects yz plane
	case 0:
		if(y < centerPoint.y - halfWidthExtents[1] || y > centerPoint.y + halfWidthExtents[1])
		{
			return noIntersection;
		}
		if(z < centerPoint.z - halfWidthExtents[2] || z > centerPoint.z + halfWidthExtents[2])
		{
			return noIntersection;
		}
		break;
	// intersects xz plane
	case 1:
		
		if(x < centerPoint.x - halfWidthExtents[0] || x > centerPoint.x + halfWidthExtents[0])
		{
			return noIntersection;
		}
		if(z < centerPoint.z - halfWidthExtents[2] || z > centerPoint.z + halfWidthExtents[2])
		{
			return noIntersection;
		}
		break;
	// intersects xy plane
	case 2:
		if(x < centerPoint.x - halfWidthExtents[0] || x > centerPoint.x + halfWidthExtents[0])
		{
			return noIntersection;
		}
		if(y < centerPoint.y - halfWidthExtents[1] || y > centerPoint.y + halfWidthExtents[1])
		{
			return noIntersection;
		}
		break;
	}

	return t;
}
