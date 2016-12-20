#include "NodeTexture.h"
#include "ImportStream.h"
#include "simp_define.h"

#include <bimp/typedef.h>
#include <bimp/Allocator.h>

namespace simp
{

NodeTexture::NodeTexture(bimp::Allocator& alloc, ImportStream& is)
{
	n = is.UInt16();
	alloc.Alloc(ALIGN_4BYTE(sizeof(uint32_t) * n));
	for (int i = 0; i < n; ++i) {
		polys[i] = is.UInt32();
	}
}

int NodeTexture::Size()
{
	return sizeof(NodeTexture) - sizeof(uint32_t);
}

}