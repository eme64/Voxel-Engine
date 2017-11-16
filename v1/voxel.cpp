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

// my includes
#include "src/chunk.hpp"
#include "src/vertex.hpp"
#include "src/camera.hpp"
#include "src/model.hpp"
#include "src/triangle.hpp"
#include "src/createShaderObject.hpp"
#include "src/voxelmap.hpp"

// ------------------------------ GLOBAL VARIABLES
int g_Screen_x = 800;
int g_Screen_y = 600;

// Handle of shader program which defines the material of our object
GLuint g_shaderProgram = 0;

bool initGraphics(){
	g_shaderProgram = createShaderObject(MAIN_DIR "shaders/color.glslv", MAIN_DIR "shaders/color.glslf");
	if(g_shaderProgram == 0) {
		std::cout << "error when creating the shader Objects!" << std::endl;
		return false;
	}

	// To actually start using the shaders in the program
	glUseProgram(g_shaderProgram);
	
	// Set model matrix
	glm::mat4 model_mat = glm::mat4(1.0f);
	GLint location = glGetUniformLocation(g_shaderProgram, "Model_mat");
	assert(location != -1);
	glUniformMatrix4fv(location, 1, false, &model_mat[0][0]);

	// Set projection*view Matrix
	glm::mat4 projview_mat = glm::lookAt(
		glm::vec3(-3,-3,-3), glm::vec3(0,0,0), glm::vec3(0,1,0));
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
		std::cout << "initializing the graphic libs failed!" << std::endl;		
		return 1;		
	}
	
	// --- init camera:
	Camera cam;
	initCamera(cam, window); // after window opened!
	


	// genererate map
	VoxelMap vmap;
	
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			// generate some chunk
			Chunk* chunk = new Chunk;
			chunk->SetData(ChunkPosition{i,j,0});
			chunk->createModel();
			vmap.addChunk(chunk);// add to map
		}
	}
	
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
		updateCamera(cam, window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		vmap.render(); // render all models
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}  
