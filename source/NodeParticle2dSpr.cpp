#include "simp/NodeParticle2dSpr.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeParticle2dSpr::NodeParticle2dSpr(ImportStream& is)
{
	sym		= is.UInt32();
	loop	= is.UInt8();
	local	= is.UInt8();
}

int NodeParticle2dSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeParticle2dSpr));
}

}