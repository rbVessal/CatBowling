#ifndef GLOBALS_H
#define GLOBALS_H

#include "GL/glfw3.h"

extern GLFWwindow*	window;
extern GLuint		programID;
extern bool			debugVisible;

// Time Variables
extern double		previousTime;
extern double		currentTime;
extern double		deltaTime;

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))


#endif

