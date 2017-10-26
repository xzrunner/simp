#include "simp/NodeMask.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeMask::NodeMask(ImportStream& is)
{
	base_id = is.UInt32();
	mask_id = is.UInt32();
}

int NodeMask::Size()
{
	return ALIGN_4BYTE(sizeof(NodeMask));
}

}