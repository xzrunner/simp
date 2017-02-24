#include "NodeShape.h"
#include "ImportStream.h"
#include "NodeArray.h"

#include <bimp/typedef.h>

namespace simp
{

NodeShape::NodeShape(bimp::Allocator& alloc, ImportStream& is)
{
	type	= is.UInt8();
	filling	= 1;	// todo
	color	= is.UInt32();
	uint32_t* vs = NodeArray::Load32(alloc, is, 2, vertices_n);
	for (int i = 0, n = vertices_n * 2; i < n; ++i) {
		vertices[i] = vs[i];
	}
}

int NodeShape::Size()
{
	return ALIGN_4BYTE(sizeof(NodeShape) - sizeof(uint32_t));
}

}