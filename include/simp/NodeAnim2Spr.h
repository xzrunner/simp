#ifndef _SIMP_NODE_ANIM2_SPR_H_
#define _SIMP_NODE_ANIM2_SPR_H_

#include <cu/uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeAnim2Spr : private cu::Uncopyable
{
public:
	uint32_t sym;
	uint32_t static_time;

public:
	NodeAnim2Spr(ImportStream& is);

	static int Size();

}; // NodeAnim2Spr

}

#endif // _SIMP_NODE_ANIM2_SPR_H_