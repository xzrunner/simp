#include "NodeTextureSpr.h"
#include "ImportStream.h"

#include <bimp/bimp_typedef.h>

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