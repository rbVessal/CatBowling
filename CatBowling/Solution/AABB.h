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
	bool checkCollision(Collider*);
	glm::vec3 collisionResponseVector(Collider*, glm::vec3);
	int calculateCollisionType(Collider* other);

	// Functions
	bool checkAABB(AABB*);
	glm::vec3 getNormal(AABB*);
	float getSquaredDistancePointToLine(glm::vec3, glm::vec3, glm::vec3);
	
	// Setters / Getters
	glm::vec3 getCenter(){ return centerPoint; }
	void setCenter(glm::vec3 newCenter){ centerPoint = newCenter; }
	float* gethalfWidths(){ return halfWidthExtents; }

private:
	float halfWidthExtents[3];
	glm::vec3 centerPoint;
};
#endif
