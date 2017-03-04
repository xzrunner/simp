#include "NodeMeshSpr.h"
#include "ImportStream.h"
#include "NodeArray.h"

#include <bimp/bimp_typedef.h>

#include <string.h>

namespace simp
{

NodeMeshSpr::NodeMeshSpr(bimp::Allocator& alloc, ImportStream& is)
{
	mesh_id = is.UInt32();
	base_id	= is.UInt32();

	uint16_t num;
	uint16_t* array = NodeArray::Load16(alloc, is, 3, num);
	n = num;
	memcpy(trans, array, sizeof(uint16_t) * 3 * num);
}

int NodeMeshSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeMeshSpr) - sizeof(uint16_t));
}

}