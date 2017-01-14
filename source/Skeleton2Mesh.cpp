#include "Skeleton2Mesh.h"
#include "ImportStream.h"
#include "simp_define.h"
#include "NodeArray.h"

#include <bimp/typedef.h>
#include <bimp/Allocator.h>

namespace simp
{

Skeleton2Mesh::Skeleton2Mesh(bimp::Allocator& alloc, ImportStream& is)
{
	uint16_t n;
	items_n = NodeArray::Load8(alloc, is, 1, n);
	vertices_n = n;

	int count = 0;
	for (int i = 0; i < vertices_n; ++i) {
		count += items_n[i];
	}

	items = (Item*)alloc.Alloc(ItemSize() * count);
	for (int i = 0; i < count; ++i) {
		items[i].joint  = is.UInt16();
		items[i].vx     = (int16_t)(is.UInt16());
		items[i].vy     = (int16_t)(is.UInt16());
		items[i].weight = is.UInt16();
	}

	texcoords = NodeArray::Load16(alloc, is, 2, n);
	texcoords_n = n;
	triangle = NodeArray::Load16(alloc, is, 1, n);
	triangle_n = n;
}

int Skeleton2Mesh::Size()
{
	return ALIGN_4BYTE(sizeof(Skeleton2Mesh) + PTR_SIZE_DIFF * 4);
}

int Skeleton2Mesh::ItemSize()
{
	return ALIGN_4BYTE(sizeof(Item));
}

}