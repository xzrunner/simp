#include "NodeComplex.h"
#include "ImportStream.h"
#include "simp_define.h"
#include "Allocator.h"
#include "NodeTrans.h"

namespace simp
{

NodeComplex::NodeComplex(Allocator& alloc, ImportStream& is)
{
	// scissor
	for (int i = 0; i < 4; ++i) {
		uint16_t s = is.UInt16();
		scissor[i] = s;
	}

	// actions
	actions_n = is.UInt16();
	alloc.Alloc(ActionSize() * actions_n);
	for (int i = 0; i < actions_n; ++i) 
	{
		Action* dst = &actions[i];
		dst->name = is.String(alloc);
		dst->n = is.UInt16();
		dst->idx = (uint16_t*)(alloc.Alloc(sizeof(uint16_t) * dst->n));
		for (int j = 0; j < dst->n; ++j) {
			dst->idx[j] = is.UInt16();
		}
	}

	// sprs
	sprs_n = is.UInt16();
	sprs = (uint32_t*)(alloc.Alloc(sizeof(uint32_t) * sprs_n));
	trans = static_cast<NodeTrans**>(alloc.Alloc(SIZEOF_POINTER * sprs_n));
	memset(trans, 0, SIZEOF_POINTER * sprs_n);
	for (int i = 0; i < sprs_n; ++i) {
		sprs[i] = is.UInt32();
		trans[i] = NodeTrans::LoadTrans(alloc, is);
	}
}

int NodeComplex::Size()
{
	return sizeof(NodeComplex) + PTR_SIZE_DIFF * 2 - sizeof(Action);
}

int NodeComplex::ActionSize()
{
	return sizeof(Action) + PTR_SIZE_DIFF * 2;
}

}