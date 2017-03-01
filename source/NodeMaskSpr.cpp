#include "NodeMaskSpr.h"
#include "ImportStream.h"

#include <bimp/bimp_typedef.h>

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