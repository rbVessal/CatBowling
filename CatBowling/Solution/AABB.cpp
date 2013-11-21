#include "AABB.h"


AABB::AABB(void)
{
}

AABB::AABB(vec3 center, float half0, float half1, float half2)
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
vec3 AABB::collisionResponseVector(Collider* other, vec3 velocity)
{
	AABB* otherAABB = dynamic_cast<AABB*>(other);

	if(otherAABB)
	{
		// If collision, calculate a new velocity
		if(checkAABB(otherAABB))
		{
			// Get the normal (axis)
			vec3 closestPoint = getClosestPoint(otherAABB->centerPoint);
			vec3 normal = getNormal(centerPoint - closestPoint);
			
			//std::cout << closestPoint << std::endl;
			
			velocity = glm::reflect(velocity, normal);
		}
	}
	return velocity;
}

vec3 AABB::getClosestPoint(vec3 otherPoint)
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
}

vec3 AABB::getNormal(vec3 dist)
{
	vec3 normal;
	float dotProd[6];

	// World axes
	dotProd[0] = dot(dist, vec3(1, 0, 0));
	dotProd[1] = dot(dist, vec3(0, 1, 0));
	dotProd[2] = dot(dist, vec3(0, 0, 1));

	// Reversed axes
	dotProd[3] = dot(dist, vec3(-1, 0, 0));
	dotProd[4] = dot(dist, vec3(0, -1, 0));
	dotProd[5] = dot(dist, vec3(0, 0, -1));

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
		case 0: normal = vec3(1, 0, 0); break;
		case 1: normal = vec3(0, 1, 0); break;
		case 2: normal = vec3(0, 0, 1); break;
		case 3: normal = vec3(-1, 0, 0); break;
		case 4: normal = vec3(0, -1, 0); break;
		case 5: normal = vec3(0, 0, -1); break;
		default: normal = vec3(0, 0, 0); break;
	}

	return normal;
}