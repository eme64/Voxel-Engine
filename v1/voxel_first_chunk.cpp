#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <stdio.h>
#include <math.h>

#include <vector>
#include <iostream>
#include <ios>
#include <fstream>
#include <string>

#include "/home/emanuel/Documents/glm/glm/glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "/home/emanuel/Documents/glm/glm/glm/gtx/transform.hpp"

// Default name of the mesh file to load
#define DEFAULT_MESH_NAME MAIN_DIR "volcano_mesh.raw"

#ifndef offsetof
	#define offsetof(s, m) (size_t)&(((s *)0)->m)
#endif

using namespace std;

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

void _check_gl_error(const char *file, int line) {
	GLenum err (glGetError());

	while(err!=GL_NO_ERROR) {
			string error;

			switch(err) {
					case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
					case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
					case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
					case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
					case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
			}

			cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<endl;
			err=glGetError();
	}
}

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

// A vertex has a position, a color and a normal vector
struct Vertex
{
	// Position of the vertex
	GLfloat pos[4];

	// R/G/B/Alpha color of the vertex
	GLfloat color[4];

	// Normal vector of the vertex
	GLfloat normal[3];
};

struct Model
{
	GLuint IBOSize;
	GLuint VBO;
	GLuint IBO;
};

struct Camera
{
	// position
	glm::vec3 position = glm::vec3( 5, 5, 5 );
	// horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = 0.0f;
	// Initial Field of View
	float FoV = 45.0f;

	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.1f;
	
	double lastFrameTime;
};

/*
########################### CHUNK
*/
#define CHUNK_SIZE 16
#define int_chunk_t uint8_t
struct Chunk
{
	int world_offset_x;
	int world_offset_y;
	int world_offset_z;
	
	int_chunk_t block[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
};

bool chunkSetData(Chunk *chunk)
{
	chunk->world_offset_x = 0;
	chunk->world_offset_y = 0;
	chunk->world_offset_z = 0;
	
	for(int x=0;x<CHUNK_SIZE;x++)
		for(int y=0;y<CHUNK_SIZE;y++)
			for(int z=0;z<CHUNK_SIZE;z++)
			{
				if(x==0 || y==0 || z==0 || x==CHUNK_SIZE-1 || y==CHUNK_SIZE-1 || z==CHUNK_SIZE-1)
				{
					chunk->block[x][y][z] = 0;
				}else{
					if(((x+y+z) % 6)<5)
					{
						chunk->block[x][y][z] = 1;
					}else{
						chunk->block[x][y][z] = 0;
					}
				}
			}
}

int g_Screen_x = 800;
int g_Screen_y = 600;

// Constants for coordinate axes
#define COR_X 0
#define COR_Y 1
#define COR_Z 2
#define COR_W 3

// Handle of shader program which defines the material of our object
GLuint g_shaderProgram = 0;

void initCamera(Camera *cam, GLFWwindow* window){
	glfwSetCursorPos(window, g_Screen_x/2, g_Screen_y/2);
	cam->lastFrameTime = glfwGetTime();
}

void updateCamera(Camera *cam, GLFWwindow* window){
	
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - cam->lastFrameTime);
	cam->lastFrameTime = currentTime;
	
	// reed mouse speeds
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, g_Screen_x/2, g_Screen_y/2);
	
	// cam angles
	cam->horizontalAngle += cam->mouseSpeed * deltaTime * float(g_Screen_x/2 - xpos );
	cam->verticalAngle   += cam->mouseSpeed * deltaTime * float(g_Screen_y/2 - ypos );
	
	// calc direction
	glm::vec3 direction(
		cos(cam->verticalAngle) * sin(cam->horizontalAngle),
		sin(cam->verticalAngle),
		cos(cam->verticalAngle) * cos(cam->horizontalAngle));
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(cam->horizontalAngle - 3.14f/2.0f),
		0,
		cos(cam->horizontalAngle - 3.14f/2.0f));
	
	// Up vector : perpendicular to both direction and right
	glm::vec3 up = glm::cross( right, direction );
	
	glm::vec3 globalup = glm::vec3(0,1,0);
	glm::vec3 walkdirection = -glm::cross( right, globalup);
	
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
	    cam->position += walkdirection * deltaTime * cam->speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS){
	    cam->position -= walkdirection * deltaTime * cam->speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
	    cam->position += right * deltaTime * cam->speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
	    cam->position -= right * deltaTime * cam->speed;
	}
	
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS){
	    cam->position += globalup * deltaTime * cam->speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS){
	    cam->position -= globalup * deltaTime * cam->speed;
	}
	
	if (glfwGetKey(window, GLFW_KEY_SPACE ) == GLFW_PRESS){
	    cout << cam->horizontalAngle << ", " << cam->verticalAngle << endl;
	}
	
	//cam->FoV -= 5 * glfwGetMouseWheel(window);
	
	// Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(cam->FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 ViewMatrix = glm::lookAt(
	    cam->position,           // Camera is here
	    cam->position+direction, // and looks here : at the same position, plus "direction"
	    up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 projview_mat = ProjectionMatrix * ViewMatrix;
	GLuint location = glGetUniformLocation(g_shaderProgram, "ProjectView_mat");
	assert(location != -1);
	glUniformMatrix4fv(location, 1, false, &projview_mat[0][0]);
}
//=======================================================================
// Converts hsv to rgb
//=======================================================================
// IN:  hsv color coordinates h in [0,360), s in [0,1], v in [0,1]
// OUT: rgb coordinates of hsv color, rgb in [0,1]^3
//=======================================================================
void hsv2rgb(float h, float s, float v, float *pR, float *pG, float *pB)
{
	assert(h >= 0.0f && h < 360.0f);
	assert(s >= 0.0f && s <= 1.0f);
	assert(v >= 0.0f && v <= 1.0f);

	// Get hue in [0,1)
	h /= 360.0;
	if(h == 1.0f)
		h = 0.0f;
	h *= 6.0f;

	// Get used segment i, i in [0,5]
	int i = (int)floor(h);

	// Get the fractional part of h
	float f = h-i;
	float p = v * (1 - s);
	float q = v * (1 - (s * f));
	float t = v * (1 - (s * (1 - f)));

	// Cases for every segment
	switch(i)
	{
	case 0: *pR=v; *pG=t; *pB=p; break;
	case 1: *pR=q; *pG=v; *pB=p; break;
	case 2: *pR=p; *pG=v; *pB=t; break;
	case 3: *pR=p; *pG=q; *pB=v; break;
	case 4: *pR=t; *pG=p; *pB=v; break;
	case 5: *pR=v; *pG=p; *pB=q; break;
	}
}

//=======================================================================
// Calculates and defines the surface normals for all vertices by
// combining the normals of all adjacent triangles.
//=======================================================================
// IN:  pTriangleArray: List of triangles
//      numTriangles:   number of triangles in pTriangleArray
//      numVertices:    Number of triangles in pVertices
// OUT: pVertices:      The 'normal' member of each vertex is modified
//=======================================================================
void calcVertexNormals(const Triangle *pTriangleArray, int numTriangles, Vertex *pVertices, int numVertices)
{
	// Set all vertex normals to zero
	for(int vtxIndex = 0; vtxIndex < numVertices; vtxIndex++)
	{
		pVertices[vtxIndex].normal[0] = 0.0f;
		pVertices[vtxIndex].normal[1] = 0.0f;
		pVertices[vtxIndex].normal[2] = 0.0f;
	}

	// For each vertex, sum up normals of adjacent triangles
	for(int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		int A = pTriangleArray[triIndex].vtxIndexA;
		int B = pTriangleArray[triIndex].vtxIndexB;
		int C = pTriangleArray[triIndex].vtxIndexC;
		
		pVertices[A].normal[0] += pTriangleArray[triIndex].normal[0];
		pVertices[A].normal[1] += pTriangleArray[triIndex].normal[1];
		pVertices[A].normal[2] += pTriangleArray[triIndex].normal[2];
		
		pVertices[B].normal[0] += pTriangleArray[triIndex].normal[0];
		pVertices[B].normal[1] += pTriangleArray[triIndex].normal[1];
		pVertices[B].normal[2] += pTriangleArray[triIndex].normal[2];
		
		pVertices[C].normal[0] += pTriangleArray[triIndex].normal[0];
		pVertices[C].normal[1] += pTriangleArray[triIndex].normal[1];
		pVertices[C].normal[2] += pTriangleArray[triIndex].normal[2];
	}
	
	// Normalize all vertex normals	
	for(int vtxIndex = 0; vtxIndex < numVertices; vtxIndex++)
	{
		GLfloat norm = pVertices[vtxIndex].normal[0]*pVertices[vtxIndex].normal[0]
				+pVertices[vtxIndex].normal[1]*pVertices[vtxIndex].normal[1]
				+pVertices[vtxIndex].normal[2]*pVertices[vtxIndex].normal[2];
		norm = sqrt(norm);
		pVertices[vtxIndex].normal[0] /= norm;
		pVertices[vtxIndex].normal[1] /= norm;
		pVertices[vtxIndex].normal[2] /= norm;
		cout << vtxIndex << ":" << pVertices[vtxIndex].normal[0] << ","<< pVertices[vtxIndex].normal[1] << ","<< pVertices[vtxIndex].normal[2] << endl;
	}
}


bool createMeshfromChunk(Chunk *chunk, Model *model)
{
	std::vector<Vertex> vert;
	vert.reserve(20);
	std::vector<Triangle> tria;
	tria.reserve(10);
	
	for(int x=1;x<CHUNK_SIZE-1;x++)
		for(int y=1;y<CHUNK_SIZE-1;y++)
			for(int z=1;z<CHUNK_SIZE-1;z++)
			{
				int_chunk_t me = chunk->block[x][y][z];
				
				if(me>0){
					int_chunk_t yp = chunk->block[x][y+1][z];
					int_chunk_t ym = chunk->block[x][y-1][z];
					int_chunk_t xm = chunk->block[x-1][y][z];
					int_chunk_t xp = chunk->block[x+1][y][z];
					int_chunk_t zm = chunk->block[x][y][z-1];
					int_chunk_t zp = chunk->block[x][y][z+1];

					GLfloat offx = x + 	CHUNK_SIZE*chunk->world_offset_x;
					GLfloat offy = y + 	CHUNK_SIZE*chunk->world_offset_y;
					GLfloat offz = z + 	CHUNK_SIZE*chunk->world_offset_z;
					
					if(yp==0){
						// x1z
						Vertex x0 = {{offx+0,offy+1,offz+0,1},{1,0,0,1},{0,0,0}};
						Vertex x1 = {{offx+1,offy+1,offz+1,1},{1,1,0,1},{0,0,0}};
						Vertex x2 = {{offx+0,offy+1,offz+1,1},{0,1,0,1},{0,0,0}};
						Vertex x3 = {{offx+1,offy+1,offz+0,1},{0,0,1,1},{0,0,0}};
						
						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);
	
						Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
						Triangle t2 = {index+0, index+1, index+3, {0,0,0}};
	
						tria.push_back(t1);
						tria.push_back(t2);
					}
					if(ym==0){
						// x0z	
						Vertex x0 = {{offx+0,offy+0,offz+0,1},{1,0,0,1},{0,0,0}};
						Vertex x1 = {{offx+1,offy+0,offz+1,1},{1,1,0,1},{0,0,0}};
						Vertex x2 = {{offx+0,offy+0,offz+1,1},{0,1,0,1},{0,0,0}};
						Vertex x3 = {{offx+1,offy+0,offz+0,1},{0,0,1,1},{0,0,0}};
		
						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);
	
						Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
						Triangle t2 = {index+0, index+3, index+1, {0,0,0}};
	
						tria.push_back(t1);
						tria.push_back(t2);
					}
					
					if(xm==0){
						// 0yz	
						Vertex x0 = {{offx+0,offy+0,offz+0,1},{1,0,0,1},{0,0,0}};
						Vertex x1 = {{offx+0,offy+1,offz+1,1},{1,1,0,1},{0,0,0}};
						Vertex x2 = {{offx+0,offy+0,offz+1,1},{0,1,0,1},{0,0,0}};
						Vertex x3 = {{offx+0,offy+1,offz+0,1},{0,0,1,1},{0,0,0}};
		
						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);
	
						Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
						Triangle t2 = {index+0, index+1, index+3, {0,0,0}};
	
						tria.push_back(t1);
						tria.push_back(t2);
					}
					
					if(xp==0){
						// 0yz	
						Vertex x0 = {{offx+1,offy+0,offz+0,1},{1,0,0,1},{0,0,0}};
						Vertex x1 = {{offx+1,offy+1,offz+1,1},{1,1,0,1},{0,0,0}};
						Vertex x2 = {{offx+1,offy+0,offz+1,1},{0,1,0,1},{0,0,0}};
						Vertex x3 = {{offx+1,offy+1,offz+0,1},{0,0,1,1},{0,0,0}};
		
						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);
	
						Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
						Triangle t2 = {index+0, index+3, index+1, {0,0,0}};
	
						tria.push_back(t1);
						tria.push_back(t2);
					}
					
					if(zm==0){
						// xy0
						Vertex x0 = {{offx+0,offy+0,offz+0,1},{1,0,0,1},{0,0,0}};
						Vertex x1 = {{offx+1,offy+1,offz+0,1},{1,1,0,1},{0,0,0}};
						Vertex x2 = {{offx+0,offy+1,offz+0,1},{0,1,0,1},{0,0,0}};
						Vertex x3 = {{offx+1,offy+0,offz+0,1},{0,0,1,1},{0,0,0}};
	
						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);
	
						Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
						Triangle t2 = {index+0, index+1, index+3, {0,0,0}};
	
						tria.push_back(t1);
						tria.push_back(t2);
					}
					
					if(zp==0){
						// xy1	
						Vertex x0 = {{offx+0,offy+0,offz+1,1},{1,0,0,1},{0,0,0}};
						Vertex x1 = {{offx+1,offy+1,offz+1,1},{1,1,0,1},{0,0,0}};
						Vertex x2 = {{offx+0,offy+1,offz+1,1},{0,1,0,1},{0,0,0}};
						Vertex x3 = {{offx+1,offy+0,offz+1,1},{0,0,1,1},{0,0,0}};
	
						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);
		
						Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
						Triangle t2 = {index+0, index+3, index+1, {0,0,0}};
	
						tria.push_back(t1);
						tria.push_back(t2);
					}

				}
				
			}
	
	
	// ------------- convert vector to array
	int numVertices = vert.size();
	Vertex *pVertexArray = &vert[0];
	
	int numTriangles = tria.size();
	Triangle *pTriangleArray = &tria[0];	
	
	// ------------- calculate normals
	for(int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		// Positions of the triangles nodes
		GLfloat positionA[3];
		GLfloat positionB[3];
		GLfloat positionC[3];

		// Get the positions of the triangles nodes
		int A = pTriangleArray[triIndex].vtxIndexA;
		int B = pTriangleArray[triIndex].vtxIndexB;
		int C = pTriangleArray[triIndex].vtxIndexC;
		
		cout << A << "," << B << "," << C << endl;
		
		GLfloat Ax = pVertexArray[A].pos[COR_X];
		GLfloat Ay = pVertexArray[A].pos[COR_Y];
		GLfloat Az = pVertexArray[A].pos[COR_Z];
		
		GLfloat Bx = pVertexArray[B].pos[COR_X];
		GLfloat By = pVertexArray[B].pos[COR_Y];
		GLfloat Bz = pVertexArray[B].pos[COR_Z];

		GLfloat Cx = pVertexArray[C].pos[COR_X];
		GLfloat Cy = pVertexArray[C].pos[COR_Y];
		GLfloat Cz = pVertexArray[C].pos[COR_Z];
		
		// Vectors for the calculation of the normal
		GLfloat vec1[3];
		GLfloat vec2[3];

		// Calculate two difference vectors of the triangle
		vec1[COR_X] = Ax - Bx;
		vec1[COR_Y] = Ay - By;
		vec1[COR_Z] = Az - Bz;

		vec2[COR_X] = Ax - Cx;
		vec2[COR_Y] = Ay - Cy;
		vec2[COR_Z] = Az - Cz;

		// Calculate the normal vector: cross product between x and y
		pTriangleArray[triIndex].normal[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
		pTriangleArray[triIndex].normal[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
		pTriangleArray[triIndex].normal[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];

		// Normalize the normal vector:
		GLfloat norm = pTriangleArray[triIndex].normal[0]*pTriangleArray[triIndex].normal[0]
				+ pTriangleArray[triIndex].normal[1]*pTriangleArray[triIndex].normal[1]
				+ pTriangleArray[triIndex].normal[2]*pTriangleArray[triIndex].normal[2];
		norm = sqrt(norm);
		
		pTriangleArray[triIndex].normal[0] /= norm;
		pTriangleArray[triIndex].normal[1] /= norm;
		pTriangleArray[triIndex].normal[2] /= norm;
	}

	// Calculate vertex normals
	calcVertexNormals(pTriangleArray, numTriangles, pVertexArray, numVertices);
	
	model->IBOSize = numTriangles * 3;
	GLshort *pIndices = new GLshort[model->IBOSize];
	for(int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		pIndices[3*triIndex + 0] = pTriangleArray[triIndex].vtxIndexA;
		pIndices[3*triIndex + 1] = pTriangleArray[triIndex].vtxIndexB;
		pIndices[3*triIndex + 2] = pTriangleArray[triIndex].vtxIndexC;
	}
	
	
	// ------------- send to GL
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	// Create vertex buffer object and download vertex array
	model->VBO = 0;
	glGenBuffers(1, &(model->VBO));
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, pVertexArray, GL_STATIC_DRAW);

	GLint location;
	// bind Vertex shader per-vertex attributes
	// load positions	
	location = glGetAttribLocation(g_shaderProgram, "position");
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(location);
	
	// load color
	location = glGetAttribLocation(g_shaderProgram, "color_in");
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(location);

	// load normal	
	location = glGetAttribLocation(g_shaderProgram, "normal");
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(location);	
	
	// Create index buffer and download index data
	model->IBO = 0;
	glGenBuffers(1, &(model->IBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*model->IBOSize, pIndices, GL_STATIC_DRAW);
	
	delete[] pIndices;
	// not free because from vector !
	//delete[] pVertexArray;
	//delete[] pTriangleArray;
	
	check_gl_error();

	return true;
}


// ----------------------- Create some Mesh for testing
bool createMesh(Model *model)
{	
	std::vector<Vertex> vert;
	vert.reserve(20);
	std::vector<Triangle> tria;
	tria.reserve(10);
	
	// ------------ do cube
	{
		// xy0
		Vertex x0 = {{0,0,0,1},{1,0,0,1},{0,0,0}};
		Vertex x1 = {{1,1,0,1},{1,1,0,1},{0,0,0}};
		Vertex x2 = {{0,1,0,1},{0,1,0,1},{0,0,0}};
		Vertex x3 = {{1,0,0,1},{0,0,1,1},{0,0,0}};
	
		int index = vert.size();
		vert.push_back(x0);
		vert.push_back(x1);
		vert.push_back(x2);
		vert.push_back(x3);
	
		Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
		Triangle t2 = {index+0, index+1, index+3, {0,0,0}};
	
		tria.push_back(t1);
		tria.push_back(t2);
	}
	{	
		// xy1	
		Vertex x0 = {{0,0,1,1},{1,0,0,1},{0,0,0}};
		Vertex x1 = {{1,1,1,1},{1,1,0,1},{0,0,0}};
		Vertex x2 = {{0,1,1,1},{0,1,0,1},{0,0,0}};
		Vertex x3 = {{1,0,1,1},{0,0,1,1},{0,0,0}};
	
		int index = vert.size();
		vert.push_back(x0);
		vert.push_back(x1);
		vert.push_back(x2);
		vert.push_back(x3);
		
		Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
		Triangle t2 = {index+0, index+3, index+1, {0,0,0}};
	
		tria.push_back(t1);
		tria.push_back(t2);
	}
	

	{	
		// 0yz	
		Vertex x0 = {{0,0,0,1},{1,0,0,1},{0,0,0}};
		Vertex x1 = {{0,1,1,1},{1,1,0,1},{0,0,0}};
		Vertex x2 = {{0,0,1,1},{0,1,0,1},{0,0,0}};
		Vertex x3 = {{0,1,0,1},{0,0,1,1},{0,0,0}};
		
		int index = vert.size();
		vert.push_back(x0);
		vert.push_back(x1);
		vert.push_back(x2);
		vert.push_back(x3);
	
		Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
		Triangle t2 = {index+0, index+1, index+3, {0,0,0}};
	
		tria.push_back(t1);
		tria.push_back(t2);
	}
	{	
		// 0yz	
		Vertex x0 = {{1,0,0,1},{1,0,0,1},{0,0,0}};
		Vertex x1 = {{1,1,1,1},{1,1,0,1},{0,0,0}};
		Vertex x2 = {{1,0,1,1},{0,1,0,1},{0,0,0}};
		Vertex x3 = {{1,1,0,1},{0,0,1,1},{0,0,0}};
		
		int index = vert.size();
		vert.push_back(x0);
		vert.push_back(x1);
		vert.push_back(x2);
		vert.push_back(x3);
	
		Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
		Triangle t2 = {index+0, index+3, index+1, {0,0,0}};
	
		tria.push_back(t1);
		tria.push_back(t2);
	}

	{	
		// x0z	
		Vertex x0 = {{0,0,0,1},{1,0,0,1},{0,0,0}};
		Vertex x1 = {{1,0,1,1},{1,1,0,1},{0,0,0}};
		Vertex x2 = {{0,0,1,1},{0,1,0,1},{0,0,0}};
		Vertex x3 = {{1,0,0,1},{0,0,1,1},{0,0,0}};
		
		int index = vert.size();
		vert.push_back(x0);
		vert.push_back(x1);
		vert.push_back(x2);
		vert.push_back(x3);
	
		Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
		Triangle t2 = {index+0, index+3, index+1, {0,0,0}};
	
		tria.push_back(t1);
		tria.push_back(t2);
	}
	{	
		// x1z	
		Vertex x0 = {{0,1,0,1},{1,0,0,1},{0,0,0}};
		Vertex x1 = {{1,1,1,1},{1,1,0,1},{0,0,0}};
		Vertex x2 = {{0,1,1,1},{0,1,0,1},{0,0,0}};
		Vertex x3 = {{1,1,0,1},{0,0,1,1},{0,0,0}};
		
		int index = vert.size();
		vert.push_back(x0);
		vert.push_back(x1);
		vert.push_back(x2);
		vert.push_back(x3);
	
		Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
		Triangle t2 = {index+0, index+1, index+3, {0,0,0}};
	
		tria.push_back(t1);
		tria.push_back(t2);
	}
	// ------------- convert vector to array
	int numVertices = vert.size();
	Vertex *pVertexArray = &vert[0];
	
	int numTriangles = tria.size();
	Triangle *pTriangleArray = &tria[0];	
	
	// ------------- calculate normals
	for(int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		// Positions of the triangles nodes
		GLfloat positionA[3];
		GLfloat positionB[3];
		GLfloat positionC[3];

		// Get the positions of the triangles nodes
		int A = pTriangleArray[triIndex].vtxIndexA;
		int B = pTriangleArray[triIndex].vtxIndexB;
		int C = pTriangleArray[triIndex].vtxIndexC;
		
		cout << A << "," << B << "," << C << endl;
		
		GLfloat Ax = pVertexArray[A].pos[COR_X];
		GLfloat Ay = pVertexArray[A].pos[COR_Y];
		GLfloat Az = pVertexArray[A].pos[COR_Z];
		
		GLfloat Bx = pVertexArray[B].pos[COR_X];
		GLfloat By = pVertexArray[B].pos[COR_Y];
		GLfloat Bz = pVertexArray[B].pos[COR_Z];

		GLfloat Cx = pVertexArray[C].pos[COR_X];
		GLfloat Cy = pVertexArray[C].pos[COR_Y];
		GLfloat Cz = pVertexArray[C].pos[COR_Z];
		
		// Vectors for the calculation of the normal
		GLfloat vec1[3];
		GLfloat vec2[3];

		// Calculate two difference vectors of the triangle
		vec1[COR_X] = Ax - Bx;
		vec1[COR_Y] = Ay - By;
		vec1[COR_Z] = Az - Bz;

		vec2[COR_X] = Ax - Cx;
		vec2[COR_Y] = Ay - Cy;
		vec2[COR_Z] = Az - Cz;

		// Calculate the normal vector: cross product between x and y
		pTriangleArray[triIndex].normal[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
		pTriangleArray[triIndex].normal[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
		pTriangleArray[triIndex].normal[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];

		// Normalize the normal vector:
		GLfloat norm = pTriangleArray[triIndex].normal[0]*pTriangleArray[triIndex].normal[0]
				+ pTriangleArray[triIndex].normal[1]*pTriangleArray[triIndex].normal[1]
				+ pTriangleArray[triIndex].normal[2]*pTriangleArray[triIndex].normal[2];
		norm = sqrt(norm);
		
		pTriangleArray[triIndex].normal[0] /= norm;
		pTriangleArray[triIndex].normal[1] /= norm;
		pTriangleArray[triIndex].normal[2] /= norm;
	}

	// Calculate vertex normals
	calcVertexNormals(pTriangleArray, numTriangles, pVertexArray, numVertices);
	
	model->IBOSize = numTriangles * 3;
	GLshort *pIndices = new GLshort[model->IBOSize];
	for(int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		pIndices[3*triIndex + 0] = pTriangleArray[triIndex].vtxIndexA;
		pIndices[3*triIndex + 1] = pTriangleArray[triIndex].vtxIndexB;
		pIndices[3*triIndex + 2] = pTriangleArray[triIndex].vtxIndexC;
	}
	
	
	// ------------- send to GL
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	// Create vertex buffer object and download vertex array
	model->VBO = 0;
	glGenBuffers(1, &(model->VBO));
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, pVertexArray, GL_STATIC_DRAW);

	GLint location;
	// bind Vertex shader per-vertex attributes
	// load positions	
	location = glGetAttribLocation(g_shaderProgram, "position");
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(location);
	
	// load color
	location = glGetAttribLocation(g_shaderProgram, "color_in");
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(location);

	// load normal	
	location = glGetAttribLocation(g_shaderProgram, "normal");
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(location);	
	
	// Create index buffer and download index data
	model->IBO = 0;
	glGenBuffers(1, &(model->IBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*model->IBOSize, pIndices, GL_STATIC_DRAW);
	
	delete[] pIndices;
	// not free because from vector !
	//delete[] pVertexArray;
	//delete[] pTriangleArray;
	
	check_gl_error();

	return true;
}

std::string readFile(const char *filePath) {
    std::ifstream myFile(filePath);
	std::string content((std::istreambuf_iterator<char>(myFile)), std::istreambuf_iterator<char>());
	return content;
}

//=======================================================================
// Creates two shader objects, fills them with the shader source,
// compiles them, creates a new Program object, attaches both shaders
// to this object, links the program object, returns its identifier.
//=======================================================================
// IN:  path to a GLSL vertex shader file, path to a GLSL fragment shader file
// OUT: GLuint identifier of the (Shader)Program object
//=======================================================================
GLuint createShaderObject(const std::string &vertex_file, const std::string &fragment_file)
{
	GLint status = GL_FALSE;
	int logLength;

	/* source: http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/ */	
	// read file and get source source code char array
    std::string vertShaderStr = readFile(vertex_file.c_str());
	std::string fragShaderStr = readFile(fragment_file.c_str());

    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

	// Create and compile the vertex shader
	std::cout << "Compiling vertex shader." << std::endl;	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertexShader);

	// Check vertex shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);		
		std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(vertexShader, logLength, NULL, &vertShaderError[0]);
		std::cout << &vertShaderError[0] << std::endl;
		return -1;
	}

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::cout << "Compiling fragment shader." << std::endl;	
	glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragmentShader);

	// check fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);		
		std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(fragmentShader, logLength, NULL, &fragShaderError[0]);
		std::cout << &fragShaderError[0] << std::endl;
		return -1;
	}
	
	// Link the vertex and fragment shader into a shader program
	GLuint program = glCreateProgram();

	// attach both shaders
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link Program
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE){
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> programError((logLength > 1) ? logLength : 1 );
		glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
		std::cout << &programError[0] << std::endl;
		return -1;
	}

	// Flag the shader objects for deletion so they get deleted
	// automatically when the program object is deleted.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
	
	return program;
}

bool initGraphics(){
	g_shaderProgram = createShaderObject(MAIN_DIR "shaders/color.glslv", MAIN_DIR "shaders/color.glslf");
	if(g_shaderProgram == 0) {
		cout << "error when creating the shader Objects!" << endl;
		return false;
	}

	// To actually start using the shaders in the program
	glUseProgram(g_shaderProgram);
	
	// Set model matrix
	glm::mat4 model_mat = glm::mat4(1.0f);
	
	/*
	GLfloat model_mat[16] = {
		1.0f,  0.0f,  0.0f, 0.0f,
		0.0f,  1.0f,  0.0f, 0.0f,
		0.0f,  0.0f,  1.0f, 0.0f,
	   	0.0f,  0.0f,  0.0f, 1.0f
  	};
	*/
	
   GLint location = glGetUniformLocation(g_shaderProgram, "Model_mat");
   assert(location != -1);
   glUniformMatrix4fv(location, 1, false, &model_mat[0][0]);

   // Set projection*view Matrix
	/*
   GLfloat projview_mat[16] = {
	   2.747478f, 0.000000f,  0.000000f,  0.000000f,
	   0.000000f, 2.457419f,  0.546594f,  0.447214f,
	   0.000000f, 1.228709f, -1.093189f, -0.894427f,
	   0.000000f, 0.000000f,  1.877236f,  3.354102f
   };
	*/
	glm::mat4 projview_mat = glm::lookAt(
					glm::vec3(-3,-3,-3),
					glm::vec3(0,0,0),
					glm::vec3(0,1,0));

   location = glGetUniformLocation(g_shaderProgram, "ProjectView_mat");
   assert(location != -1);   
   glUniformMatrix4fv(location, 1, false, &projview_mat[0][0]);

   // Set position of light
   GLfloat lightpos[4] = {500.0f, 200.0f, 300.0f, 1.0f};
   location = glGetUniformLocation(g_shaderProgram, "lightPos");
   glUniform4fv(location, 1, lightpos);

   // Preset some Attributes in case no array is set
   location = glGetAttribLocation(g_shaderProgram, "normal");
   assert(location != -1);
   glVertexAttrib4f(location, 0.0f, 1.0f, 0.0f, 1.0f);

   location = glGetAttribLocation(g_shaderProgram, "color_in");
   assert(location != -1);
   glVertexAttrib4f(location, 1.0f, 1.0f, 1.0f, 1.0f);

   return true;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void error_callback(int, const char* err_str)
{
	std::cout << "GLFW Error: " << err_str << std::endl;
}


void displayFunc(const Model* model){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw all triangles defined in the index buffer object
	// with the vertex data from vertex array in the vertex buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);		
	glDrawElements(GL_TRIANGLES, model->IBOSize, GL_UNSIGNED_SHORT, 0);
}

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	GLFWwindow* window = glfwCreateWindow(g_Screen_x, g_Screen_y, "OpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// set callback function for key-inputs
	glfwSetKeyCallback(window, key_callback);
	glfwSetErrorCallback(error_callback);
	
	// init Glew
	glewExperimental = GL_TRUE;
	glewInit();
	
	if(!initGraphics()) {
		cout << "initializing the graphic libs failed!" << endl;		
		return 1;		
	}
	
	// --- init camera:
	Camera cam;
	initCamera(&cam, window); // after window opened!
	
	// load my stuff:
	Model model;
	//createMesh(&model);
	Chunk chunk;
	chunkSetData(&chunk);
	createMeshfromChunk(&chunk, &model);
	cout << "Model IBOSize: " << model.IBOSize << endl;
	
	// ------ background-buffer -> triangles rightly ordered/drawn.
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	
	// ------ make triangles onesided
	glEnable(GL_CULL_FACE);
	
	// ------ background color
	glClearColor(0.2f, 0.1f, 0.1f, 1.0f);	
	
	while(!glfwWindowShouldClose(window))
	{
		updateCamera(&cam, window);
		displayFunc(&model);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
