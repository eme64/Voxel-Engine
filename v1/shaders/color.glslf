#version 150

in vec4 color_out;
in float cornerness_out;
out vec4 color;

in vec2 UV;
uniform sampler2D myTextureSampler;

//=======================================================================
// Fragment program entry point
//=======================================================================
void main(void)
{
	float ccc = 1.0 - 0.25*cornerness_out*cornerness_out;
	color[0] = texture(myTextureSampler, UV).rgb[0] * color_out[0] * ccc;
	color[1] = texture(myTextureSampler, UV).rgb[1] * color_out[1] * ccc;
	color[2] = texture(myTextureSampler, UV).rgb[2] * color_out[2] * ccc;

	color[3] = color_out[3];

}
