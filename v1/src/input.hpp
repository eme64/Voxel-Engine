#ifndef __E_INPUT_H_DEFINED__
#define __E_INPUT_H_DEFINED__

// --------------------  to be defined below:
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void error_callback(int, const char* err_str);

// -------------------- THE MAIN STRUCT
class EInput
{
public:
	// Mouse
	static bool mouseDown[2];
	static bool mouseHit[2];
	

	static void poll(){
		// resets all inputs like mouseHit
	}
	
	static bool MouseHit(int i){
		bool ret = mouseHit[i];
		mouseHit[i] = false;
		return ret;
		
	}
	
	static void init(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetErrorCallback(error_callback);
	}
};

// -----------------------   declare static members:
bool EInput::mouseDown[2];
bool EInput::mouseHit[2];

// ---------------------- REMAINING FUNCTION DEFINITION

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	
	// even with this we can show lag in input ending!
	
	/*
	if (key == GLFW_KEY_W && action == GLFW_PRESS){
		std::cout << "W start" << std::endl;
	}
	
	if (key == GLFW_KEY_W && action == GLFW_RELEASE){
		std::cout << "W end" << std::endl;
	}
	*/
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT){
    	if(action == GLFW_PRESS){
    		EInput::mouseDown[0] = true;
    		EInput::mouseHit[0] = true;
    	}else{
    		EInput::mouseDown[0] = false;
    	}
    }else if (button == GLFW_MOUSE_BUTTON_RIGHT){
    	if(action == GLFW_PRESS){
    		EInput::mouseDown[1] = true;
    		EInput::mouseHit[1] = true;
    	}else{
    		EInput::mouseDown[1] = false;
    	}
    }
}

void error_callback(int, const char* err_str)
{
	std::cout << "GLFW Error: " << err_str << std::endl;
}
#endif
