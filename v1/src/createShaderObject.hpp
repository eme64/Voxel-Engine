#ifndef __CREATESHADEROBJECT_H_DEFINED__
#define __CREATESHADEROBJECT_H_DEFINED__

// defined here:
std::string readFile(const char *filePath);
GLuint createShaderObject(const std::string &vertex_file, const std::string &fragment_file);

// definitions below:

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

#endif
