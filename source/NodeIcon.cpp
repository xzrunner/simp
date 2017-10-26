#include "simp/NodeIcon.h"
#include "simp/ImportStream.h"
#include "simp/NodeArray.h"

#include <bimp/typedef.h>

namespace simp
{

NodeIcon::NodeIcon(bimp::Allocator& alloc, ImportStream& is)
{
	base_id = is.UInt32();
	type = is.UInt8();

	NodeArray::Load16(alloc, is, 2, vertices_n);
}

int NodeIcon::Size()
{
	return ALIGN_4BYTE(sizeof(NodeIcon) - sizeof(uint16_t));
}

}