#ifndef LINE_GEOMETRY
#define LINE_GEOMETRY

// "line" class used for a narrow cube that has a line collider instead of an AABB

#include "boxshape.h"
#include "LineSegment.h"

#define LINE_VISIBILITY 1 // render lines or don't render lines?

class Line : public BoxShape
{
public:
	Line(void);
	Line(glm::vec3, glm::vec3); // Line with endpoints a and b
	Line(const Line& other);
	const Line& operator=(const Line& other);
	~Line(void);
};
#endif
