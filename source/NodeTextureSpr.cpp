#include "simp/NodeTextureSpr.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeTextureSpr::NodeTextureSpr(ImportStream& is)
{
	sym = is.UInt32();
}

int NodeTextureSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeTextureSpr));
}

}