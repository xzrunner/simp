#ifndef _SIMP_SKELETON2_MESH_H_
#define _SIMP_SKELETON2_MESH_H_

#include "MeshShape.h"

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class Skeleton2Mesh : public MeshShape
{
public:
	struct Item
	{
		uint16_t joint;
		uint16_t vx, vy;
		uint16_t weight;
	};

public:
	uint32_t  vertices_n;
	uint8_t*  items_n;
	Item*     items;

	uint32_t  texcoords_n;
	uint16_t* texcoords;

	uint32_t  triangle_n;
	uint16_t* triangle;

public:
	Skeleton2Mesh(bimp::Allocator& alloc, ImportStream& is);

	virtual MeshType Type() const { return MESH_SKELETON2; }

	static int Size();

	static int ItemSize();

}; // Skeleton2Mesh

}

#endif // _SIMP_SKELETON2_MESH_H_