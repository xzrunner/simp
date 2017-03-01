#include "NodeTexture.h"
#include "ImportStream.h"
#include "simp_define.h"
#include "NodeArray.h"

#include <bimp/bimp_typedef.h>

namespace simp
{

NodeTexture::NodeTexture(bimp::Allocator& alloc, ImportStream& is)
{
	uint16_t _n;
	NodeArray::Load32(alloc, is, 1, _n);
	n = _n;
}

int NodeTexture::Size()
{
	return ALIGN_4BYTE(sizeof(NodeTexture) - sizeof(uint32_t));
}

}