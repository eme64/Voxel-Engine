#ifndef __VOXELMAP_H_DEFINED__
#define __VOXELMAP_H_DEFINED__
// dependencies:
#include <map>

#include "FastNoise/FastNoise.h"
//https://github.com/Auburns/FastNoise/wiki

// needed for collision detection:
typedef unsigned int uint32;
#define coll_in(a) ((uint32&) a)

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
	
	void calculateCollision(glm::vec3 &pos, glm::vec3 &velocity, float &r){
		// v will be updated !
		
		int min_block[3];
		int max_block[3];
		
		min_block[0] = std::min(pos[0], pos[0] + velocity[0])-3.0-r; // make sure to get one boundary each!
		min_block[1] = std::min(pos[1], pos[1] + velocity[1])-3.0-r;
		min_block[2] = std::min(pos[2], pos[2] + velocity[2])-3.0-r;
		
		max_block[0] = std::max(pos[0], pos[0] + velocity[0])+3.0+r;
		max_block[1] = std::max(pos[1], pos[1] + velocity[1])+3.0+r;
		max_block[2] = std::max(pos[2], pos[2] + velocity[2])+3.0+r;
		
		// iterate over corresponding chunks to copy block data:
		
		block_t block_copy[max_block[0] - min_block[0] + 1][max_block[1] - min_block[1] + 1][max_block[2] - min_block[2] + 1];
		
		// safety memory -> zero
		for(int lx = 0; lx <= max_block[0]-min_block[0]; lx++){
			for(int ly = 0; ly <= max_block[1]-min_block[1]; ly++){
				for(int lz = 0; lz <= max_block[2]-min_block[2]; lz++){
					block_copy[lx][ly][lz] = 0;
				}
			}
		}
		
		
		for(int cx = floor((float)min_block[0]/(float)Chunk::SIZE); cx <= floor((float)max_block[0]/(float)Chunk::SIZE); cx++){
			for(int cy = floor((float)min_block[1]/(float)Chunk::SIZE); cy <= floor((float)max_block[1]/(float)Chunk::SIZE); cy++){
				for(int cz = floor((float)min_block[2]/(float)Chunk::SIZE); cz <= floor((float)max_block[2]/(float)Chunk::SIZE); cz++){
					Chunk* cc = getChunk(cx, cy, cz);
					
					if(cc != empty_chunk){
						int loc_x_min = std::max(0, min_block[0]-cx*Chunk::SIZE);
						int loc_y_min = std::max(0, min_block[1]-cy*Chunk::SIZE);
						int loc_z_min = std::max(0, min_block[2]-cz*Chunk::SIZE);
						
						int loc_x_max = std::min(Chunk::SIZE-1, max_block[0]-cx*Chunk::SIZE);
						int loc_y_max = std::min(Chunk::SIZE-1, max_block[1]-cy*Chunk::SIZE);
						int loc_z_max = std::min(Chunk::SIZE-1, max_block[2]-cz*Chunk::SIZE);
						
						for(int xx = loc_x_min; xx <= loc_x_max; xx++){
							for(int yy = loc_y_min; yy <= loc_y_max; yy++){
								for(int zz = loc_z_min; zz <= loc_z_max; zz++){
									block_copy[xx+cx*Chunk::SIZE  - min_block[0]]
										[yy+cy*Chunk::SIZE  - min_block[1]]
										[zz+cz*Chunk::SIZE  - min_block[2]]
									 = cc->block[xx][yy][zz];
								}
							}
						}
					}
				}
			}
		}
		
		// ----------  iterate over copied data to check collisions:
		
		// deform points to allign with unit circle:
		
		// push to zero:
		glm::mat4 to_zero = glm::translate(-pos);
		
		// scale
		glm::mat4 scale_it = glm::scale(glm::vec3(1.0,1.0,1.0));
		
		glm::mat4 do_transform = scale_it * to_zero;
		
		glm::mat4 inv_transform = inverse(do_transform);
		
		glm::vec4 velo4 = do_transform*glm::vec4(velocity,0);
		glm::vec3 velo3(velo4[0], velo4[1], velo4[2]);
		
		for(int lx = 1; lx < max_block[0]-min_block[0]; lx++){
			for(int ly = 1; ly < max_block[1]-min_block[1]; ly++){
				for(int lz = 1; lz < max_block[2]-min_block[2]; lz++){
					int offx = lx + min_block[0];
					int offy = ly + min_block[1];
					int offz = lz + min_block[2];
					
					if(Block_type[block_copy[lx][ly][lz]].solid){
						
						if(!Block_type[block_copy[lx-1][ly][lz]].solid){
							
							glm::vec4 pp0 = do_transform*glm::vec4(offx+0,offy+0,offz+0,1);
							glm::vec4 pp1 = do_transform*glm::vec4(offx+0,offy+1,offz+1,1);
							glm::vec4 pp2 = do_transform*glm::vec4(offx+0,offy+0,offz+1,1);
							glm::vec4 pp3 = do_transform*glm::vec4(offx+0,offy+1,offz+0,1);
							
							glm::vec3 p0(pp0[0],pp0[1],pp0[2]);
							glm::vec3 p1(pp1[0],pp1[1],pp1[2]);
							glm::vec3 p2(pp2[0],pp2[1],pp2[2]);
							glm::vec3 p3(pp3[0],pp3[1],pp3[2]);
							
							renderCollisionTriangleSphere(velo3, p0, p2, p1);
							renderCollisionTriangleSphere(velo3, p0, p1, p3);
						}
					}
				}
			}
		}
		
		// return velocity to original coordinates:
		velo4 = inv_transform*glm::vec4(velo3,0);
		velocity = glm::vec3(velo4[0], velo4[1], velo4[2]);
	}
	
	void renderCollisionTriangleSphere(glm::vec3 &velocity, glm::vec3 &a, glm::vec3 &b, glm::vec3 &c){
		// assume:
		// pos = (0,0,0), r=1
		
		if(glm::length(velocity) == 0){ // safety
			return;
		}
		
		// analyze plane:
		glm::vec3 v1 = glm::vec3(b[0]-a[0],b[1]-a[1],b[2]-a[2]);
		glm::vec3 v2 = glm::vec3(c[0]-a[0],c[1]-a[1],c[2]-a[2]);
		
		glm::vec3 normal = glm::normalize(glm::cross(b-a,c-a));
		
		float dist_before = -glm::dot(normal, a); // planeC
		float dist_plus = glm::dot(normal, velocity);
		
		if(dist_plus == 0){ // safety
			return;
		}
		
		float t0 = (1.0-dist_before) / dist_plus;
		float t1 = (-1.0-dist_before) / dist_plus;
		
		
		if(dist_before-1.0>=-0.001 and dist_before+dist_plus-1.0<0){
			// possible collision:
			
			glm::vec3 intersect = /*  base point (0,0,0) */ -normal + t0*velocity;
			
			// test if in triangle:
			float aa = glm::dot(v1,v1);
			float bb = glm::dot(v1,v2);
			float cc = glm::dot(v2,v2);
			
			float ac_bb=(aa*cc)-(bb*bb);
			
			glm::vec3 vp = intersect-a;
			
			float dd = glm::dot(vp, v1);
			float ee = glm::dot(vp, v2);
			float xx = (dd*cc)-(ee*bb);
			float yy = (ee*aa)-(dd*bb);
			float zz = xx+yy-ac_bb;
			
			/*
			/----------- from a paper:
			
			VECTOR e10=pb-pa;
			VECTOR e20=pc-pa;
			float a = e10.dot(e10);
			float b = e10.dot(e20);
			float c = e20.dot(e20);
			float ac_bb=(a*c)-(b*b);
			VECTOR vp(point.x-pa.x, point.y-pa.y, point.z-pa.z);
			float d = vp.dot(e10);
			float e = vp.dot(e20);
			float x = (d*c)-(e*b);
			float y = (e*a)-(d*b);
			float z = x+y-ac_bb;
			return (( in(z)& ~(in(x)|in(y)) ) & 0x80000000);
			*/
			
			if ( (( coll_in(zz)& ~(coll_in(xx)|coll_in(yy)) ) & 0x80000000)){
				velocity = velocity - normal*(dist_plus);
				//std::cout << "collision!" << std::endl;
			}
			
			
			// continue with edge
			
			// and do corner:
			
			
			// possibly outside this if block?
		}
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
	
	void render(glm::vec3 &view_pos){
		std::map<ChunkPosition, Chunk*>::iterator it;
		
		int count_model_generations = 0;
		
		for (it=map.begin(); it!=map.end(); ++it)
		{
			Chunk* c = it->second;
			ChunkPosition pos = it->first;
			
			if(c->model == NULL && count_model_generations < 2){
				Chunk* c_zp = getChunk(pos.x, pos.y, pos.z+1);
				Chunk* c_zm = getChunk(pos.x, pos.y, pos.z-1);
				Chunk* c_xp = getChunk(pos.x+1, pos.y, pos.z);
				Chunk* c_xm = getChunk(pos.x-1, pos.y, pos.z);
				Chunk* c_yp = getChunk(pos.x, pos.y+1, pos.z);
				Chunk* c_ym = getChunk(pos.x, pos.y-1, pos.z);
				
				c->createModel(c_xp, c_xm, c_yp, c_ym, c_zp, c_zm);
				
				/*
				int distx = abs(pos.x - (view_pos[0] + 0.5*Chunk::SIZE)/Chunk::SIZE);
				int disty = abs(pos.y - (view_pos[1] + 0.5*Chunk::SIZE)/Chunk::SIZE);
				int distz = abs(pos.z - (view_pos[2] + 0.5*Chunk::SIZE)/Chunk::SIZE);
				
				int dist = std::max(distx, std::max(disty, distz));
				
				if(dist < 2){
					c->createModel(c_xp, c_xm, c_yp, c_ym, c_zp, c_zm);
				}else if(dist < 4){
					c->createModel_LOD(c_xp, c_xm, c_yp, c_ym, c_zp, c_zm,2);
				}else{
					c->createModel_LOD(c_xp, c_xm, c_yp, c_ym, c_zp, c_zm,4);
				}
				*/
				count_model_generations++;
				
				c->render();
			}else if(c->model != NULL){
				c->render();
			}
			
			
		}
	}
};
#endif
