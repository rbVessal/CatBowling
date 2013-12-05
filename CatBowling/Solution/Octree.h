#ifndef _OCTREE_
#define _OCTREE_

#include "Collider.h"

// An object represented as a sphere to easily fit the octree grid
// Functions as a linked list
struct OctreeObject
{
	glm::vec3 centerPoint;
	float radius;

	Collider* collider;  // todo: change to polyhedrons
	OctreeObject* next;
};

// A grid-node that is either a leaf or interior
struct OctreeNode
{ 
	glm::vec3 centerPoint;
	float halfWidth;
	OctreeNode* children[8];
	bool hasChildren;

	// List of objects contained at this node
	OctreeObject* objList;
};

// Source: Real-Time Collision Detection
// Ch 7.3 - Octrees
class Octree
{
public:
	Octree(void);
	~Octree(void);

	void deleteNode(OctreeNode*);

	OctreeNode* buildTree(glm::vec3, float, int);
	void insertObject(OctreeNode*, OctreeObject*);
	void testAllCollisions(OctreeNode*);

private:
	OctreeNode* root;
};
#endif
