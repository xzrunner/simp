#include "simp/NodeComplexSpr.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeComplexSpr::NodeComplexSpr(ImportStream& is)
{
	sym = is.UInt32();
	action = is.UInt16();
}

int NodeComplexSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeComplexSpr));
}

}