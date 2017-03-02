#include "SIMP_PointsMesh.h"
#include "ImportStream.h"
#include "simp_define.h"
#include "NodeArray.h"

#include <bimp/bimp_typedef.h>

namespace simp
{

PointsMesh::PointsMesh(bimp::Allocator& alloc, ImportStream& is)
{
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