#ifndef __VOXELMAP_H_DEFINED__
#define __VOXELMAP_H_DEFINED__
// dependencies:
#include <map>

#include "FastNoise/FastNoise.h"
//https://github.com/Auburns/FastNoise/wiki

// defined in here:
struct VoxelMap;

// definitions below:
typedef std::map<ChunkPosition, Chunk*, cmpByChunkPosition> chunkMap_t;
struct VoxelMap
{
	// keeps pointers to chunks:
	chunkMap_t map;
	
	FastNoise myNoise1;
	FastNoise myNoise2;
	
	Chunk* empty_chunk;
	
	VoxelMap(int init_x, int init_y, int init_z){
		myNoise1.SetNoiseType(FastNoise::SimplexFractal);
		myNoise1.SetSeed(1000);
		myNoise1.SetFrequency(0.05);
		
		myNoise2.SetNoiseType(FastNoise::SimplexFractal);
		myNoise2.SetSeed(1111);
		
		for(int i=0;i<init_x;i++){
			for(int j=0;j<init_y;j++){
				for(int k=0;k<init_z;k++){
					// generate some chunk
					Chunk* chunk = new Chunk;
					chunk->SetData(ChunkPosition{i,j,k}, myNoise1, myNoise2);
					//chunk->createModel();
					addChunk(chunk);// add to map
				}
			}
		}
		
		empty_chunk = new Chunk;
	}
	
	void blockKill(int (&kill_pos)[3]){
		int c_x = floor((float)kill_pos[0] / (float)Chunk::SIZE);
		int c_y = floor((float)kill_pos[1] / (float)Chunk::SIZE);
		int c_z = floor((float)kill_pos[2] / (float)Chunk::SIZE);
		
		int xx = kill_pos[0]-c_x*Chunk::SIZE;
		int yy = kill_pos[1]-c_y*Chunk::SIZE;
		int zz = kill_pos[2]-c_z*Chunk::SIZE;
		
		Chunk* cc = getChunk(c_x, c_y, c_z);
		
		if(cc == empty_chunk){
			std::cout << "null chunk" << std::endl;
			return;
		}
		
		std::cout << "del block "<< xx << ", " << yy << ", " << zz << std::endl;
		cc->block[xx][yy][zz] = 0;
		
		std::cout << "rem model" << std::endl;
		cc->model->clear(); // reset -> will redo automatically!
		delete cc->model;
		cc->model = NULL;
	}
	
	void blockSet(int (&set_pos)[3], block_t id){
		int c_x = floor((float)set_pos[0] / (float)Chunk::SIZE);
		int c_y = floor((float)set_pos[1] / (float)Chunk::SIZE);
		int c_z = floor((float)set_pos[2] / (float)Chunk::SIZE);
		
		int xx = set_pos[0]-c_x*Chunk::SIZE;
		int yy = set_pos[1]-c_y*Chunk::SIZE;
		int zz = set_pos[2]-c_z*Chunk::SIZE;
		
		Chunk* cc = getChunk(c_x, c_y, c_z);
		
		if(cc == empty_chunk){
			std::cout << "null chunk" << std::endl;
			return;
		}
		
		std::cout << "set block "<< xx << ", " << yy << ", " << zz << std::endl;
		cc->block[xx][yy][zz] = id;
		
		std::cout << "rem model" << std::endl;
		cc->model->clear(); // reset -> will redo automatically!
		delete cc->model;
		cc->model = NULL;
	}
	
	void addChunk(Chunk* c){
		map.insert(std::pair<ChunkPosition, Chunk*>(c->pos, c));
	}
	
	Chunk* getChunk(int x, int y, int z){
		std::map<ChunkPosition, Chunk*>::iterator it;
		
		it = map.find(ChunkPosition{x,y,z});
		
		if(it != map.end()){
			return it->second;
		}else{
			return empty_chunk;
		}
	}
	
	void render(){
		std::map<ChunkPosition, Chunk*>::iterator it;
		for (it=map.begin(); it!=map.end(); ++it)
		{
			Chunk* c = it->second;
			ChunkPosition pos = it->first;
			
			if(c->model == NULL){
				Chunk* c_zp = getChunk(pos.x, pos.y, pos.z+1);
				Chunk* c_zm = getChunk(pos.x, pos.y, pos.z-1);
				Chunk* c_xp = getChunk(pos.x+1, pos.y, pos.z);
				Chunk* c_xm = getChunk(pos.x-1, pos.y, pos.z);
				Chunk* c_yp = getChunk(pos.x, pos.y+1, pos.z);
				Chunk* c_ym = getChunk(pos.x, pos.y-1, pos.z);
				
				c->createModel(c_xp, c_xm, c_yp, c_ym, c_zp, c_zm);
			}
			
			c->render();
		}
	}
};
#endif
