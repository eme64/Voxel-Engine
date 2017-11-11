#version 150

in vec4 color_out;
out vec4 color;

//=======================================================================
// Fragment program entry point
//=======================================================================
void main(void)
{
	color = color_out;
}