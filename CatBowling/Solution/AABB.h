#ifndef _AABB_
#define _AABB_

#include "collider.h"

class AABB : public Collider
{
public:
	AABB(void);
	AABB(vec3, float, float, float);
	AABB(const AABB&);
	const AABB& operator=(const AABB&);
	~AABB(void);

	// Inherited functions
	bool checkCollision(Collider* other){ return checkAABB( dynamic_cast<AABB*>(other) ); }
	vec3 getClosestPoint(vec3);
	vec3 collisionResponseVector(Collider*, vec3);
	vec3 getNormal(vec3);

	// Functions
	// TODO: fix the collision response; it isn't fully working
	bool checkAABB(AABB*);

	// TODO: make this a private field and add setters and getters
	vec3 centerPoint;

private:
	float halfWidthExtents[3];
};

#endif