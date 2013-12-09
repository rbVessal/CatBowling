#include "Octree.h"

Octree::Octree(void)
{
	root = NULL;
}

Octree::~Octree(void)
{
	// TODO: deallocate all nodes...
	deleteNode(root);

	std::cout << "done";
}

void Octree::deleteNode(OctreeNode* node)
{
	// Deletes children before deleting self
	if(node != NULL)
	{
		if(node->hasChildren)
		{
			for(int i=0; i<8; i++)
			{
				// Recursively delete each child
				deleteNode(node->children[i]);
			}
		}

		// Delete any objects in the node, then delete the node
		deleteObjs(node->objList);
		delete node;
	}
}

void Octree::deleteObjs(OctreeObject* obj)
{
	// Deletes linked list in reverse order
	if(obj != NULL)
	{
		if(obj->next != NULL)
		{
			deleteObjs(obj->next);
		}
	
		delete obj;
	}
}


// Source: Real-Time Collision Detection
// Ch 7.3 - Octrees
OctreeNode* Octree::buildTree(glm::vec3 centerPoint, float halfWidth, int stopDepth)
{
	// This ends the recursion, as stopDepth will eventually fall below zero
	if(stopDepth < 0)
	{
		return NULL;
	}
	else
	{
		// Construct and fill in root of this subtree
		OctreeNode* node = new OctreeNode();
		node->centerPoint = centerPoint;
		node->halfWidth = halfWidth;
		node->objList = NULL;

		// If we don't have one, this node is the root
		if(root==NULL)
			root = node;

		// Recursively construct the eight children of the subtree
		glm::vec3 offset;
		float step = halfWidth * 0.5f;
		
		for(int i=0; i<8; i++)
		{
			offset.x = ((i & 1) ? step : -step);
			offset.y = ((i & 2) ? step : -step);
			offset.z = ((i & 4) ? step : -step);

			// Recursively insert each child
			node->children[i] = buildTree(centerPoint + offset, step, stopDepth - 1);
		}

		// (isn't this needed?)
		return node;
	}

	return NULL;
}

// Create an OctreeObject for a polyhedron and insert it into tree
void Octree::insert(Polyhedron* poly, glm::vec3 center, float radius)
{
	// Create the OctreeObject
	OctreeObject* obj = new OctreeObject();
	obj->polyhedron = poly;
	obj->centerPoint = center;
	obj->radius = radius;

	// Pass in the root as the first node for standard tree traversal
	insertObject(root, obj);
}

// Source: Real-Time Collision Detection
// Ch 7.3 - Octrees
void Octree::insertObject(OctreeNode* node, OctreeObject* obj)
{
	int index = 0;
	int straddle = 0;

	// What octant (0-7) is the collider's center in?
	// If straddling a dividing x, y, or z plane, exit
	for(int i=0; i<3; i++)
	{
		float delta = obj->centerPoint[i] - node->centerPoint[i];

		if(abs(delta) < node->halfWidth + obj->radius)
		{
			straddle = 1;
			break;
		}

		if(delta > 0) index |= (1 << 1); // ZYX
	}

	if(!straddle && node->children[index])
	{
		// Insert in subtree (object is fully contained in child node)
		insertObject(node->children[index], obj);
	}
	else
	{
		// Straddling a boundary or no child node to descend into
		// Link object into this node's list
		obj->next = node->objList;
		node->objList = obj;
	}
}

void Octree::checkCollisions()
{
	testAllCollisions(root);
}

// Source: Real-Time Collision Detection
// Ch 7.3 - Octrees
void Octree::testAllCollisions(OctreeNode* node)
{
	// Keep track of all ancestor object lists in a stack/list
	// (static is for using same data through each recursive call)
	const int MAX_DEPTH = 40;
	static OctreeNode* ancestorStack[MAX_DEPTH];
	static int depth = 0;

	// Check Collision between all objects on this level and all ancestor objects.
	// (Current level is its own ancestor)
	ancestorStack[depth++] = node;

	for(int i=0; i<depth; i++)
	{
		OctreeObject *objA, *objB;

		for(objA = ancestorStack[i]->objList; objA; objA = objA->next)
		{
			for(objB = node->objList; objB; objB = objB->next)
			{
				// Avoid testing both A->B ad B->A
				if(objA == objB)
					break;

				// Perform the collision test between objA and objB in same manner
				objA->polyhedron->testCollision(objB->polyhedron);
			}
		}
	}

	// Recursively test all existing children
	for(int i = 0; i < 8; i++)
	{
		if(node->children[i])
		{
			testAllCollisions(node->children[i]);
		}
	}

	// Remove current node from ancestor stack before returning
	depth--;
}