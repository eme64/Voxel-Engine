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


#include "/home/emanuel/Documents/glm/glm/glm.hpp"
//#include "/home/emanuel/Document/glm/glm/glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "/home/emanuel/Documents/glm/glm/gtx/transform.hpp"
//#include "/home/emanuel/Documents/glm/glm/glm/gtx/transform.hpp"


// my includes
#include "src/chunk.hpp"
#include "src/vertex.hpp"
#include "src/camera.hpp"
#include "src/model.hpp"
#include "src/triangle.hpp"
#include "src/createShaderObject.hpp"
#include "src/voxelmap.hpp"
#include "src/block.hpp"
#include "src/input.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "src/stb_image.c"

// ------------------------------ GLOBAL VARIABLES
int g_Screen_x = 1024;
int g_Screen_y = 800;

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
	GLfloat lightpos[4] = {-700.0f, -300.0f, 1000.0f, 1.0f};
	location = glGetUniformLocation(g_shaderProgram, "lightPos");
	glUniform4fv(location, 1, lightpos);

	// Preset some Attributes in case no array is set
	location = glGetAttribLocation(g_shaderProgram, "normal");
	assert(location != -1);
	glVertexAttrib4f(location, 0.0f, 1.0f, 0.0f, 1.0f);
	
	location = glGetAttribLocation(g_shaderProgram, "color_in");
	assert(location != -1);
	glVertexAttrib4f(location, 1.0f, 1.0f, 1.0f, 1.0f);
	
	
	// --------------------------  load Textures:
	int im_x,im_y,im_n;
	unsigned char *data = stbi_load("../../gfx/block.png", &im_x, &im_y, &im_n, 0);
	
	if(data == NULL){
		std::cout << "texture not loaded!" << std::endl;
	}
	
	std::cout << "img_load: " <<  im_x << ", " << im_y << ": " << im_n << std::endl;
	
	
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture :
	//all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, im_y, im_x, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	stbi_image_free(data);
	
	// init block types -> texture mappings
	init_block_types(6);
	
	//set_block_type_harmonious(int id, bool solid, int tex_x, int tex_y, int tex_n, lrc[3]);
	set_block_type_harmonious(0, false, 0, 0, 4, 1.0f,1.0f,1.0f);
	
	set_block_type_harmonious(1, true, 1, 0, 4, 0.0f,1.0f,0.0f);
	set_block_type_harmonious(2, true, 2, 0, 4, 0.7f,0.5f,0.2f);
	set_block_type_harmonious(3, true, 3, 0, 4, 0.7f,0.2f,0.2f);
	set_block_type_harmonious(4, true, 0, 1, 4, 1.0f,1.0f,1.0f);
	set_block_type_harmonious(5, true, 1, 1, 4, 1.0f,1.0f,1.0f);
	
	return true;
}


int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	GLFWwindow* window = glfwCreateWindow(g_Screen_x, g_Screen_y, "Voxel-Engine", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// set callback function for key-inputs
	EInput::init(window);
	
	// init Glew
	glewExperimental = GL_TRUE;
	glewInit();
	
	if(!initGraphics()) {
		std::cout << "initializing the graphic libs failed!" << std::endl;		
		return 1;		
	}
	
	// --- init camera:
	Camera cam;
	cam.init(window); // after window opened!
	
	// genererate map
	VoxelMap *vmap = new VoxelMap(10,10,5);
	
	
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
		
		int kill_pos[3] = {0,0,0};
		int set_pos[3] = {0,0,0};
		bool looking_active = false; // passed in
		
		cam.update_inputs(looking_active, kill_pos, set_pos);
		
		// call collision here !
		vmap->calculateCollision(cam.position, cam.velocity, cam.radius);
		
		cam.update_finalize();
		
		if(EInput::MouseHit(0) && looking_active){
			vmap->blockKill(kill_pos);
		}
		
		if(EInput::MouseHit(1) && looking_active){
			vmap->blockSet(set_pos, 1);
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		vmap->render(cam.position); // render all models
		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwPollEvents();
		EInput::poll();
	}
	
	glfwTerminate();
	return 0;
}  
