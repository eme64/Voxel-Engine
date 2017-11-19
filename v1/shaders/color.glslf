#version 150

in vec4 color_out;
out vec4 color;

in vec2 UV;
uniform sampler2D myTextureSampler;

//=======================================================================
// Fragment program entry point
//=======================================================================
void main(void)
{
	color[0] = texture(myTextureSampler, UV).rgb[0] * color_out[0];
	color[1] = texture(myTextureSampler, UV).rgb[1] * color_out[1];
	color[2] = texture(myTextureSampler, UV).rgb[2] * color_out[2];
	
	color[3] = color_out[3];
	
}
