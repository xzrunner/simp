#include "NodeParticle3dSpr.h"
#include "ImportStream.h"
#include "from_int.h"

#include <bimp/bimp_typedef.h>

namespace simp
{

NodeParticle3dSpr::NodeParticle3dSpr(ImportStream& is)
{
	sym		= is.UInt32();

	uint8_t pack8 = is.UInt8();
	loop  = (pack8 & 0x1) ? 1 : 0;
	local = (pack8 & 0x2) ? 1 : 0;
	alone = (pack8 & 0x4) ? 1 : 0;
	reuse = (pack8 >> 3) & 0xff;

	radius = int2float(is.UInt16(), 16);
}

int NodeParticle3dSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeParticle3dSpr));
}

}