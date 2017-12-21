#version 150

//=======================================================================
// Uniforms: Constant over all vertices
//=======================================================================
uniform mat4 Model_mat; 			// Transformation matrix from model

uniform mat4 ProjectView_mat;		// View/camera projection matrix

//=======================================================================
// Attributes: Per vertex data
//=======================================================================
in vec4 position;			// Vertex coordinates in model space
in vec4 color_in;			// Vertex color

in float cornerness;			// Vertex cornerness

in vec2 vertexUV;

//=======================================================================
// Varying: Vertex shader output, input to rasterizer
//=======================================================================
out vec4 color_out;				// shaded color for this vertex

out float cornerness_out;
out vec2 UV;

//=======================================================================
// Vertex program entry point
//=======================================================================
void main(void)
{
	color_out = color_in;
	cornerness_out = cornerness;

	// Project vertex coordinates to screen
	gl_Position = ProjectView_mat * Model_mat * position;

	UV = vertexUV;
}
