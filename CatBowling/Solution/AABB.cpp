#include "AABB.h"


AABB::AABB(void)
{
}

AABB::AABB(glm::vec3 center, float half0, float half1, float half2)
{
	centerPoint = center;
	halfWidthExtents[0] = half0;
	halfWidthExtents[1] = half1;
	halfWidthExtents[2] = half2;
}

AABB::AABB(const AABB& other)
{
	centerPoint = other.centerPoint;
	halfWidthExtents[0] = other.halfWidthExtents[0];
	halfWidthExtents[1] = other.halfWidthExtents[1];
	halfWidthExtents[2] = other.halfWidthExtents[2];
}

const AABB& AABB::operator=(const AABB& other)
{
	if(&other != this)
	{
		centerPoint = other.centerPoint;
		halfWidthExtents[0] = other.halfWidthExtents[0];
		halfWidthExtents[1] = other.halfWidthExtents[1];
		halfWidthExtents[2] = other.halfWidthExtents[2];
	}
	return *this;
}

AABB::~AABB(void)
{
}

// Check the type of collision
int AABB::calculateCollisionType(Collider* other)
{
	AABB* otherAABB = dynamic_cast<AABB*>(other);
	if(otherAABB)
	{
		return AABB_AABB;
	}
	else
	{
		return AABB_LINE;
	}
}

// Check the collision based on type
bool AABB::checkCollision(Collider* other)
{
	int type = calculateCollisionType(other);

	if(type == AABB_AABB)
	{
		return checkAABB(dynamic_cast<AABB*>(other));
	}
	else if(type == AABB_LINE)
	{
		return(other->checkCollision(this));
	}
	return false;
}

//AABB collison detection
//see: Real Time Collision book 4.2 Axis-Aligned Bounding Boxes (AABBs)
bool AABB::checkAABB(AABB* other)
{
	if(abs(this->centerPoint.x - other->centerPoint.x) > (this->halfWidthExtents[0] + other->halfWidthExtents[0]))
	{
		return false;
	}
	if(abs(this->centerPoint.y - other->centerPoint.y) > (this->halfWidthExtents[1] + other->halfWidthExtents[1]))
	{
		return false;
	}
	if(abs(this->centerPoint.z - other->centerPoint.z) > (this->halfWidthExtents[2] + other->halfWidthExtents[2]))
	{
		return false;
	}
	return true;
}

// Returns the new velocity to use based on the collision
// (If no collision, return given velocity)
glm::vec3 AABB::collisionResponseVector(Collider* other, glm::vec3 velocity)
{
	if(calculateCollisionType(other) == AABB_AABB)
	{
		AABB* otherAABB = dynamic_cast<AABB*>(other);

		// If collision, calculate a new velocity
		if(checkAABB(otherAABB))
		{
			//Play sound when collision happens
			//see: http://stackoverflow.com/questions/1565439/how-to-playsound-in-c-using-windows-api
			//see: http://msdn.microsoft.com/en-us/library/windows/desktop/dd743680(v=vs.85).aspx
			//Note: you have to place sound file where .exe file is
			PlaySound(TEXT("bounce.wav"), NULL, SND_ASYNC);
			// Get the normal (axis)
			glm::vec3 normal = getNormal(otherAABB);
			velocity = glm::reflect(velocity, normal);
		}
	}
	return velocity;
}

// Collision normal of AABB-AABB
// source: http://www.gamedev.net/topic/567310-platform-game-collision-detection/
glm::vec3 AABB::getNormal(AABB* other)
{
	glm::vec3 normals[6] = 
	{
		glm::vec3(1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, -1),
	};

	// Min and max boundaries on this AABB
	float minX = centerPoint.x - halfWidthExtents[0];
	float maxX = centerPoint.x + halfWidthExtents[0];
	float minY = centerPoint.y - halfWidthExtents[1];
	float maxY = centerPoint.y + halfWidthExtents[1];
	float minZ = centerPoint.z - halfWidthExtents[2];
	float maxZ = centerPoint.z + halfWidthExtents[2];

	// Min and max boundaries on other AABB
	float otherMinX = other->centerPoint.x - other->halfWidthExtents[0];
	float otherMaxX = other->centerPoint.x + other->halfWidthExtents[0];
	float otherMinY = other->centerPoint.y - other->halfWidthExtents[1];
	float otherMaxY = other->centerPoint.y + other->halfWidthExtents[1];
	float otherMinZ = other->centerPoint.z - other->halfWidthExtents[2];
	float otherMaxZ = other->centerPoint.z + other->halfWidthExtents[2];

	float distances[6] = 
	{
		(otherMaxX - minX), // distance of other AABB to face on 'left' side of this AABB
		(maxX - otherMinX), // distance of other to 'right' face of this

		(otherMaxY - minY), // distance of other to 'bottom' face of this
		(maxY - otherMinY), // distance of other to 'top' face of this

		(otherMaxZ - minZ), // distance of other to 'far' face of this
		(maxZ - otherMinZ) // distance of other to 'near' face of this
	};

	// Find the smallest
	float temp = distances[0];
	int index = 0;
	for(int i=0; i<6; i++)
	{
		if(temp > distances[i])
		{
			temp = distances[i];
			index = i;
		}
	}

	return normals[index];
}

// This is possibly for OBB?
/*vec3 AABB::getClosestPoint(vec3 otherPoint)
{
	vec3 point = centerPoint;

	// Distance between AABB centers
	vec3 dist = otherPoint - point;

	// World axes
	vec3 axes[3];
	axes[0] = vec3(1, 0, 0);
	axes[1] = vec3(0, 1, 0);
	axes[2] = vec3(0, 0, 1);
	
	// For each world axis
	for(int i=0; i<3; i++)
	{
		// Use dot product for distance
		float distance = dot(dist, axes[i]);

		// If distance is farther than the box extends, clamp to the box
		if(distance > halfWidthExtents[i])
		{
			distance = halfWidthExtents[i];
		}
		if(distance < halfWidthExtents[i] * -1)
		{
			distance = halfWidthExtents[i] * -1;
		}

		// Step the distance along the world coordinate
		point += distance * axes[i];
	}

	return point;
}*/

/*
// THIS WORKS BUT IS NO LONGER NEEDED HERE
// Math Primer for Graphics and Game Development
// A.5 Closest Point on an AABB
glm::vec3 AABB::getClosestPoint(glm::vec3 otherPoint)
{
	glm::vec3 point = otherPoint;

	float minX = centerPoint.x - halfWidthExtents[0];
	float maxX = centerPoint.x + halfWidthExtents[0];
	float minY = centerPoint.y - halfWidthExtents[1];
	float maxY = centerPoint.y + halfWidthExtents[1];
	float minZ = centerPoint.z - halfWidthExtents[2];
	float maxZ = centerPoint.z + halfWidthExtents[2];

	if(point.x < minX)
	{
		point.x = minX;
	} else if(point.x > maxX) {
		point.x = maxX;
	}

	if(point.y < minY)
	{
		point.y = minY;
	} else if(point.y > maxY) {
		point.y = maxY;
	}

	if(point.z < minZ)
	{
		point.z = minZ;
	} else if(point.z > maxZ) {
		point.z = maxZ;
	}

	return point;
}
*/

// This works most of the time but has issues with distances of 0
// See new version of function for better implementation
/* glm::vec3 AABB::getNormal(glm::vec3 dist)
{
	glm::vec3 normal;
	float dotProd[6];

	// World axes
	dotProd[0] = glm::dot(dist, glm::vec3(1, 0, 0));
	dotProd[1] = glm::dot(dist, glm::vec3(0, 1, 0));
	dotProd[2] = glm::dot(dist, glm::vec3(0, 0, 1));

	// Reversed axes
	dotProd[3] = glm::dot(dist, glm::vec3(-1, 0, 0));
	dotProd[4] = glm::dot(dist, glm::vec3(0, -1, 0));
	dotProd[5] = glm::dot(dist, glm::vec3(0, 0, -1));

	// Square instead of using absolute value
	for(int i=0; i<6; i++)
	{
		dotProd[i] *= dotProd[i];
	}

	// Find the smallest
	float temp = dotProd[0];
	int index = 0;
	for(int i=0; i<6; i++)
	{
		if(temp > dotProd[i])
		{
			temp = dotProd[i];
			index = i;
		}
	}

	switch(index)
	{
		case 0: normal = glm::vec3(1, 0, 0); break;
		case 1: normal = glm::vec3(0, 1, 0); break;
		case 2: normal = glm::vec3(0, 0, 1); break;
		case 3: normal = glm::vec3(-1, 0, 0); break;
		case 4: normal = glm::vec3(0, -1, 0); break;
		case 5: normal = glm::vec3(0, 0, -1); break;
		default: normal = glm::vec3(0, 0, 0); break;
	}

	return normal;
}
*/