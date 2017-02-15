#include "NodeMeshSpr.h"
#include "ImportStream.h"
#include "NodeArray.h"

#include <bimp/typedef.h>

namespace simp
{

NodeMeshSpr::NodeMeshSpr(bimp::Allocator& alloc, ImportStream& is)
{
	mesh_id = is.UInt32();
	base_id	= is.UInt32();
	
	uint16_t num;
	NodeArray::Load16(alloc, is, 3, num);
	n = num;
}

int NodeMeshSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeMeshSpr) - sizeof(uint16_t));
}

}