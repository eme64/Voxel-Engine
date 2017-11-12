#ifndef __CHUNK_H_DEFINED__
#define __CHUNK_H_DEFINED__

// dependency classes:
#include <string>

#include "model.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
extern GLuint g_shaderProgram;

// done in this file:
struct ChunkPosition;
struct cmpByChunkPosition;
struct Chunk;
void calcVertexNormals(const Triangle *pTriangleArray, int numTriangles, Vertex *pVertices, int numVertices);
void _check_gl_error(const char *file, int line);
void displayFunc(const Model& model);

// ok now we start
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

#ifndef offsetof
	#define offsetof(s, m) (size_t)&(((s *)0)->m)
#endif

#define COR_X 0
#define COR_Y 1
#define COR_Z 2
#define COR_W 3

#define CHUNK_SIZE 16
#define int_chunk_t uint8_t
struct ChunkPosition
{
	int x;
	int y;
	int z;
};

struct cmpByChunkPosition {
    bool operator()(const ChunkPosition& a, const ChunkPosition& b) const {
        //return a.length() < b.length();
	if(a.x < b.x){
		return true;
	}else if(a.x > b.x){
		return false;
	}else if(a.y < b.y){
		return true;
	}else if(a.y > b.y){
		return false;
	}else{
		return (a.z < b.z);
	}
    }
};

struct Chunk
{
	ChunkPosition pos;
	
	int_chunk_t block[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	
	bool SetData(ChunkPosition newpos);
	
	Model* model = NULL;
	bool createModel();
	void render();
};

void Chunk::render()
{
	displayFunc(*model);
}

void displayFunc(const Model& model){
	// Draw all triangles defined in the index buffer object
	// with the vertex data from vertex array in the vertex buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.IBO);		
	glDrawElements(GL_TRIANGLES, model.IBOSize, GL_UNSIGNED_SHORT, 0);
}

bool Chunk::SetData(ChunkPosition newpos)
{
	pos = newpos;
	
	for(int x=0;x<CHUNK_SIZE;x++)
		for(int y=0;y<CHUNK_SIZE;y++)
			for(int z=0;z<CHUNK_SIZE;z++)
			{
				if(x==0 || y==0 || z==0 || x==CHUNK_SIZE-1 || y==CHUNK_SIZE-1 || z==CHUNK_SIZE-1)
				{
					block[x][y][z] = 0;
				}else{
					if(((x+y+z) % 6)<3)
					{
						block[x][y][z] = 1;
					}else{
						block[x][y][z] = 0;
					}
				}
			}
}

bool Chunk::createModel()
{
	if(model != NULL){
		std::cout << "you want to recreate the model -> fail!" << std::endl;
		return false;
	}
	
	model = new Model; // only allocate if needed
	
	std::cout << "create new model for (" << pos.x << ", "<< pos.y <<", "<< pos.z << ")"<< std::endl;
	
	std::vector<Vertex> vert;
	vert.reserve(20);
	std::vector<Triangle> tria;
	tria.reserve(10);
	
	for(int x=1;x<CHUNK_SIZE-1;x++)
		for(int y=1;y<CHUNK_SIZE-1;y++)
			for(int z=1;z<CHUNK_SIZE-1;z++)
			{
				int_chunk_t me = block[x][y][z];
				
				if(me>0){
					int_chunk_t yp = block[x][y+1][z];
					int_chunk_t ym = block[x][y-1][z];
					int_chunk_t xm = block[x-1][y][z];
					int_chunk_t xp = block[x+1][y][z];
					int_chunk_t zm = block[x][y][z-1];
					int_chunk_t zp = block[x][y][z+1];

					GLfloat offx = x + 	CHUNK_SIZE*pos.x;
					GLfloat offy = y + 	CHUNK_SIZE*pos.y;
					GLfloat offz = z + 	CHUNK_SIZE*pos.z;
					
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
	}
}

void _check_gl_error(const char *file, int line) {
	GLenum err (glGetError());

	while(err!=GL_NO_ERROR) {
			std::string error;

			switch(err) {
					case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
					case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
					case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
					case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
					case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
			}

			std::cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<std::endl;
			err=glGetError();
	}
}


#endif
