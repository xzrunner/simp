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
	NodeArray::Load16(alloc, is, 2, vertices_n);
}

int NodeShape::Size()
{
	return ALIGN_4BYTE(sizeof(NodeShape) - sizeof(uint16_t));
}

}