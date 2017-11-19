#version 150

//=======================================================================
// Uniforms: Constant over all vertices
//=======================================================================
uniform vec4 lightPos;				// Position of light in model coordinates
uniform mat4 Model_mat; 			// Transformation matrix from model 
									// coordinates to world coordinates
uniform mat4 ProjectView_mat;		// View/camera projection matrix

//=======================================================================
// Attributes: Per vertex data
//=======================================================================
in vec4 position;			// Vertex coordinates in model space
in vec4 color_in;			// Vertex color
in vec3 normal;				// Vertex normal

in vec2 vertexUV;

//=======================================================================
// Varying: Vertex shader output, input to rasterizer
//=======================================================================
out vec4 color_out;				// shaded color for this vertex

out vec2 UV;
//=======================================================================
// Computes the intensity of the reflected diffuse light for a point
// at position 'position' with normal vector 'normal', if the light
// source is located at 'lightPos'.
//=======================================================================
float computeDiffuseIntens(in vec3 position, in vec3 normal, in vec3 lightPos)
{
	normal = normalize(normal);
	vec3 vecToLight = normalize(lightPos.xyz - position.xyz);
	
	float diffuseIntensity = dot(normal, vecToLight);
	
	diffuseIntensity = clamp(diffuseIntensity, 0.0, 1.0);
	
	return diffuseIntensity;
}

//=======================================================================
// Vertex program entry point
//=======================================================================
void main(void)
{
	vec4 c = color_in;
	
	// Do some simple diffuse shading
	float diffuseIntensity = computeDiffuseIntens(position.xyz, normal, lightPos.xyz);
	float ambientIntensity = 0.3;
	
	color_out = c*(diffuseIntensity + ambientIntensity);
	
	// Project vertex coordinates to screen
	gl_Position = ProjectView_mat * Model_mat * position;
	
	UV = vertexUV;
}
