#include "simp/NodeMaskSpr.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeMaskSpr::NodeMaskSpr(ImportStream& is)
{
	sym = is.UInt32();
}

int NodeMaskSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeMaskSpr));
}

}