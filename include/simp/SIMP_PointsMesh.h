#ifndef _SIMP_POINTS_MESH_H_
#define _SIMP_POINTS_MESH_H_

#include "MeshShape.h"

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class PointsMesh : public MeshShape
{
public:
	uint32_t  outline_n;
	uint16_t* outline;

	uint32_t  points_n;
	uint16_t* points;

public:
	PointsMesh(bimp::Allocator& alloc, ImportStream& is);

	static int Size();

}; // PointsMesh

}

#endif // _SIMP_POINTS_MESH_H_