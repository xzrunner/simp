#include "NodeTrailSpr.h"
#include "ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeTrailSpr::NodeTrailSpr(ImportStream& is)
{
	sym = is.UInt32();
}

int NodeTrailSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeTrailSpr));
}

}