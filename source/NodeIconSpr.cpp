#include "NodeIconSpr.h"
#include "ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeIconSpr::NodeIconSpr(ImportStream& is)
{
	sym = is.UInt32();
	process = is.UInt16();
}

int NodeIconSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeIconSpr));
}

}