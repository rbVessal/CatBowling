#ifndef _AABB_
#define _AABB_

#include "collider.h"

class AABB : public Collider
{
public:
	AABB(void);
	AABB(glm::vec3, float, float, float);
	AABB(const AABB&);
	const AABB& operator=(const AABB&);
	~AABB(void);

	// Inherited functions
	bool checkCollision(Collider* other)
	{
		AABB* otherAABB = dynamic_cast<AABB*>(other);
		if(otherAABB)
		{
			return checkAABB(otherAABB);
		}
		else
		{
			return(other->checkCollision(this));
		}
		return false;
	}

	glm::vec3 collisionResponseVector(Collider*, glm::vec3);
	glm::vec3 getClosestPointToLine(glm::vec3, glm::vec3);
	float getDistanceToLine(glm::vec3, glm::vec3);

	// Functions
	bool checkAABB(AABB*);
	glm::vec3 getNormal(AABB*);
	//glm::vec3 getClosestPoint(glm::vec3);
	float getSquaredDistancePointToLine(glm::vec3, glm::vec3, glm::vec3);
	
	// Setters / Getters
	glm::vec3 getCenter(){ return centerPoint; }
	void setCenter(glm::vec3 newCenter){ centerPoint = newCenter; }

private:
	float halfWidthExtents[3];
	glm::vec3 centerPoint;
};
#endif
