#include "simp/NodeAnimationSpr.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeAnimationSpr::NodeAnimationSpr(ImportStream& is)
{
	sym = is.UInt32();

	uint8_t pack8 = is.UInt8();
	loop = (pack8 & 0x1) ? 1 : 0;
	start_random = (pack8 & 0x2) ? 1 : 0;

	interval = is.UInt32();
	fps = is.UInt16();
}

int NodeAnimationSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAnimationSpr));
}

}