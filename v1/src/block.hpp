#ifndef __BLOCK_H_DEFINED__
#define __BLOCK_H_DEFINED__

#define block_t uint8_t // also defined in chunk.hpp !

struct Block
{
	block_t id;

	bool solid;

	GLfloat uv[6][4][2]; // face, vertex, uv
	/*
	----- face:

	0   top
	1-4 sides
	5   bottom

	*/

	GLfloat low_res_col[3];
	// used if LOD coarser -> should match texture!

	// zero for non lights, some color for lights!
	glm::vec3 light = glm::vec3(0,0,0);
};

Block* Block_type; // --------------------- global!

void init_block_types(int n){
	// initialize Array where blocktypes will be stored
	Block_type = new Block[n];
}

void set_block_type_harmonious(int id, bool solid,
				int tex_x, int tex_y, int tex_n,
				GLfloat lrc_r, GLfloat lrc_g, GLfloat lrc_b,
				glm::vec3 light_emission){
	// all blockfaces have same texture

	Block_type[id].id = id;
	Block_type[id].solid = solid;

	Block_type[id].low_res_col[0] = lrc_r;
	Block_type[id].low_res_col[1] = lrc_g;
	Block_type[id].low_res_col[2] = lrc_b;

	GLfloat dx = 1.0/tex_n;
	GLfloat x0 = dx*tex_x;
	GLfloat y0 = dx*tex_y;

	for(int i=0; i<6; i++){
		Block_type[id].uv[i][0][0] = x0;
		Block_type[id].uv[i][0][1] = y0;

		Block_type[id].uv[i][1][0] = x0 + dx;
		Block_type[id].uv[i][1][1] = y0 + dx;

		Block_type[id].uv[i][2][0] = x0;
		Block_type[id].uv[i][2][1] = y0 + dx;

		Block_type[id].uv[i][3][0] = x0 + dx;
		Block_type[id].uv[i][3][1] = y0;
	}
	
	Block_type[id].light = light_emission;
}
#endif
