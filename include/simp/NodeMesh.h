#ifndef _SIMP_NODE_MESH_H_
#define _SIMP_NODE_MESH_H_

#include <cu/uncopyable.h>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;
class MeshShape;

class NodeMesh : private cu::Uncopyable
{
public:
	uint32_t base_id;
	uint32_t padding;

	MeshShape* shape;

public:
	NodeMesh(bimp::Allocator& alloc, ImportStream& is);

	static int Size();

}; // NodeMesh

}

#endif // _SIMP_NODE_MESH_H_