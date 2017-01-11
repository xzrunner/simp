#include "NetworkMesh.h"
#include "ImportStream.h"
#include "simp_define.h"
#include "NodeArray.h"

#include <bimp/typedef.h>

namespace simp
{

NetworkMesh::NetworkMesh(bimp::Allocator& alloc, ImportStream& is)
{
	uint16_t n;
	outer = NodeArray::Load16(alloc, is, 2, n);
	outer_n = n;
	inner = NodeArray::Load16(alloc, is, 2, n);
	inner_n = 0;
}

int NetworkMesh::Size()
{
	return ALIGN_4BYTE(sizeof(NetworkMesh) + PTR_SIZE_DIFF * 2);
}

}