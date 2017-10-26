#include "simp/NodeAnim2Spr.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeAnim2Spr::NodeAnim2Spr(ImportStream& is)
{
	sym = is.UInt32();
	static_time = is.UInt32();
}

int NodeAnim2Spr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAnim2Spr));
}

}