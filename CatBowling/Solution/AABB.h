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
	bool checkCollision(Collider* other){ return checkAABB( dynamic_cast<AABB*>(other) ); }
	glm::vec3 collisionResponseVector(Collider*, glm::vec3);

	// Functions
	bool checkAABB(AABB*);
	glm::vec3 getNormal(AABB*);
	//glm::vec3 getClosestPoint(glm::vec3);
	
	// Setters / Getters
	glm::vec3 getCenter(){ return centerPoint; }
	void setCenter(glm::vec3 newCenter){ centerPoint = newCenter; }

private:
	float halfWidthExtents[3];
	glm::vec3 centerPoint;
};
#endif
