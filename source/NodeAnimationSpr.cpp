#include "NodeAnimationSpr.h"
#include "ImportStream.h"

#include <bimp/bimp_typedef.h>

namespace simp
{

NodeAnimationSpr::NodeAnimationSpr(ImportStream& is)
{
	sym = is.UInt32();
	loop = is.UInt8();
	interval = is.UInt32();
	fps = is.UInt16();
}

int NodeAnimationSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAnimationSpr));
}

}