#ifndef LINE_COLLIDER
#define LINE_COLLIDER

#include "aabb.h"
class LineSegment :	public Collider
{
public:
	LineSegment(void);
	LineSegment(glm::vec3, glm::vec3);
	~LineSegment(void);

	// Inherited functions
	bool checkCollision(Collider* other);
	int calculateCollisionType(Collider* other);

	// empty inherited functions...
	glm::vec3 collisionResponseVector(Collider*, glm::vec3) { return glm::vec3(0, 0, 0); }

	// other functions
	float checkAABB(AABB*);

private:
	glm::vec3 endPoint1;
	glm::vec3 endPoint2;
};
#endif
