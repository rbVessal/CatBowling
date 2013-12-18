#ifndef _COLLIDER_
#define _COLLIDER_

#include <cstdlib>
#include "Angel.h"

//see: http://www.g-truc.net/project-0016.html
#include "glm\glm.hpp" //for vectors and matrices

#define AABB_AABB 0
#define AABB_LINE 1
#define LINE_AABB 2
#define LINE_LINE 3

class Collider
{
public:
	Collider(void);
	Collider(const Collider&);
	const Collider& operator=(const Collider&);
	~Collider(void);

	// Pure-virtual Functions
	virtual bool checkCollision(Collider*)=0;
	virtual glm::vec3 collisionResponseVector(Collider*, glm::vec3)=0;
	virtual int calculateCollisionType(Collider* other)=0;

	// TODO: OBB, point-to-line collisions, handle tunneling
	// ...
};
#endif
