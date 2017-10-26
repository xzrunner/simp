#include "simp/TrianglesMesh.h"
#include "simp/ImportStream.h"
#include "simp/simp_define.h"
#include "simp/NodeArray.h"

#include <bimp/typedef.h>

namespace simp
{

TrianglesMesh::TrianglesMesh(bimp::Allocator& alloc, ImportStream& is)
{
	type = MESH_TRIANGLES;

	uint16_t n;
	vertices = NodeArray::Load16(alloc, is, 2, n);
	vertices_n = n;
	texcoords = NodeArray::Load16(alloc, is, 2, n);
	texcoords_n = n;
	triangle = NodeArray::Load16(alloc, is, 1, n);
	triangle_n = n;
}

int TrianglesMesh::Size()
{
	return ALIGN_4BYTE(sizeof(TrianglesMesh) + PTR_SIZE_DIFF * 3);
}

}