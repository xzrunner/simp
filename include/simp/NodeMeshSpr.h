#ifndef _SIMP_NODE_MESH_SPR_H_
#define _SIMP_NODE_MESH_SPR_H_

#include <cu/uncopyable.h>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class NodeMeshSpr : private cu::Uncopyable
{
public:
	uint32_t mesh_id, base_id;

	uint32_t n;
	uint16_t trans[1];

public:
	NodeMeshSpr(bimp::Allocator& alloc, ImportStream& is);

	static int Size();

}; // NodeMeshSpr

}

#endif // _SIMP_NODE_MESH_SPR_H_