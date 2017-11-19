#ifndef __MODEL_H_DEFINED__
#define __MODEL_H_DEFINED__

struct Model
{
	GLuint vao; // array
	GLuint IBOSize;
	GLuint VBO; // buffer
	GLuint IBO; // buffer
	
	void clear(){
		// ---------------------------------------- things to undo:
		// glGenVertexArrays(1, &(model->vao));
		// glGenBuffers(1, &(model->VBO));
		// glGenBuffers(1, &(model->IBO));
		
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
	}
};

#endif
