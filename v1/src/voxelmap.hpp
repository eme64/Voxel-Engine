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
		empty_chunk->SetEmpty();
	}

	void calculateCollision(glm::vec3 &pos, glm::vec3 &velocity, float &rx, float &ry, float &rz){
		// v will be updated !

		int min_block[3];
		int max_block[3];

		min_block[0] = std::min(pos[0], pos[0] + velocity[0])-3.0-rx; // make sure to get one boundary each!
		min_block[1] = std::min(pos[1], pos[1] + velocity[1])-3.0-ry;
		min_block[2] = std::min(pos[2], pos[2] + velocity[2])-3.0-rz;

		max_block[0] = std::max(pos[0], pos[0] + velocity[0])+3.0+rx;
		max_block[1] = std::max(pos[1], pos[1] + velocity[1])+3.0+ry;
		max_block[2] = std::max(pos[2], pos[2] + velocity[2])+3.0+rz;

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
									 = cc->block[xx][yy][zz].t;
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
		glm::mat4 scale_it = glm::scale(glm::vec3(1.0/rx,1.0/ry,1.0/rz));

		glm::mat4 do_transform = scale_it * to_zero;

		glm::mat4 inv_transform = inverse(do_transform);

		glm::vec4 velo4 = do_transform*glm::vec4(velocity,0);
		glm::vec3 velo3(velo4[0], velo4[1], velo4[2]);

		bool hasCollision = true; // first time must check
		int interationCounter = 0;
		while(hasCollision && interationCounter < 1000){
			interationCounter++;
			if(interationCounter > 2){
				std::cout << interationCounter << std::endl;
				if(interationCounter >= 1000){
					std::cout << "Collision went on for 1000 Cycles -> Was stopped !" << std::endl;
				}
			}

			hasCollision = false; // reset for this round
			for(int lx = 1; lx < max_block[0]-min_block[0]; lx++){
				for(int ly = 1; ly < max_block[1]-min_block[1]; ly++){
					for(int lz = 1; lz < max_block[2]-min_block[2]; lz++){
						int offx = lx + min_block[0];
						int offy = ly + min_block[1];
						int offz = lz + min_block[2];

						if(Block_type[block_copy[lx][ly][lz]].solid){

							if(!Block_type[block_copy[lx-1][ly][lz]].solid)
								{// x = 0
									glm::vec4 pp0 = do_transform*glm::vec4(offx+0,offy+0,offz+0,1);
									glm::vec4 pp1 = do_transform*glm::vec4(offx+0,offy+1,offz+1,1);
									glm::vec4 pp2 = do_transform*glm::vec4(offx+0,offy+0,offz+1,1);
									glm::vec4 pp3 = do_transform*glm::vec4(offx+0,offy+1,offz+0,1);

									hasCollision = hasCollision || renderCollisionRectSphere(velo3, pp0, pp1, pp2, pp3);
								}

							if(!Block_type[block_copy[lx+1][ly][lz]].solid)
								{// x = 1
									glm::vec4 pp1 = do_transform*glm::vec4(offx+1,offy+0,offz+0,1);
									glm::vec4 pp0 = do_transform*glm::vec4(offx+1,offy+1,offz+1,1);
									glm::vec4 pp2 = do_transform*glm::vec4(offx+1,offy+0,offz+1,1);
									glm::vec4 pp3 = do_transform*glm::vec4(offx+1,offy+1,offz+0,1);

									hasCollision = hasCollision || renderCollisionRectSphere(velo3, pp0, pp1, pp2, pp3);
								}

							if(!Block_type[block_copy[lx][ly+1][lz]].solid)
								{// y = 1
									glm::vec4 pp0 = do_transform*glm::vec4(offx+0,offy+1,offz+0,1);
									glm::vec4 pp1 = do_transform*glm::vec4(offx+1,offy+1,offz+1,1);
									glm::vec4 pp2 = do_transform*glm::vec4(offx+0,offy+1,offz+1,1);
									glm::vec4 pp3 = do_transform*glm::vec4(offx+1,offy+1,offz+0,1);

									hasCollision = hasCollision || renderCollisionRectSphere(velo3, pp0, pp1, pp2, pp3);
								}

							if(!Block_type[block_copy[lx][ly-1][lz]].solid)
								{// y = 0
									glm::vec4 pp1 = do_transform*glm::vec4(offx+0,offy+0,offz+0,1);
									glm::vec4 pp0 = do_transform*glm::vec4(offx+1,offy+0,offz+1,1);
									glm::vec4 pp2 = do_transform*glm::vec4(offx+0,offy+0,offz+1,1);
									glm::vec4 pp3 = do_transform*glm::vec4(offx+1,offy+0,offz+0,1);

									hasCollision = hasCollision || renderCollisionRectSphere(velo3, pp0, pp1, pp2, pp3);
								}

							if(!Block_type[block_copy[lx][ly][lz-1]].solid)
								{// z = 0
									glm::vec4 pp0 = do_transform*glm::vec4(offx+0,offy+0,offz+0,1);
									glm::vec4 pp1 = do_transform*glm::vec4(offx+1,offy+1,offz+0,1);
									glm::vec4 pp2 = do_transform*glm::vec4(offx+0,offy+1,offz+0,1);
									glm::vec4 pp3 = do_transform*glm::vec4(offx+1,offy+0,offz+0,1);

									hasCollision = hasCollision || renderCollisionRectSphere(velo3, pp0, pp1, pp2, pp3);
								}

							if(!Block_type[block_copy[lx][ly][lz+1]].solid)
								{// z = 1
									glm::vec4 pp1 = do_transform*glm::vec4(offx+0,offy+0,offz+1,1);
									glm::vec4 pp0 = do_transform*glm::vec4(offx+1,offy+1,offz+1,1);
									glm::vec4 pp2 = do_transform*glm::vec4(offx+0,offy+1,offz+1,1);
									glm::vec4 pp3 = do_transform*glm::vec4(offx+1,offy+0,offz+1,1);

									hasCollision = hasCollision || renderCollisionRectSphere(velo3, pp0, pp1, pp2, pp3);
								}
						}
					}
				}
			}
		}

		// return velocity to original coordinates:
		velo4 = inv_transform*glm::vec4(velo3,0);
		velocity = glm::vec3(velo4[0], velo4[1], velo4[2]);
	}

	bool renderCollisionRectSphere(glm::vec3 &velocity, glm::vec4 &pp0, glm::vec4 &pp1, glm::vec4 &pp2, glm::vec4 &pp3)
	{
		// assume:
		// pos = (0,0,0), r=1

		glm::vec3 p0(pp0[0],pp0[1],pp0[2]);
		glm::vec3 p1(pp1[0],pp1[1],pp1[2]);
		glm::vec3 p2(pp2[0],pp2[1],pp2[2]);
		glm::vec3 p3(pp3[0],pp3[1],pp3[2]);

		bool locCol = false;
		locCol = locCol || renderCollisionTriangleSphere(velocity, p0, p2, p1);
		locCol = locCol || renderCollisionTriangleSphere(velocity, p0, p1, p3);

		if(!locCol){
			// check edges:
			locCol = locCol || renderCollisionEdgeSphere(velocity, p0, p2);
			locCol = locCol || renderCollisionEdgeSphere(velocity, p0, p3);
			locCol = locCol || renderCollisionEdgeSphere(velocity, p1, p2);
			locCol = locCol || renderCollisionEdgeSphere(velocity, p1, p3);
		}

		if(!locCol){
			// check corners:
			locCol = locCol || renderCollisionPointSphere(velocity, p0);
			locCol = locCol || renderCollisionPointSphere(velocity, p1);
			locCol = locCol || renderCollisionPointSphere(velocity, p2);
			locCol = locCol || renderCollisionPointSphere(velocity, p3);
		}

		return locCol;
	}

	bool getLowestRoot(float a, float b, float c, float maxR, float &root){
		// taken from paper
		// Ax²+Bx+C=0
		float determinant = b*b - 4.0f*a*c;

		if(determinant < 0.0f) return false; // no solution

		float sqrtD = sqrt(determinant);
		float r1 = (-b-sqrtD)/(2*a);
		float r2 = (-b+sqrtD)/(2*a);

		if(r1 > r2){// sort
			float temp = r2;
			r2 = r1;
			r1 = temp;
		}

		if(r1 > 0 && r1 < maxR){
			root = r1;
			return true;
		}

		if(r2 > 0 && r2 < maxR){
			root = r2;
			return true;
		}
		return false;
	}

	bool renderCollisionPointSphere(glm::vec3 &velocity, glm::vec3 &point)
	{
		// assume:
		// pos = (0,0,0), r=1

		float x; // solve equation:
		float A = glm::length2(velocity);
		float B = 2.0f * glm::dot(velocity,-point);
		float C = glm::length2(point)-1.0f;
		bool locCol = getLowestRoot(A,B,C, 1.0f, x);

		if(locCol){
			glm::vec3 collisionPoint = velocity*x;
			glm::vec3 collPlaneNormal = collisionPoint - point;

			// project final pos to plane:
			float distFromPlaneAfter = glm::dot(collPlaneNormal, velocity-collisionPoint)-0.0001f;
			velocity = velocity - distFromPlaneAfter*collPlaneNormal;
			return true;
		}

		return false;
	}

	bool renderCollisionEdgeSphere(glm::vec3 &velocity, glm::vec3 &p1, glm::vec3 &p2)
	{
		// assume:
		// pos = (0,0,0), r=1

		glm::vec3 edge = p2-p1;
		float edgeSquaredLength = glm::length2(edge);
		float edgeDotVelocity = glm::dot(edge, velocity);
		float edgeDotBaseToVertex = glm::dot(edge, p1);
		float velocitySquaredLength = glm::length2(velocity);
		float velocityDotp1 = glm::dot(velocity, p1);
		float p1SquaredLength = glm::length2(p1);

		// I trust the paper again:
		float A = edgeSquaredLength*-velocitySquaredLength+edgeDotVelocity*edgeDotVelocity;
		float B = edgeSquaredLength*(2.0f*velocityDotp1)-2.0f*edgeDotVelocity*edgeDotBaseToVertex;
		float C = edgeSquaredLength*(1-p1SquaredLength)+edgeDotBaseToVertex*edgeDotBaseToVertex;
		float x; // solve equation:
		bool locCol = getLowestRoot(A,B,C, 1.0f, x);

		if(locCol){
			// check if in segment
			float f=(edgeDotVelocity*x-edgeDotBaseToVertex)/edgeSquaredLength;
			if(f>=0.0f && f <= 1.0f){
				glm::vec3 collisionPoint = velocity*x;
				glm::vec3 collisionPointOnSegment = p1 + f*edge;

				glm::vec3 collPlaneNormal = collisionPoint - collisionPointOnSegment;

				float distFromPlaneAfter = glm::dot(collPlaneNormal, velocity-collisionPoint)-0.0001f;
				velocity = velocity - distFromPlaneAfter*collPlaneNormal;

				return true;
			}
		}

		return false;
	}

	bool renderCollisionTriangleSphere(glm::vec3 &velocity, glm::vec3 &a, glm::vec3 &b, glm::vec3 &c){
		// assume:
		// pos = (0,0,0), r=1

		if(glm::length(velocity) == 0){ // safety
			return false;
		}

		// analyze plane:
		glm::vec3 v1 = glm::vec3(b[0]-a[0],b[1]-a[1],b[2]-a[2]);
		glm::vec3 v2 = glm::vec3(c[0]-a[0],c[1]-a[1],c[2]-a[2]);

		glm::vec3 normal = glm::normalize(glm::cross(b-a,c-a));

		float dist_before = -glm::dot(normal, a); // planeC
		float dist_plus = glm::dot(normal, velocity);

		if(dist_plus == 0){ // safety
			return false;
		}

		float t0 = (1.0-dist_before) / dist_plus;
		float t1 = (-1.0-dist_before) / dist_plus;


		if(dist_before-1.0f>=-0.0f and dist_before+dist_plus-1.0f<0.0f){
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

				return true;
			}
		}
		return false;
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

		block_t old_id = cc->block[xx][yy][zz].t;
		cc->block[xx][yy][zz].t = 0;

		cc->new_model_requested = true;

		glm::vec3 emit_light_old = Block_type[old_id].light;
		if(emit_light_old[0] > 0 && emit_light_old[1] > 0 && emit_light_old[2] > 0){
			// kill a light -> reset light levels in perimiter
			kill_lights_perimiter(kill_pos);
		}else{
			// kill a normal block -> do nothing
		}

		if(xx == 0)
		{
			getChunk(c_x-1, c_y, c_z)->new_model_requested = true;
		}else if(xx == Chunk::SIZE-1){
			getChunk(c_x+1, c_y, c_z)->new_model_requested = true;
		}

		if(yy == 0)
		{
			getChunk(c_x, c_y-1, c_z)->new_model_requested = true;
		}else if(yy == Chunk::SIZE-1){
			getChunk(c_x, c_y+1, c_z)->new_model_requested = true;
		}

		if(zz == 0)
		{
			getChunk(c_x, c_y, c_z-1)->new_model_requested = true;
		}else if(zz == Chunk::SIZE-1){
			getChunk(c_x, c_y, c_z+1)->new_model_requested = true;
		}
	}

	void kill_lights_perimiter(int (&kill_pos)[3]){
		//std::cout << "kill lights" << std::endl;

		const int radius = 30;

		int c_x0 = floor(((float)kill_pos[0]-radius) / (float)Chunk::SIZE);
		int c_y0 = floor(((float)kill_pos[1]-radius) / (float)Chunk::SIZE);
		int c_z0 = floor(((float)kill_pos[2]-radius) / (float)Chunk::SIZE);

		int c_xc = floor(((float)kill_pos[0]) / (float)Chunk::SIZE);
		int c_yc = floor(((float)kill_pos[1]) / (float)Chunk::SIZE);
		int c_zc = floor(((float)kill_pos[2]) / (float)Chunk::SIZE);

		int c_x1 = floor(((float)kill_pos[0]+radius) / (float)Chunk::SIZE);
		int c_y1 = floor(((float)kill_pos[1]+radius) / (float)Chunk::SIZE);
		int c_z1 = floor(((float)kill_pos[2]+radius) / (float)Chunk::SIZE);

		int maxdist = std::max((c_xc-c_x0), (c_x1-c_xc)) + std::max((c_yc-c_y0), (c_y1-c_yc)) + std::max((c_zc-c_z0), (c_z1-c_zc));
		//std::cout << "## maxdist: " << maxdist << std::endl;

		// create lists for each distance
		Chunk* dist_lists[maxdist+1][16*maxdist*maxdist];
		int dist_list_counter[maxdist+1];
		for(int i=0; i<= maxdist; i++){dist_list_counter[i] = 0;}

		// set all to zero and add them to right dist_list
		for(int xx=c_x0; xx <= c_x1; xx++){
			for(int yy=c_y0; yy <= c_y1; yy++){
				for(int zz=c_z0; zz <= c_z1; zz++){
					Chunk* cc = getChunk(xx, yy, zz);

					if(cc != empty_chunk){
						cc->light_set_zero();

						int locdist = std::abs(c_xc - xx) + std::abs(c_yc - yy) + std::abs(c_zc - zz);

						//std::cout << "d: " << locdist << " c: " << dist_list_counter[locdist] << std::endl;
						assert(dist_list_counter[locdist]+1 < 16*maxdist*maxdist);

						dist_lists[locdist][dist_list_counter[locdist]] = cc;
						dist_list_counter[locdist]++;




					}
				}
			}
		}

		// run light update quietly on all
		for(int i=0; i<= maxdist; i++){
			//std::cout << "layer " << i << " counting " << dist_list_counter[i] << std::endl;

			for(int j=0; j<dist_list_counter[i]; j++){

				Chunk* cneighbors[3][3][3];
				int pos_x = dist_lists[i][j]->pos.x;
				int pos_y = dist_lists[i][j]->pos.y;
				int pos_z = dist_lists[i][j]->pos.z;


				for(int xx = -1; xx <2; xx++)
					for(int yy = -1; yy <2; yy++)
						for(int zz = -1; zz <2; zz++)
							cneighbors[1+xx][1+yy][1+zz] = getChunk(pos_x+xx, pos_y+yy, pos_z+zz);

				dist_lists[i][j]->renderLights(cneighbors);
			}
		}
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
		cc->block[xx][yy][zz].t = id;

		cc->new_model_requested = true;

		glm::vec3 emit_light = Block_type[id].light;
		if(emit_light[0] > 0 && emit_light[1] > 0 && emit_light[2] > 0){
			// set a light -> do nothing
		}else{
			// set a non-light -> reset light levels in perimiter
			kill_lights_perimiter(set_pos);
		}

		if(xx == 0)
		{
			getChunk(c_x-1, c_y, c_z)->new_model_requested = true;
		}else if(xx == Chunk::SIZE-1){
			getChunk(c_x+1, c_y, c_z)->new_model_requested = true;
		}

		if(yy == 0)
		{
			getChunk(c_x, c_y-1, c_z)->new_model_requested = true;
		}else if(yy == Chunk::SIZE-1){
			getChunk(c_x, c_y+1, c_z)->new_model_requested = true;
		}

		if(zz == 0)
		{
			getChunk(c_x, c_y, c_z-1)->new_model_requested = true;
		}else if(zz == Chunk::SIZE-1){
			getChunk(c_x, c_y, c_z+1)->new_model_requested = true;
		}
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

			if((c->model == NULL || !c->light_is_stable || c->new_model_requested) && count_model_generations < 20){
				Chunk* cneighbors[3][3][3];



				for(int xx = -1; xx <2; xx++)
					for(int yy = -1; yy <2; yy++)
						for(int zz = -1; zz <2; zz++)
							cneighbors[1+xx][1+yy][1+zz] = getChunk(pos.x+xx, pos.y+yy, pos.z+zz);

				if(c->renderLights(cneighbors)){
					// stable
					//std::cout << "## light now stable" << std::endl;

					c->new_model_requested = true;
				}else{
					// unstable
					//std::cout << "# light unstable" << std::endl;

					//c->new_model_requested = false; // because need redo lights anyway !

					for(int xx = -1; xx <2; xx++)
						for(int yy = -1; yy <2; yy++)
							for(int zz = -1; zz <2; zz++)
								if(cneighbors[1+xx][1+yy][1+zz]!=NULL){cneighbors[1+xx][1+yy][1+zz]->light_is_stable = false;}
				}

				if(c->new_model_requested && c->light_is_stable){c->createModel(cneighbors);}

				count_model_generations++;

				//c->render();
			}

			if(c->model != NULL){
				c->render();
			}
		}
	}
};
#endif
