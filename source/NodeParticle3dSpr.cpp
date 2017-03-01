#include "NodeParticle3dSpr.h"
#include "ImportStream.h"

#include <bimp/bimp_typedef.h>

namespace simp
{

NodeParticle3dSpr::NodeParticle3dSpr(ImportStream& is)
{
	sym		= is.UInt32();
	loop	= is.UInt8();
	local	= is.UInt8();
	alone	= is.UInt8();
	reuse	= is.UInt8();
}

int NodeParticle3dSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeParticle3dSpr));
}

}