#ifndef __CAMERA_H_DEFINED__
#define __CAMERA_H_DEFINED__

#include "input.hpp"

// depending on:
extern int g_Screen_x;
extern int g_Screen_y;

// defined here:
struct Camera;
void initCamera(Camera &cam, GLFWwindow* window);
void updateCamera(Camera &cam, GLFWwindow* window, bool &looking_active, int (&kill_pos)[3], int (&set_pos)[3]);

struct Camera
{
	// position
	glm::vec3 position = glm::vec3( -5, -5, 1 );
	// horizontal angle : toward -Z
	float horizontalAngle = 2.0f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = 0.0f;
	// Initial Field of View
	float FoV = 45.0f;

	float speed = 20.0f; // 3 units / second
	float mouseSpeed = 0.1f;
	
	double lastFrameTime;
};


void initCamera(Camera &cam, GLFWwindow* window){
	glfwSetCursorPos(window, g_Screen_x/2, g_Screen_y/2);
	cam.lastFrameTime = glfwGetTime();
}

void updateCamera(Camera &cam, GLFWwindow* window, bool &looking_active, int (&kill_pos)[3], int (&set_pos)[3]){
	
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - cam.lastFrameTime);
	cam.lastFrameTime = currentTime;
	
	// reed mouse speeds
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, g_Screen_x/2, g_Screen_y/2);
	
	// cam angles
	cam.horizontalAngle -= 0.01*cam.mouseSpeed * float(g_Screen_x/2 - xpos );
	cam.verticalAngle  -= 0.01*cam.mouseSpeed * float(g_Screen_y/2 - ypos );
	
	// clip it!
	if(cam.verticalAngle < 3.14f/2){cam.verticalAngle = 3.14f/2;}
	if(cam.verticalAngle > 3.14f/2*3){cam.verticalAngle = 3.14f/2*3;}
	
	// calc direction
	glm::vec3 direction(
		cos(cam.verticalAngle) * sin(cam.horizontalAngle),
		cos(cam.verticalAngle) * cos(cam.horizontalAngle),
		sin(cam.verticalAngle));
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(cam.horizontalAngle - 3.14f/2.0f),
		cos(cam.horizontalAngle - 3.14f/2.0f),
		0);
	
	// Up vector : perpendicular to both direction and right
	glm::vec3 up = glm::cross( right, direction );
	
	glm::vec3 globalup = glm::vec3(0,0,1);
	glm::vec3 walkdirection = -glm::cross( right, globalup);
	
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
	    cam.position += walkdirection * deltaTime * cam.speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS){
	    cam.position -= walkdirection * deltaTime * cam.speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
	    cam.position += right * deltaTime * cam.speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
	    cam.position -= right * deltaTime * cam.speed;
	}
	
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS){
	    cam.position += globalup * deltaTime * cam.speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_E ) == GLFW_PRESS){
	    cam.position -= globalup * deltaTime * cam.speed;
	}
	
	
	
	//cam.FoV -= 5 * glfwGetMouseWheel(window);
	
	// Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(cam.FoV), (float)g_Screen_x / (float)g_Screen_y, 0.1f, 300.0f);
	// Camera matrix
	glm::mat4 ViewMatrix = glm::lookAt(
	    cam.position,           // Camera is here
	    cam.position+direction, // and looks here : at the same position, plus "direction"
	    up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 projview_mat = ProjectionMatrix * ViewMatrix;
	GLuint location = glGetUniformLocation(g_shaderProgram, "ProjectView_mat");
	assert(location != -1);
	glUniformMatrix4fv(location, 1, false, &projview_mat[0][0]);
	
	
	if (glfwGetKey(window, GLFW_KEY_SPACE ) == GLFW_PRESS){
	    std::cout << "FPS: " << (int)(1.0/deltaTime) <<  ", x: " << sin(cam.horizontalAngle)<< ", y: " << cos(cam.horizontalAngle) << std::endl;
	    
	}
	
	
	
	//bool &looking_active, int &kill_pos[3], int &set_pos[3]
	{
		looking_active = true;
		
		
		GLfloat depth_comp;
		glReadPixels( g_Screen_x/2, g_Screen_y/2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth_comp);
		
		if(depth_comp < 1.0f){
			
			//std::cout <<"depth: "<< depth_comp << std::endl;
			glm::vec3 look = glm::unProject(glm::vec3( g_Screen_x/2, g_Screen_y/2, depth_comp ),
			    					ViewMatrix, ProjectionMatrix,
			    					glm::vec4(0,0,g_Screen_x, g_Screen_y));
			//std::cout << "look: " << look[0] << ", "<< look[1] << ", "<< look[2] << std::endl;
			
			glm::vec3 rel_look_vec = look - cam.position;
			
			double dist = glm::length(rel_look_vec);
			
			if(dist < 20){
				looking_active = true;
				
				glm::vec3 kill_pos_loc = look + 0.01f* glm::normalize(rel_look_vec);
				
				kill_pos[0] = floor(kill_pos_loc[0]);
				kill_pos[1] = floor(kill_pos_loc[1]);
				kill_pos[2] = floor(kill_pos_loc[2]);
				
				//std::cout << "[kill] dist: " << dist << ": " << kill_pos[0] << ", " << kill_pos[1] << ", " << kill_pos[2] << std::endl;
				
				glm::vec3 set_pos_loc = look - 0.01f* glm::normalize(rel_look_vec);
				
				set_pos[0] = floor(set_pos_loc[0]);
				set_pos[1] = floor(set_pos_loc[1]);
				set_pos[2] = floor(set_pos_loc[2]);
				
				//std::cout << "[set] dist: " << dist << ": " << set_pos[0] << ", " << set_pos[1] << ", " << set_pos[2] << std::endl;
			}
		}
	}
	
}

#endif
