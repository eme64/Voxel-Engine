#ifndef __VOXELMAP_H_DEFINED__
#define __VOXELMAP_H_DEFINED__
// dependencies:
#include <map>

// defined in here:
struct VoxelMap;

// definitions below:
typedef std::map<ChunkPosition, Chunk*, cmpByChunkPosition> chunkMap_t;
struct VoxelMap
{
	// keeps pointers to chunks:
	chunkMap_t map;
	
	void addChunk(Chunk* c){
		map.insert(std::pair<ChunkPosition, Chunk*>(c->pos, c));
	}
	
	void render(){
		std::map<ChunkPosition, Chunk*>::iterator it;
		for (it=map.begin(); it!=map.end(); ++it)
		{
			Chunk* c = it->second;
			c->render();
		}
	}
};
#endif
