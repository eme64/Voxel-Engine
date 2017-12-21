#ifndef __CHUNK_H_DEFINED__
#define __CHUNK_H_DEFINED__

// dependency classes:
#include <string>

#include "model.hpp"
#include "vertex.hpp"
#include "triangle.hpp"
#include "block.hpp"

#include "FastNoise/FastNoise.h"
//https://github.com/Auburns/FastNoise/wiki

extern GLuint g_shaderProgram;
extern Block* Block_type;

// done in this file:
struct ChunkPosition;
struct cmpByChunkPosition;
struct Chunk;
void calcVertexNormals(const Triangle *pTriangleArray, int numTriangles, Vertex *pVertices, int numVertices);
void _check_gl_error(const char *file, int line);
void displayFunc(const Model& model);

// ok now we start
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

#ifndef offsetof
	#define offsetof(s, m) (size_t)&(((s *)0)->m)
#endif

#define COR_X 0
#define COR_Y 1
#define COR_Z 2
#define COR_W 3

//#define CHUNK_SIZE 16
#define block_t uint8_t // also defined in block.hpp !
struct block_data_t
{
	block_t t; // type -> see Block id's

	// illumination level of block
	glm::vec3 col = glm::vec3(0,0,0);

	// space for more attributes
};

struct ChunkPosition
{
	int x;
	int y;
	int z;
};

struct cmpByChunkPosition {
    bool operator()(const ChunkPosition& a, const ChunkPosition& b) const {
        //return a.length() < b.length();
	if(a.x < b.x){
		return true;
	}else if(a.x > b.x){
		return false;
	}else if(a.y < b.y){
		return true;
	}else if(a.y > b.y){
		return false;
	}else{
		return (a.z < b.z);
	}
    }
};

struct Chunk
{
	static const uint8_t SIZE = 16;
	ChunkPosition pos;

	block_data_t block[Chunk::SIZE][Chunk::SIZE][Chunk::SIZE];

	bool SetData(ChunkPosition newpos, FastNoise &noiseObj1, FastNoise &noiseObj2);
	void SetEmpty();

	Model* model = NULL;
	int model_LOD; // how many blocks rendered together, min 1
	bool createModel(Chunk* cneighbors[3][3][3]);
	//bool createModel_LOD(Chunk* c_xp, Chunk* c_xm, Chunk* c_yp, Chunk* c_ym, Chunk* c_zp, Chunk* c_zm, int lod);
	void render();

	void clearModel();

	bool new_model_requested = true;

	bool renderLights(Chunk* cneighbors[3][3][3]); // returns true if stable
	uint8_t light_direction = 0; // 8 directions
	bool light_is_stable = false;
	void light_set_zero();
};
//block_t Chunk::SIZE;

void Chunk::render()
{
	displayFunc(*model);
}

void displayFunc(const Model& model){
	// Draw all triangles defined in the index buffer object
	// with the vertex data from vertex array in the vertex buffer object
	glBindVertexArray(model.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.IBO);
	glDrawElements(GL_TRIANGLES, model.IBOSize, GL_UNSIGNED_SHORT, 0);
}

bool Chunk::SetData(ChunkPosition newpos, FastNoise &noiseObj1, FastNoise &noiseObj2)
{
	pos = newpos;

	for(int x=0;x<Chunk::SIZE;x++)
		for(int y=0;y<Chunk::SIZE;y++)
			for(int z=0;z<Chunk::SIZE;z++)
			{
				float locval1 = (noiseObj1.GetNoise(x + pos.x*Chunk::SIZE,y + pos.y*Chunk::SIZE,z + pos.z*Chunk::SIZE)+1.0);

				float locval2 = (noiseObj2.GetNoise(x + pos.x*Chunk::SIZE,y + pos.y*Chunk::SIZE,z + pos.z*Chunk::SIZE)+0.5)*3;

				if(locval1 > 1.2){
					if(locval2 > 1){
						block[x][y][z].t = locval2;
					}else{
						block[x][y][z].t = 9;
					}
				}else{
					block[x][y][z].t = 0;
				}

				//std::cout << locval << std::endl;

				/*
				if(x==0 || y==0 || z==0 || x==Chunk::SIZE-1 || y==Chunk::SIZE-1 || z==Chunk::SIZE-1)
				{
					block[x][y][z] = 0;
				}else{
					if(((x+y+z) % 6)<3)
					{
						block[x][y][z] = 1 + ((x+y+z) % 6);
					}else{
						block[x][y][z] = 0;
					}
				}
				*/
			}
}

void Chunk::SetEmpty()
{
	for(int x=0;x<Chunk::SIZE;x++)
		for(int y=0;y<Chunk::SIZE;y++)
			for(int z=0;z<Chunk::SIZE;z++)
				{
					block[x][y][z].t = 0;
					block[x][y][z].col = {1,1,1};
				}
}

void Chunk::clearModel()
{
	if(model != NULL){
		model->clear();
		delete model;
		model = NULL;
	}
}

void copy_chunk_to_data(block_data_t (&data)[Chunk::SIZE+2][Chunk::SIZE+2][Chunk::SIZE+2],
													Chunk* cneighbors[3][3][3],
												block_data_t (&block)[Chunk::SIZE][Chunk::SIZE][Chunk::SIZE])
{
	// copy block into it:
	for(int x=0;x<Chunk::SIZE;x++)
	{
		for(int y=0;y<Chunk::SIZE;y++)
		{
			/*
			for(int z=0;z<Chunk::SIZE;z++)
			{

				data[1+x][1+y][1+z] = cneighbors[1][1][1]->block[x][y][z];
			}
			*/
			std::memcpy(&(data[x+1][y+1][1]), &(block[x][y][0]), Chunk::SIZE*sizeof(block_data_t));
		}
	}

	// corners:
	data[0            ][0            ][0            ] = cneighbors[0][0][0]->block[Chunk::SIZE-1][Chunk::SIZE-1][Chunk::SIZE-1];
	data[0            ][0            ][Chunk::SIZE+1] = cneighbors[0][0][2]->block[Chunk::SIZE-1][Chunk::SIZE-1][0            ];
	data[0            ][Chunk::SIZE+1][0            ] = cneighbors[0][2][0]->block[Chunk::SIZE-1][0            ][Chunk::SIZE-1];
	data[0            ][Chunk::SIZE+1][Chunk::SIZE+1] = cneighbors[0][2][2]->block[Chunk::SIZE-1][0            ][0            ];
	data[Chunk::SIZE+1][0            ][0            ] = cneighbors[2][0][0]->block[0            ][Chunk::SIZE-1][Chunk::SIZE-1];
	data[Chunk::SIZE+1][0            ][Chunk::SIZE+1] = cneighbors[2][0][2]->block[0            ][Chunk::SIZE-1][0            ];
	data[Chunk::SIZE+1][Chunk::SIZE+1][0            ] = cneighbors[2][2][0]->block[0            ][0            ][Chunk::SIZE-1];
	data[Chunk::SIZE+1][Chunk::SIZE+1][Chunk::SIZE+1] = cneighbors[2][2][2]->block[0            ][0            ][0            ];

	// edges:
	for(int i=0; i<Chunk::SIZE; i++)
	{
		data[    1 + i    ][0            ][0            ] = cneighbors[1][0][0]->block[        i    ][Chunk::SIZE-1][Chunk::SIZE-1];
		data[    1 + i    ][Chunk::SIZE+1][0            ] = cneighbors[1][2][0]->block[        i    ][0            ][Chunk::SIZE-1];
		data[    1 + i    ][0            ][Chunk::SIZE+1] = cneighbors[1][0][2]->block[        i    ][Chunk::SIZE-1][0            ];
		data[    1 + i    ][Chunk::SIZE+1][Chunk::SIZE+1] = cneighbors[1][2][2]->block[        i    ][0            ][0            ];

		data[0            ][    1 + i    ][0            ] = cneighbors[0][1][0]->block[Chunk::SIZE-1][        i    ][Chunk::SIZE-1];
		data[Chunk::SIZE+1][    1 + i    ][0            ] = cneighbors[2][1][0]->block[0            ][        i    ][Chunk::SIZE-1];
		data[0            ][    1 + i    ][Chunk::SIZE+1] = cneighbors[0][1][2]->block[Chunk::SIZE-1][        i    ][0            ];
		data[Chunk::SIZE+1][    1 + i    ][Chunk::SIZE+1] = cneighbors[2][1][2]->block[0            ][        i    ][0            ];

		data[0            ][0            ][    1 + i    ] = cneighbors[0][0][1]->block[Chunk::SIZE-1][Chunk::SIZE-1][        i    ];
		data[Chunk::SIZE+1][0            ][    1 + i    ] = cneighbors[2][0][1]->block[0            ][Chunk::SIZE-1][        i    ];
		data[0            ][Chunk::SIZE+1][    1 + i    ] = cneighbors[0][2][1]->block[Chunk::SIZE-1][0            ][        i    ];
		data[Chunk::SIZE+1][Chunk::SIZE+1][    1 + i    ] = cneighbors[2][2][1]->block[0            ][0            ][        i    ];
	}

	//faces:
	for(int i=0; i<Chunk::SIZE; i++)
	{
		for(int h=0; h<Chunk::SIZE; h++)
		{
			data[    1 + i    ][    1 + h    ][0            ] = cneighbors[1][1][0]->block[        i    ][        h    ][Chunk::SIZE-1];
			data[    1 + i    ][    1 + h    ][Chunk::SIZE+1] = cneighbors[1][1][2]->block[        i    ][        h    ][0            ];

			data[    1 + i    ][0            ][    1 + h    ] = cneighbors[1][0][1]->block[        i    ][Chunk::SIZE-1][        h    ];
			data[    1 + i    ][Chunk::SIZE+1][    1 + h    ] = cneighbors[1][2][1]->block[        i    ][0            ][        h    ];

			data[0            ][    1 + i    ][    1 + h    ] = cneighbors[0][1][1]->block[Chunk::SIZE-1][        i    ][        h    ];
			data[Chunk::SIZE+1][    1 + i    ][    1 + h    ] = cneighbors[2][1][1]->block[0            ][        i    ][        h    ];
		}
	}
}

void Chunk::light_set_zero(){
	light_is_stable = false;

	for(int x=0;x<Chunk::SIZE;x++)
		for(int y=0;y<Chunk::SIZE;y++)
			for(int z=0;z<Chunk::SIZE;z++)
				{
					block[x][y][z].col = {0,0,0};
				}
}

bool Chunk::renderLights(Chunk* cneighbors[3][3][3])
{
	block_data_t data[Chunk::SIZE+2][Chunk::SIZE+2][Chunk::SIZE+2];
	copy_chunk_to_data(data, cneighbors, block);

	bool stable = true;
	light_is_stable = false;

	bool always_was_stable = true;

	for(int cycles = 0; cycles < 8 && light_is_stable==false; cycles++){
		stable = true;

		light_direction++;
		light_direction%=8;

		int xstart;
		int ystart;
		int zstart;

		int xstep;
		int ystep;
		int zstep;

		if(light_direction%2 == 0){xstart = 0; xstep = 1;}else{xstart = Chunk::SIZE-1; xstep = -1;}
		if((light_direction/2)%2 == 0){ystart = 0; ystep = 1;}else{ystart = Chunk::SIZE-1; ystep = -1;}
		if((light_direction/4)%2 == 0){zstart = 0; zstep = 1;}else{zstart = Chunk::SIZE-1; zstep = -1;}

		for(int x=xstart; x>=0 && x<Chunk::SIZE;x+= xstep)
			for(int y=ystart;y>=0 && y<Chunk::SIZE;y+= ystep)
				for(int z=zstart;z>=0 && z<Chunk::SIZE;z+= zstep)
				{
					int xx = x+1; // used for accessing the data[][][] block.
					int yy = y+1;
					int zz = z+1;

					if(Block_type[data[xx][yy][zz].t].solid)
					{
						// nothing much, copy block light_emission
						block[x][y][z].col = Block_type[block[x][y][z].t].light;
					}else{
						glm::vec3 old = data[xx][yy][zz].col;
						glm::vec3 res = glm::max( glm::vec3(0.1,0.1,0.1) ,
																	0.9f * glm::max( glm::max(
																						glm::max(data[xx-1][yy][zz].col ,  data[xx+1][yy][zz].col),
																					  glm::max(data[xx][yy-1][zz].col ,  data[xx][yy+1][zz].col)),
																						glm::max(data[xx][yy][zz-1].col ,  data[xx][yy][zz+1].col)));

						if(stable == true && ( res[0] != old[0] || res[1] != old[1] || res[2] != old[2]))
						{
							stable = false;
						}

						data[xx][yy][zz].col = res; // for faster propagation
						block[x][y][z].col = res;
					}
				}

		light_is_stable = stable;

		always_was_stable = always_was_stable && stable;
	}
	return always_was_stable;
}

void get_contour_and_color(bool &cont, glm::vec3 &baseCol,
														const block_data_t &data0, // center
														const block_data_t &data1, // diag
														const block_data_t &data2,
														const block_data_t &data3
													){
	cont =
					Block_type[data1.t].solid
				||Block_type[data2.t].solid
				||Block_type[data3.t].solid;

	baseCol = data0.col;
	float c = 1.0;

	if(!Block_type[data1.t].solid && (!Block_type[data2.t].solid || !Block_type[data3.t].solid)){
		baseCol+=data1.col;
		c+=1.0;
	}
	if(!Block_type[data2.t].solid){
		baseCol+=data2.col;
		c+=1.0;
	}
	if(!Block_type[data3.t].solid){
		baseCol+=data3.col;
		c+=1.0;
	}
	baseCol/=c;
}

//bool Chunk::createModel(Chunk* c_xp, Chunk* c_xm, Chunk* c_yp, Chunk* c_ym, Chunk* c_zp, Chunk* c_zm)
bool Chunk::createModel(Chunk* cneighbors[3][3][3])
{
	new_model_requested = false;
	if(model != NULL){
		clearModel();
	}

	model = new Model; // only allocate if needed
	model_LOD = 1;


	// create data block:
	block_data_t data[Chunk::SIZE+2][Chunk::SIZE+2][Chunk::SIZE+2];
	copy_chunk_to_data(data, cneighbors, block);

	//std::cout << "create new model for (" << pos.x << ", "<< pos.y <<", "<< pos.z << ")"<< std::endl;

	std::vector<Vertex> vert;
	vert.reserve(20);
	std::vector<Triangle> tria;
	tria.reserve(10);

	for(int x=0;x<Chunk::SIZE;x++)
		for(int y=0;y<Chunk::SIZE;y++)
			for(int z=0;z<Chunk::SIZE;z++)
			{
				int xx = x+1; // used for accessing the data[][][] block.
				int yy = y+1;
				int zz = z+1;

				block_t me = data[xx][yy][zz].t;

				Block blk_type = Block_type[me];

				if(blk_type.solid){
					GLfloat offx = x + 	Chunk::SIZE*pos.x;
					GLfloat offy = y + 	Chunk::SIZE*pos.y;
					GLfloat offz = z + 	Chunk::SIZE*pos.z;

					if(!Block_type[data[xx][yy+1][zz].t].solid){
						// x1z

						bool cont_0;
						glm::vec3 baseCol0;
						get_contour_and_color(cont_0, baseCol0, data[xx][yy+1][zz],
																										data[xx-1][yy+1][zz-1],
																										data[xx][yy+1][zz-1],
																										data[xx-1][yy+1][zz]);


						bool cont_1;
						glm::vec3 baseCol1;
						get_contour_and_color(cont_1, baseCol1, data[xx][yy+1][zz],
																										data[xx+1][yy+1][zz+1],
																										data[xx][yy+1][zz+1],
																										data[xx+1][yy+1][zz]);

						bool cont_2;
						glm::vec3 baseCol2;
						get_contour_and_color(cont_2, baseCol2, data[xx][yy+1][zz],
																										data[xx-1][yy+1][zz+1],
																										data[xx][yy+1][zz+1],
																										data[xx-1][yy+1][zz]);

						bool cont_3;
						glm::vec3 baseCol3;
						get_contour_and_color(cont_3, baseCol3, data[xx][yy+1][zz],
																										data[xx+1][yy+1][zz-1],
																										data[xx][yy+1][zz-1],
																										data[xx+1][yy+1][zz]);

						Vertex x0 = {{offx+0,offy+1,offz+0,1}, {baseCol0[0], baseCol0[1], baseCol0[2], 1},(float)cont_0, {0,0,0},  {blk_type.uv[0][0][0],blk_type.uv[0][0][1]}};
						Vertex x1 = {{offx+1,offy+1,offz+1,1}, {baseCol1[0], baseCol1[1], baseCol1[2], 1},(float)cont_1, {0,0,0},  {blk_type.uv[0][1][0],blk_type.uv[0][1][1]}};
						Vertex x2 = {{offx+0,offy+1,offz+1,1}, {baseCol2[0], baseCol2[1], baseCol2[2], 1},(float)cont_2, {0,0,0},  {blk_type.uv[0][2][0],blk_type.uv[0][2][1]}};
						Vertex x3 = {{offx+1,offy+1,offz+0,1}, {baseCol3[0], baseCol3[1], baseCol3[2], 1},(float)cont_3, {0,0,0},  {blk_type.uv[0][3][0],blk_type.uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);

						Triangle t1;
						Triangle t2;

						if((cont_0 || cont_1) && !(cont_2 && cont_3)){
							Triangle t1 = {index+2, index+1, index+3, {0,0,0}};
							Triangle t2 = {index+2, index+3, index+0, {0,0,0}};
						}else{
							Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
							Triangle t2 = {index+0, index+1, index+3, {0,0,0}};
						}

						tria.push_back(t1);
						tria.push_back(t2);
					}
					if(!Block_type[data[xx][yy-1][zz].t].solid){
						// x0z

						bool cont_0;
						glm::vec3 baseCol0;
						get_contour_and_color(cont_0, baseCol0, data[xx][yy-1][zz],
																										data[xx-1][yy-1][zz-1],
																										data[xx][yy-1][zz-1],
																										data[xx-1][yy-1][zz]);


						bool cont_1;
						glm::vec3 baseCol1;
						get_contour_and_color(cont_1, baseCol1, data[xx][yy-1][zz],
																										data[xx+1][yy-1][zz+1],
																										data[xx][yy-1][zz+1],
																										data[xx+1][yy-1][zz]);

						bool cont_2;
						glm::vec3 baseCol2;
						get_contour_and_color(cont_2, baseCol2, data[xx][yy-1][zz],
																										data[xx-1][yy-1][zz+1],
																										data[xx][yy-1][zz+1],
																										data[xx-1][yy-1][zz]);

						bool cont_3;
						glm::vec3 baseCol3;
						get_contour_and_color(cont_3, baseCol3, data[xx][yy-1][zz],
																										data[xx+1][yy-1][zz-1],
																										data[xx][yy-1][zz-1],
																										data[xx+1][yy-1][zz]);

						Vertex x0 = {{offx+0,offy+0,offz+0,1}, {baseCol0[0], baseCol0[1], baseCol0[2], 1},(float)cont_0, {0,0,0},  {blk_type.uv[0][0][0],blk_type.uv[0][0][1]}};
						Vertex x1 = {{offx+1,offy+0,offz+1,1}, {baseCol1[0], baseCol1[1], baseCol1[2], 1},(float)cont_1, {0,0,0},  {blk_type.uv[0][1][0],blk_type.uv[0][1][1]}};
						Vertex x2 = {{offx+0,offy+0,offz+1,1}, {baseCol2[0], baseCol2[1], baseCol2[2], 1},(float)cont_2, {0,0,0},  {blk_type.uv[0][2][0],blk_type.uv[0][2][1]}};
						Vertex x3 = {{offx+1,offy+0,offz+0,1}, {baseCol3[0], baseCol3[1], baseCol3[2], 1},(float)cont_3, {0,0,0},  {blk_type.uv[0][3][0],blk_type.uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);

						Triangle t1;
						Triangle t2;

						if((cont_0 || cont_1) && !(cont_2 && cont_3)){
							Triangle t1 = {index+1, index+2, index+3, {0,0,0}};
							Triangle t2 = {index+0, index+3, index+2, {0,0,0}};
						}else{
							Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
							Triangle t2 = {index+0, index+3, index+1, {0,0,0}};
						}

						tria.push_back(t1);
						tria.push_back(t2);
					}

					if(!Block_type[data[xx-1][yy][zz].t].solid){
						// 0yz
						bool cont_0;
						glm::vec3 baseCol0;
						get_contour_and_color(cont_0, baseCol0, data[xx-1][yy][zz],
																										data[xx-1][yy-1][zz-1],
																										data[xx-1][yy][zz-1],
																										data[xx-1][yy-1][zz]);


						bool cont_1;
						glm::vec3 baseCol1;
						get_contour_and_color(cont_1, baseCol1, data[xx-1][yy][zz],
																										data[xx-1][yy+1][zz+1],
																										data[xx-1][yy][zz+1],
																										data[xx-1][yy+1][zz]);

						bool cont_2;
						glm::vec3 baseCol2;
						get_contour_and_color(cont_2, baseCol2, data[xx-1][yy][zz],
																										data[xx-1][yy-1][zz+1],
																										data[xx-1][yy][zz+1],
																										data[xx-1][yy-1][zz]);

						bool cont_3;
						glm::vec3 baseCol3;
						get_contour_and_color(cont_3, baseCol3, data[xx-1][yy][zz],
																										data[xx-1][yy+1][zz-1],
																										data[xx-1][yy][zz-1],
																										data[xx-1][yy+1][zz]);


						Vertex x0 = {{offx+0,offy+0,offz+0,1}, {baseCol0[0], baseCol0[1], baseCol0[2], 1},(float)cont_0, {0,0,0},  {blk_type.uv[0][0][0],blk_type.uv[0][0][1]}};
						Vertex x1 = {{offx+0,offy+1,offz+1,1}, {baseCol1[0], baseCol1[1], baseCol1[2], 1},(float)cont_1, {0,0,0},  {blk_type.uv[0][1][0],blk_type.uv[0][1][1]}};
						Vertex x2 = {{offx+0,offy+0,offz+1,1}, {baseCol2[0], baseCol2[1], baseCol2[2], 1},(float)cont_2, {0,0,0},  {blk_type.uv[0][2][0],blk_type.uv[0][2][1]}};
						Vertex x3 = {{offx+0,offy+1,offz+0,1}, {baseCol3[0], baseCol3[1], baseCol3[2], 1},(float)cont_3, {0,0,0},  {blk_type.uv[0][3][0],blk_type.uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);

						Triangle t1;
						Triangle t2;

						if((cont_0 || cont_1) && !(cont_2 && cont_3)){
							Triangle t1 = {index+0, index+2, index+3, {0,0,0}};
							Triangle t2 = {index+2, index+1, index+3, {0,0,0}};
						}else{
							Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
							Triangle t2 = {index+0, index+1, index+3, {0,0,0}};
						}


						tria.push_back(t1);
						tria.push_back(t2);
					}

					if(!Block_type[data[xx+1][yy][zz].t].solid){
						// 1yz
						bool cont_0;
						glm::vec3 baseCol0;
						get_contour_and_color(cont_0, baseCol0, data[xx+1][yy][zz],
																										data[xx+1][yy-1][zz-1],
																										data[xx+1][yy][zz-1],
																										data[xx+1][yy-1][zz]);


						bool cont_1;
						glm::vec3 baseCol1;
						get_contour_and_color(cont_1, baseCol1, data[xx+1][yy][zz],
																										data[xx+1][yy+1][zz+1],
																										data[xx+1][yy][zz+1],
																										data[xx+1][yy+1][zz]);

						bool cont_2;
						glm::vec3 baseCol2;
						get_contour_and_color(cont_2, baseCol2, data[xx+1][yy][zz],
																										data[xx+1][yy-1][zz+1],
																										data[xx+1][yy][zz+1],
																										data[xx+1][yy-1][zz]);

						bool cont_3;
						glm::vec3 baseCol3;
						get_contour_and_color(cont_3, baseCol3, data[xx+1][yy][zz],
																										data[xx+1][yy+1][zz-1],
																										data[xx+1][yy][zz-1],
																										data[xx+1][yy+1][zz]);

						Vertex x0 = {{offx+1,offy+0,offz+0,1}, {baseCol0[0], baseCol0[1], baseCol0[2], 1},(float)cont_0, {0,0,0},  {blk_type.uv[0][0][0],blk_type.uv[0][0][1]}};
						Vertex x1 = {{offx+1,offy+1,offz+1,1}, {baseCol1[0], baseCol1[1], baseCol1[2], 1},(float)cont_1, {0,0,0},  {blk_type.uv[0][1][0],blk_type.uv[0][1][1]}};
						Vertex x2 = {{offx+1,offy+0,offz+1,1}, {baseCol2[0], baseCol2[1], baseCol2[2], 1},(float)cont_2, {0,0,0},  {blk_type.uv[0][2][0],blk_type.uv[0][2][1]}};
						Vertex x3 = {{offx+1,offy+1,offz+0,1}, {baseCol3[0], baseCol3[1], baseCol3[2], 1},(float)cont_3, {0,0,0},  {blk_type.uv[0][3][0],blk_type.uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);


						Triangle t1;
						Triangle t2;

						if((cont_0 || cont_1) && !(cont_2 && cont_3)){
							Triangle t1 = {index+0, index+3, index+2, {0,0,0}};
							Triangle t2 = {index+2, index+3, index+1, {0,0,0}};
						}else{
							Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
							Triangle t2 = {index+0, index+3, index+1, {0,0,0}};
						}

						tria.push_back(t1);
						tria.push_back(t2);
					}

					if(!Block_type[data[xx][yy][zz-1].t].solid){
						// xy0
						bool cont_0;
						glm::vec3 baseCol0;
						get_contour_and_color(cont_0, baseCol0, data[xx][yy][zz-1],
																										data[xx-1][yy-1][zz-1],
																										data[xx-1][yy][zz-1],
																										data[xx][yy-1][zz-1]);


						bool cont_1;
						glm::vec3 baseCol1;
						get_contour_and_color(cont_1, baseCol1, data[xx][yy][zz-1],
																										data[xx+1][yy+1][zz-1],
																										data[xx+1][yy][zz-1],
																										data[xx][yy+1][zz-1]);

						bool cont_2;
						glm::vec3 baseCol2;
						get_contour_and_color(cont_2, baseCol2, data[xx][yy][zz-1],
																										data[xx-1][yy+1][zz-1],
																										data[xx-1][yy][zz-1],
																										data[xx][yy+1][zz-1]);

						bool cont_3;
						glm::vec3 baseCol3;
						get_contour_and_color(cont_3, baseCol3, data[xx][yy][zz-1],
																										data[xx+1][yy-1][zz-1],
																										data[xx+1][yy][zz-1],
																										data[xx][yy-1][zz-1]);


						Vertex x0 = {{offx+0,offy+0,offz+0,1}, {baseCol0[0], baseCol0[1], baseCol0[2], 1},(float)cont_0, {0,0,0},  {blk_type.uv[0][0][0],blk_type.uv[0][0][1]}};
						Vertex x1 = {{offx+1,offy+1,offz+0,1}, {baseCol1[0], baseCol1[1], baseCol1[2], 1},(float)cont_1, {0,0,0},  {blk_type.uv[0][1][0],blk_type.uv[0][1][1]}};
						Vertex x2 = {{offx+0,offy+1,offz+0,1}, {baseCol2[0], baseCol2[1], baseCol2[2], 1},(float)cont_2, {0,0,0},  {blk_type.uv[0][2][0],blk_type.uv[0][2][1]}};
						Vertex x3 = {{offx+1,offy+0,offz+0,1}, {baseCol3[0], baseCol3[1], baseCol3[2], 1},(float)cont_3, {0,0,0},  {blk_type.uv[0][3][0],blk_type.uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);


						Triangle t1;
						Triangle t2;

						if((cont_0 || cont_1) && !(cont_2 && cont_3)){
							Triangle t1 = {index+0, index+2, index+3, {0,0,0}};
							Triangle t2 = {index+2, index+1, index+3, {0,0,0}};
						}else{
							Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
							Triangle t2 = {index+0, index+1, index+3, {0,0,0}};
						}

						tria.push_back(t1);
						tria.push_back(t2);
					}

					if(!Block_type[data[xx][yy][zz+1].t].solid){
						// xy1
						bool cont_0;
						glm::vec3 baseCol0;
						get_contour_and_color(cont_0, baseCol0, data[xx][yy][zz+1],
																										data[xx-1][yy-1][zz+1],
																										data[xx-1][yy][zz+1],
																										data[xx][yy-1][zz+1]);


						bool cont_1;
						glm::vec3 baseCol1;
						get_contour_and_color(cont_1, baseCol1, data[xx][yy][zz+1],
																										data[xx+1][yy+1][zz+1],
																										data[xx+1][yy][zz+1],
																										data[xx][yy+1][zz+1]);

						bool cont_2;
						glm::vec3 baseCol2;
						get_contour_and_color(cont_2, baseCol2, data[xx][yy][zz+1],
																										data[xx-1][yy+1][zz+1],
																										data[xx-1][yy][zz+1],
																										data[xx][yy+1][zz+1]);

						bool cont_3;
						glm::vec3 baseCol3;
						get_contour_and_color(cont_3, baseCol3, data[xx][yy][zz+1],
																										data[xx+1][yy-1][zz+1],
																										data[xx+1][yy][zz+1],
																										data[xx][yy-1][zz+1]);
																										

						Vertex x0 = {{offx+0,offy+0,offz+1,1}, {baseCol0[0], baseCol0[1], baseCol0[2], 1},(float)cont_0, {0,0,0},  {blk_type.uv[0][0][0],blk_type.uv[0][0][1]}};
						Vertex x1 = {{offx+1,offy+1,offz+1,1}, {baseCol1[0], baseCol1[1], baseCol1[2], 1},(float)cont_1, {0,0,0},  {blk_type.uv[0][1][0],blk_type.uv[0][1][1]}};
						Vertex x2 = {{offx+0,offy+1,offz+1,1}, {baseCol2[0], baseCol2[1], baseCol2[2], 1},(float)cont_2, {0,0,0},  {blk_type.uv[0][2][0],blk_type.uv[0][2][1]}};
						Vertex x3 = {{offx+1,offy+0,offz+1,1}, {baseCol3[0], baseCol3[1], baseCol3[2], 1},(float)cont_3, {0,0,0},  {blk_type.uv[0][3][0],blk_type.uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);

						Triangle t1;
						Triangle t2;

						if((cont_0 || cont_1) && !(cont_2 && cont_3)){
							Triangle t1 = {index+0, index+3, index+2, {0,0,0}};
							Triangle t2 = {index+2, index+3, index+1, {0,0,0}};
						}else{
							Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
							Triangle t2 = {index+0, index+3, index+1, {0,0,0}};
						}

						tria.push_back(t1);
						tria.push_back(t2);
					}

				}

			}


	// ------------- convert vector to array
	int numVertices = vert.size();
	Vertex *pVertexArray = &vert[0];

	int numTriangles = tria.size();
	Triangle *pTriangleArray = &tria[0];

	// ------------- calculate normals
	for(int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		// Positions of the triangles nodes
		GLfloat positionA[3];
		GLfloat positionB[3];
		GLfloat positionC[3];

		// Get the positions of the triangles nodes
		int A = pTriangleArray[triIndex].vtxIndexA;
		int B = pTriangleArray[triIndex].vtxIndexB;
		int C = pTriangleArray[triIndex].vtxIndexC;

		GLfloat Ax = pVertexArray[A].pos[COR_X];
		GLfloat Ay = pVertexArray[A].pos[COR_Y];
		GLfloat Az = pVertexArray[A].pos[COR_Z];

		GLfloat Bx = pVertexArray[B].pos[COR_X];
		GLfloat By = pVertexArray[B].pos[COR_Y];
		GLfloat Bz = pVertexArray[B].pos[COR_Z];

		GLfloat Cx = pVertexArray[C].pos[COR_X];
		GLfloat Cy = pVertexArray[C].pos[COR_Y];
		GLfloat Cz = pVertexArray[C].pos[COR_Z];

		// Vectors for the calculation of the normal
		GLfloat vec1[3];
		GLfloat vec2[3];

		// Calculate two difference vectors of the triangle
		vec1[COR_X] = Ax - Bx;
		vec1[COR_Y] = Ay - By;
		vec1[COR_Z] = Az - Bz;

		vec2[COR_X] = Ax - Cx;
		vec2[COR_Y] = Ay - Cy;
		vec2[COR_Z] = Az - Cz;

		// Calculate the normal vector: cross product between x and y
		pTriangleArray[triIndex].normal[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
		pTriangleArray[triIndex].normal[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
		pTriangleArray[triIndex].normal[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];

		// Normalize the normal vector:
		GLfloat norm = pTriangleArray[triIndex].normal[0]*pTriangleArray[triIndex].normal[0]
				+ pTriangleArray[triIndex].normal[1]*pTriangleArray[triIndex].normal[1]
				+ pTriangleArray[triIndex].normal[2]*pTriangleArray[triIndex].normal[2];
		norm = sqrt(norm);

		pTriangleArray[triIndex].normal[0] /= norm;
		pTriangleArray[triIndex].normal[1] /= norm;
		pTriangleArray[triIndex].normal[2] /= norm;
	}

	// Calculate vertex normals
	calcVertexNormals(pTriangleArray, numTriangles, pVertexArray, numVertices);

	model->IBOSize = numTriangles * 3;
	GLshort *pIndices = new GLshort[model->IBOSize];
	for(int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		pIndices[3*triIndex + 0] = pTriangleArray[triIndex].vtxIndexA;
		pIndices[3*triIndex + 1] = pTriangleArray[triIndex].vtxIndexB;
		pIndices[3*triIndex + 2] = pTriangleArray[triIndex].vtxIndexC;
	}


	// ------------- send to GL
	//GLuint vao;
	glGenVertexArrays(1, &(model->vao));
	glBindVertexArray(model->vao);

	// Create vertex buffer object and download vertex array
	model->VBO = 0;
	glGenBuffers(1, &(model->VBO));
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, pVertexArray, GL_STATIC_DRAW);

	GLint location;
	// bind Vertex shader per-vertex attributes
	// load positions
	location = glGetAttribLocation(g_shaderProgram, "position");
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(location);

	// load color
	location = glGetAttribLocation(g_shaderProgram, "color_in");
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(location);

	// load cornerness
	location = glGetAttribLocation(g_shaderProgram, "cornerness");
	glVertexAttribPointer(location, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, cornerness));
	glEnableVertexAttribArray(location);

/*
	// load normal
	location = glGetAttribLocation(g_shaderProgram, "normal");
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(location);
*/
	// load uv
	location = glGetAttribLocation(g_shaderProgram, "vertexUV");
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(location);

	// Create index buffer and download index data
	model->IBO = 0;
	glGenBuffers(1, &(model->IBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*model->IBOSize, pIndices, GL_STATIC_DRAW);


	delete[] pIndices;
	// not free because from vector !
	//delete[] pVertexArray;
	//delete[] pTriangleArray;

	check_gl_error();

	return true;
}

/*
bool Chunk::createModel_LOD(Chunk* c_xp, Chunk* c_xm, Chunk* c_yp, Chunk* c_ym, Chunk* c_zp, Chunk* c_zm, int lod)
{
	if(model != NULL){
		model->clear();
		std::cout << "kill old mesh" << std::endl;

		delete model;
		model = NULL;
		//std::cout << "you want to recreate the model -> fail!" << std::endl;
		//return false;
	}

	model = new Model; // only allocate if needed
	model_LOD = lod;

	//std::cout << "create new model for (" << pos.x << ", "<< pos.y <<", "<< pos.z << ")"<< std::endl;

	std::vector<Vertex> vert;
	vert.reserve(20);
	std::vector<Triangle> tria;
	tria.reserve(10);

	for(int x=0;x<Chunk::SIZE;x=x+lod)
		for(int y=0;y<Chunk::SIZE;y=y+lod)
			for(int z=0;z<Chunk::SIZE;z=z+lod)
			{
				block_t me = block[x][y][z];

				Block blk_type = Block_type[me];

				if(blk_type.solid){

					GLfloat cr = blk_type.low_res_col[0];
					GLfloat cg = blk_type.low_res_col[1];
					GLfloat cb = blk_type.low_res_col[2];

					block_t xm;
					block_t xp;
					block_t ym;
					block_t yp;
					block_t zm;
					block_t zp;

					if(x==0){
						xm = c_xm->block[Chunk::SIZE-lod][y][z];
					}else{
						xm = block[x-lod][y][z];
					}

					if(x==Chunk::SIZE-lod){
						xp = c_xp->block[0][y][z];
					}else{
						xp = block[x+lod][y][z];
					}

					if(y==0){
						ym = c_ym->block[x][Chunk::SIZE-lod][z];
					}else{
						ym = block[x][y-lod][z];
					}

					if(y==Chunk::SIZE-lod){
						yp = c_yp->block[x][0][z];
					}else{
						yp = block[x][y+lod][z];
					}

					if(z==0){
						zm = c_zm->block[x][y][Chunk::SIZE-lod];
					}else{
						zm = block[x][y][z-lod];
					}

					if(z==Chunk::SIZE-lod){
						zp = c_zp->block[x][y][0];
					}else{
						zp = block[x][y][z+lod];
					}


					GLfloat offx = x + 	Chunk::SIZE*pos.x;
					GLfloat offy = y + 	Chunk::SIZE*pos.y;
					GLfloat offz = z + 	Chunk::SIZE*pos.z;

					if(yp==0){
						// x1z
						Vertex x0 = {{offx+0  ,offy+lod,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][0][0],Block_type[0].uv[0][0][1]}};
						Vertex x1 = {{offx+lod,offy+lod,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][1][0],Block_type[0].uv[0][1][1]}};
						Vertex x2 = {{offx+0  ,offy+lod,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][2][0],Block_type[0].uv[0][2][1]}};
						Vertex x3 = {{offx+lod,offy+lod,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][3][0],Block_type[0].uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);

						Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
						Triangle t2 = {index+0, index+1, index+3, {0,0,0}};

						tria.push_back(t1);
						tria.push_back(t2);
					}
					if(ym==0){
						// x0z
						Vertex x0 = {{offx+0  ,offy+0  ,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][0][0],Block_type[0].uv[0][0][1]}};
						Vertex x1 = {{offx+lod,offy+0  ,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][1][0],Block_type[0].uv[0][1][1]}};
						Vertex x2 = {{offx+0  ,offy+0  ,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][2][0],Block_type[0].uv[0][2][1]}};
						Vertex x3 = {{offx+lod,offy+0  ,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][3][0],Block_type[0].uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);

						Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
						Triangle t2 = {index+0, index+3, index+1, {0,0,0}};

						tria.push_back(t1);
						tria.push_back(t2);
					}

					if(xm==0){
						// 0yz
						Vertex x0 = {{offx+0  ,offy+0  ,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][0][0],Block_type[0].uv[0][0][1]}};
						Vertex x1 = {{offx+0  ,offy+lod,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][1][0],Block_type[0].uv[0][1][1]}};
						Vertex x2 = {{offx+0  ,offy+0  ,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][2][0],Block_type[0].uv[0][2][1]}};
						Vertex x3 = {{offx+0  ,offy+lod,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][3][0],Block_type[0].uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);

						Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
						Triangle t2 = {index+0, index+1, index+3, {0,0,0}};

						tria.push_back(t1);
						tria.push_back(t2);
					}

					if(xp==0){
						// 0yz
						Vertex x0 = {{offx+lod,offy+0  ,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][0][0],Block_type[0].uv[0][0][1]}};
						Vertex x1 = {{offx+lod,offy+lod,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][1][0],Block_type[0].uv[0][1][1]}};
						Vertex x2 = {{offx+lod,offy+0  ,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][2][0],Block_type[0].uv[0][2][1]}};
						Vertex x3 = {{offx+lod,offy+lod,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][3][0],Block_type[0].uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);

						Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
						Triangle t2 = {index+0, index+3, index+1, {0,0,0}};

						tria.push_back(t1);
						tria.push_back(t2);
					}

					if(zm==0){
						// xy0
						Vertex x0 = {{offx+0  ,offy+0  ,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][0][0],Block_type[0].uv[0][0][1]}};
						Vertex x1 = {{offx+lod,offy+lod,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][1][0],Block_type[0].uv[0][1][1]}};
						Vertex x2 = {{offx+0  ,offy+lod,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][2][0],Block_type[0].uv[0][2][1]}};
						Vertex x3 = {{offx+lod,offy+0  ,offz+0  ,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][3][0],Block_type[0].uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);

						Triangle t1 = {index+0, index+2, index+1, {0,0,0}};
						Triangle t2 = {index+0, index+1, index+3, {0,0,0}};

						tria.push_back(t1);
						tria.push_back(t2);
					}

					if(zp==0){
						// xy1
						Vertex x0 = {{offx+0  ,offy+0  ,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][0][0],Block_type[0].uv[0][0][1]}};
						Vertex x1 = {{offx+lod,offy+lod,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][1][0],Block_type[0].uv[0][1][1]}};
						Vertex x2 = {{offx+0  ,offy+lod,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][2][0],Block_type[0].uv[0][2][1]}};
						Vertex x3 = {{offx+lod,offy+0  ,offz+lod,1},{cr,cg,cb,1},{0,0,0},  {Block_type[0].uv[0][3][0],Block_type[0].uv[0][3][1]}};

						int index = vert.size();
						vert.push_back(x0);
						vert.push_back(x1);
						vert.push_back(x2);
						vert.push_back(x3);

						Triangle t1 = {index+0, index+1, index+2, {0,0,0}};
						Triangle t2 = {index+0, index+3, index+1, {0,0,0}};

						tria.push_back(t1);
						tria.push_back(t2);
					}

				}

			}


	// ------------- convert vector to array
	int numVertices = vert.size();
	Vertex *pVertexArray = &vert[0];

	int numTriangles = tria.size();
	Triangle *pTriangleArray = &tria[0];

	// ------------- calculate normals
	for(int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		// Positions of the triangles nodes
		GLfloat positionA[3];
		GLfloat positionB[3];
		GLfloat positionC[3];

		// Get the positions of the triangles nodes
		int A = pTriangleArray[triIndex].vtxIndexA;
		int B = pTriangleArray[triIndex].vtxIndexB;
		int C = pTriangleArray[triIndex].vtxIndexC;

		GLfloat Ax = pVertexArray[A].pos[COR_X];
		GLfloat Ay = pVertexArray[A].pos[COR_Y];
		GLfloat Az = pVertexArray[A].pos[COR_Z];

		GLfloat Bx = pVertexArray[B].pos[COR_X];
		GLfloat By = pVertexArray[B].pos[COR_Y];
		GLfloat Bz = pVertexArray[B].pos[COR_Z];

		GLfloat Cx = pVertexArray[C].pos[COR_X];
		GLfloat Cy = pVertexArray[C].pos[COR_Y];
		GLfloat Cz = pVertexArray[C].pos[COR_Z];

		// Vectors for the calculation of the normal
		GLfloat vec1[3];
		GLfloat vec2[3];

		// Calculate two difference vectors of the triangle
		vec1[COR_X] = Ax - Bx;
		vec1[COR_Y] = Ay - By;
		vec1[COR_Z] = Az - Bz;

		vec2[COR_X] = Ax - Cx;
		vec2[COR_Y] = Ay - Cy;
		vec2[COR_Z] = Az - Cz;

		// Calculate the normal vector: cross product between x and y
		pTriangleArray[triIndex].normal[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1];
		pTriangleArray[triIndex].normal[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2];
		pTriangleArray[triIndex].normal[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0];

		// Normalize the normal vector:
		GLfloat norm = pTriangleArray[triIndex].normal[0]*pTriangleArray[triIndex].normal[0]
				+ pTriangleArray[triIndex].normal[1]*pTriangleArray[triIndex].normal[1]
				+ pTriangleArray[triIndex].normal[2]*pTriangleArray[triIndex].normal[2];
		norm = sqrt(norm);

		pTriangleArray[triIndex].normal[0] /= norm;
		pTriangleArray[triIndex].normal[1] /= norm;
		pTriangleArray[triIndex].normal[2] /= norm;
	}

	// Calculate vertex normals
	calcVertexNormals(pTriangleArray, numTriangles, pVertexArray, numVertices);

	model->IBOSize = numTriangles * 3;
	GLshort *pIndices = new GLshort[model->IBOSize];
	for(int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		pIndices[3*triIndex + 0] = pTriangleArray[triIndex].vtxIndexA;
		pIndices[3*triIndex + 1] = pTriangleArray[triIndex].vtxIndexB;
		pIndices[3*triIndex + 2] = pTriangleArray[triIndex].vtxIndexC;
	}


	// ------------- send to GL
	//GLuint vao;
	glGenVertexArrays(1, &(model->vao));
	glBindVertexArray(model->vao);

	// Create vertex buffer object and download vertex array
	model->VBO = 0;
	glGenBuffers(1, &(model->VBO));
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, pVertexArray, GL_STATIC_DRAW);

	GLint location;
	// bind Vertex shader per-vertex attributes
	// load positions
	location = glGetAttribLocation(g_shaderProgram, "position");
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(location);

	// load color
	location = glGetAttribLocation(g_shaderProgram, "color_in");
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(location);

	// load normal
	location = glGetAttribLocation(g_shaderProgram, "normal");
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(location);

	// load uv
	location = glGetAttribLocation(g_shaderProgram, "vertexUV");
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(location);

	// Create index buffer and download index data
	model->IBO = 0;
	glGenBuffers(1, &(model->IBO));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort)*model->IBOSize, pIndices, GL_STATIC_DRAW);


	delete[] pIndices;
	// not free because from vector !
	//delete[] pVertexArray;
	//delete[] pTriangleArray;

	check_gl_error();

	return true;
}
*/

//=======================================================================
// Calculates and defines the surface normals for all vertices by
// combining the normals of all adjacent triangles.
//=======================================================================
// IN:  pTriangleArray: List of triangles
//      numTriangles:   number of triangles in pTriangleArray
//      numVertices:    Number of triangles in pVertices
// OUT: pVertices:      The 'normal' member of each vertex is modified
//=======================================================================
void calcVertexNormals(const Triangle *pTriangleArray, int numTriangles, Vertex *pVertices, int numVertices)
{
	// Set all vertex normals to zero
	for(int vtxIndex = 0; vtxIndex < numVertices; vtxIndex++)
	{
		pVertices[vtxIndex].normal[0] = 0.0f;
		pVertices[vtxIndex].normal[1] = 0.0f;
		pVertices[vtxIndex].normal[2] = 0.0f;
	}

	// For each vertex, sum up normals of adjacent triangles
	for(int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		int A = pTriangleArray[triIndex].vtxIndexA;
		int B = pTriangleArray[triIndex].vtxIndexB;
		int C = pTriangleArray[triIndex].vtxIndexC;

		pVertices[A].normal[0] += pTriangleArray[triIndex].normal[0];
		pVertices[A].normal[1] += pTriangleArray[triIndex].normal[1];
		pVertices[A].normal[2] += pTriangleArray[triIndex].normal[2];

		pVertices[B].normal[0] += pTriangleArray[triIndex].normal[0];
		pVertices[B].normal[1] += pTriangleArray[triIndex].normal[1];
		pVertices[B].normal[2] += pTriangleArray[triIndex].normal[2];

		pVertices[C].normal[0] += pTriangleArray[triIndex].normal[0];
		pVertices[C].normal[1] += pTriangleArray[triIndex].normal[1];
		pVertices[C].normal[2] += pTriangleArray[triIndex].normal[2];
	}

	// Normalize all vertex normals
	for(int vtxIndex = 0; vtxIndex < numVertices; vtxIndex++)
	{
		GLfloat norm = pVertices[vtxIndex].normal[0]*pVertices[vtxIndex].normal[0]
				+pVertices[vtxIndex].normal[1]*pVertices[vtxIndex].normal[1]
				+pVertices[vtxIndex].normal[2]*pVertices[vtxIndex].normal[2];
		norm = sqrt(norm);
		pVertices[vtxIndex].normal[0] /= norm;
		pVertices[vtxIndex].normal[1] /= norm;
		pVertices[vtxIndex].normal[2] /= norm;
	}
}

void _check_gl_error(const char *file, int line) {
	GLenum err (glGetError());

	while(err!=GL_NO_ERROR) {
			std::string error;

			switch(err) {
					case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
					case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
					case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
					case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
					case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
			}

			std::cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<std::endl;
			err=glGetError();
	}
}


#endif
