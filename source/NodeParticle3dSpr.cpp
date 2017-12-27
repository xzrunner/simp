#include "simp/NodeParticle3dSpr.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>
#include <bs/FixedPointNum.h>

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

	radius = bs::int2float(is.UInt16(), 16);
}

int NodeParticle3dSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeParticle3dSpr));
}

}