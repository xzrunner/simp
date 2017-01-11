#ifndef _SIMP_TRIANGLES_MESH_H_
#define _SIMP_TRIANGLES_MESH_H_

#include "MeshShape.h"

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class TrianglesMesh : public MeshShape
{
public:
	uint32_t  vertices_n;
	uint16_t* vertices;

	uint32_t  texcoords_n;
	uint16_t* texcoords;

	uint32_t  triangle_n;
	uint16_t* triangle;
	
public:
	TrianglesMesh(bimp::Allocator& alloc, ImportStream& is);

	virtual MeshType Type() const { return MESH_TRIANGLES; }

	static int Size();

}; // TrianglesMesh

}

#endif // _SIMP_TRIANGLES_MESH_H_