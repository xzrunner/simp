#ifndef _SIMP_NODE_ANIMATION_H_
#define _SIMP_NODE_ANIMATION_H_

#include <cu/uncopyable.h>

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

	struct Lerp
	{
		uint16_t spr_data;
		uint16_t type;
		uint16_t data_n;
		uint16_t padding;
		uint32_t data[1];
	};

	struct Frame
	{
		Actor**  actors;
		Lerp**   lerps;
		uint16_t actors_n;
		uint16_t lerps_n;
		int16_t  index;
		uint8_t  tween;
		uint8_t  padding[1];
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
	static int LerpSize();
	static int FrameSize();
	static int LayerSize();

private:
	void Init(bimp::Allocator& alloc, ImportStream& is);
	void InitActors(Frame* frame, bimp::Allocator& alloc, ImportStream& is);
	void InitLerps(Frame* frame, bimp::Allocator& alloc, ImportStream& is);

}; // NodeAnimation

}

#endif // _SIMP_NODE_ANIMATION_H_