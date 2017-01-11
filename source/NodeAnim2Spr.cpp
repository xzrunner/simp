#include "NodeAnim2Spr.h"
#include "ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeAnim2Spr::NodeAnim2Spr(ImportStream& is)
{
	sym = is.UInt32();
	static_time = is.UInt16();
}

int NodeAnim2Spr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAnim2Spr));
}

}