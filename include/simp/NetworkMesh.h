#ifndef _SIMP_NETWORK_MESH_H_
#define _SIMP_NETWORK_MESH_H_

#include "MeshShape.h"

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class NetworkMesh : public MeshShape
{
public:
	uint32_t  outer_n;
	uint16_t* outer;

	uint32_t  inner_n;
	uint16_t* inner;

public:
	NetworkMesh(bimp::Allocator& alloc, ImportStream& is);

	virtual MeshType Type() const { return MESH_NETWORK; }

	static int Size();

}; // NetworkMesh

}

#endif // _SIMP_NETWORK_MESH_H_