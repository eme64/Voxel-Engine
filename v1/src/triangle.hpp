#ifndef __TRIANGLE_H_DEFINED__
#define __TRIANGLE_H_DEFINED__

// A triangle has three associated vertices and a normal vector
struct Triangle
{
	// Vertex pIndices of the triangle
	int vtxIndexA;
	int vtxIndexB;
	int vtxIndexC;

	// Normal vector of the triangle
	GLfloat normal[3];
};
#endif
