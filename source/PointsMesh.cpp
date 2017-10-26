#include "simp/PointsMesh.h"
#include "simp/ImportStream.h"
#include "simp/simp_define.h"
#include "simp/NodeArray.h"

#include <bimp/typedef.h>

namespace simp
{

PointsMesh::PointsMesh(bimp::Allocator& alloc, ImportStream& is)
{
	type = MESH_POINTS;

	uint16_t n;
	outline = NodeArray::Load16(alloc, is, 2, n);
	outline_n = n;
	points = NodeArray::Load16(alloc, is, 2, n);
	points_n = n;
}

int PointsMesh::Size()
{
	return ALIGN_4BYTE(sizeof(PointsMesh) + PTR_SIZE_DIFF * 2);
}

}