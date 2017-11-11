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

// Constants for coordinate axes
#define COR_X 0
#define COR_Y 1
#define COR_Z 2
#define COR_W 3

// Handle of shader program which defines the material of our object
GLuint g_shaderProgram = 0;


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
	}
}



// ----------------------- Create some Mesh for testing
bool createMesh(Model *model)
{
	cout << "loading mesh" << endl;
	
	std::vector<Vertex> vert;
	vert.reserve(20);
	std::vector<Triangle> tria;
	tria.reserve(10);
	
	// ------------ do cube
	// xy0
	Vertex x0 = {{0,0,0},{1,0,0,1},{0,0,0}};
	Vertex x1 = {{1,1,0},{1,1,0,1},{0,0,0}};
	Vertex x2 = {{0,1,0},{0,1,0,1},{0,0,0}};
	Vertex x3 = {{1,0,0},{0,0,1,1},{0,0,0}};
	
	int index = vert.size();
	vert.push_back(x0);
	vert.push_back(x1);
	vert.push_back(x2);
	vert.push_back(x3);
	
	Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
	Triangle t2 = {index+0, index+3, index+1, {0,0,0}};
	
	tria.push_back(t1);
	tria.push_back(t2);
	
	
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
	
	cout << "mesh to gl" << endl;
	
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
	
	cout << "mesh done" << endl;
	
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
	GLfloat model_mat[16] = {
		0.008000f,  0.000000f,  0.000000f, 0.000000f,
		0.000000f,  0.008000f,  0.000000f, 0.000000f,
		0.000000f,  0.000000f,  0.008000f, 0.000000f,
	   -1.300000f, -1.500000f, -1.000000f, 1.0000000f
   };

   GLint location = glGetUniformLocation(g_shaderProgram, "Model_mat");
   assert(location != -1);      
   glUniformMatrix4fv(location, 1, false, model_mat);

   // Set projection*view Matrix
   GLfloat projview_mat[16] = {
	   2.747478f, 0.000000f,  0.000000f,  0.000000f,
	   0.000000f, 2.457419f,  0.546594f,  0.447214f,
	   0.000000f, 1.228709f, -1.093189f, -0.894427f,
	   0.000000f, 0.000000f,  1.877236f,  3.354102f
   };

   location = glGetUniformLocation(g_shaderProgram, "ProjectView_mat");
   assert(location != -1);   
   glUniformMatrix4fv(location, 1, false, projview_mat);

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
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed
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

	// load my stuff:
	Model model;
	createMesh(&model);
	cout << "Model IBOSize: " << model.IBOSize << endl;
	
	while(!glfwWindowShouldClose(window))
	{
		displayFunc(&model);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
