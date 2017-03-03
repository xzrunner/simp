#ifndef _SIMP_NODE_ANIMATION_H_
#define _SIMP_NODE_ANIMATION_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;
class NodeTrans;

class NodeAnimation : private cu::Uncopyable
{
public:
	struct Actor
	{
		NodeTrans* trans;
		uint32_t sym_id;
		uint32_t padding;
	};

	struct Frame
	{
		Actor** actors;
		uint16_t n;
		uint16_t index;
		uint8_t tween;
		uint8_t padding[3];
	};

	struct Layer
	{
		Frame** frames;
		int n;
		uint32_t padding;
	};

	uint16_t n;
	uint16_t padding[3];
	Layer layers[1];

public:
	NodeAnimation(bimp::Allocator& alloc, ImportStream& is);

	static int Size();
	static int ActorSize();
	static int FrameSize();
	static int LayerSize();

}; // NodeAnimation

}

#endif // _SIMP_NODE_ANIMATION_H_