#include "simp/NodeTexture.h"
#include "simp/ImportStream.h"
#include "simp/simp_define.h"
#include "simp/NodeArray.h"

#include <bimp/typedef.h>

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