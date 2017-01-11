#include "NodeShapeSpr.h"
#include "ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeShapeSpr::NodeShapeSpr(ImportStream& is)
{
	sym = is.UInt32();
}

int NodeShapeSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeShapeSpr));
}

}