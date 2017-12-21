#ifndef __VERTEX_H_DEFINED__
#define __VERTEX_H_DEFINED__
// A vertex has a position, a color and a normal vector
struct Vertex
{
	// Position of the vertex
	GLfloat pos[4];

	// R/G/B/Alpha color of the vertex
	GLfloat color[4];

	// used to make corners darker
	GLfloat cornerness;

	// Normal vector of the vertex
	GLfloat normal[3];

	// uv of texture
	GLfloat uv[2];
};
#endif
