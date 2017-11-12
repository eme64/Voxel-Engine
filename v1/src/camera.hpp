#ifndef __CAMERA_H_DEFINED__
#define __CAMERA_H_DEFINED__

// depending on:
extern int g_Screen_x;
extern int g_Screen_y;

// defined here:
struct Camera;
void initCamera(Camera &cam, GLFWwindow* window);
void updateCamera(Camera &cam, GLFWwindow* window);

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


void initCamera(Camera &cam, GLFWwindow* window){
	glfwSetCursorPos(window, g_Screen_x/2, g_Screen_y/2);
	cam.lastFrameTime = glfwGetTime();
}

void updateCamera(Camera &cam, GLFWwindow* window){
	
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - cam.lastFrameTime);
	cam.lastFrameTime = currentTime;
	
	// reed mouse speeds
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, g_Screen_x/2, g_Screen_y/2);
	
	// cam angles
	cam.horizontalAngle += cam.mouseSpeed * deltaTime * float(g_Screen_x/2 - xpos );
	cam.verticalAngle   += cam.mouseSpeed * deltaTime * float(g_Screen_y/2 - ypos );
	
	// calc direction
	glm::vec3 direction(
		cos(cam.verticalAngle) * sin(cam.horizontalAngle),
		sin(cam.verticalAngle),
		cos(cam.verticalAngle) * cos(cam.horizontalAngle));
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(cam.horizontalAngle - 3.14f/2.0f),
		0,
		cos(cam.horizontalAngle - 3.14f/2.0f));
	
	// Up vector : perpendicular to both direction and right
	glm::vec3 up = glm::cross( right, direction );
	
	glm::vec3 globalup = glm::vec3(0,1,0);
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
	
	if (glfwGetKey(window, GLFW_KEY_SPACE ) == GLFW_PRESS){
	    std::cout << cam.horizontalAngle << ", " << cam.verticalAngle << std::endl;
	}
	
	//cam.FoV -= 5 * glfwGetMouseWheel(window);
	
	// Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(cam.FoV), 4.0f / 3.0f, 0.1f, 100.0f);
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
}

#endif
