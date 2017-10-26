#include "simp/NodeScale9Spr.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeScale9Spr::NodeScale9Spr(ImportStream& is)
{
	sym = is.UInt32();
	width = is.UInt16();
	height = is.UInt16();
}

int NodeScale9Spr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeScale9Spr));
}

}