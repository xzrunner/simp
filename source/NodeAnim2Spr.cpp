#include "NodeAnim2Spr.h"
#include "ImportStream.h"

namespace simp
{

NodeAnim2Spr::NodeAnim2Spr(ImportStream& is)
{
	sym = is.UInt32();
	static_time = is.UInt16();
}

int NodeAnim2Spr::Size()
{
	return sizeof(NodeAnim2Spr);
}

}