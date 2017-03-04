#include <string.h>

#include "NodeAnimation.h"
#include "simp_define.h"
#include "ImportStream.h"
#include "NodeTrans.h"
#include "from_int.h"

#include <bimp/bimp_typedef.h>
#include <bimp/Allocator.h>

#include <string.h>

namespace simp
{

NodeAnimation::NodeAnimation(bimp::Allocator& alloc, ImportStream& is)
{
	n = is.UInt16();
	alloc.Alloc(LayerSize() * n);
	for (int i = 0; i < n; ++i) 
	{
		Layer* layer = &layers[i];
		layer->n = is.UInt16();
		layer->frames = static_cast<Frame**>(alloc.Alloc(SIZEOF_POINTER * layer->n));
		memset(layer->frames, 0, SIZEOF_POINTER * layer->n);
 		for (int j = 0; j < layer->n; ++j) 
		{
			void* ptr = alloc.Alloc(FrameSize());
			Frame* frame = new (ptr) Frame();
			frame->index = is.UInt16();
			frame->tween = int2bool(is.UInt8());
			frame->n = is.UInt16();
			frame->actors = static_cast<Actor**>(alloc.Alloc(SIZEOF_POINTER * frame->n));
			memset(frame->actors, 0, SIZEOF_POINTER * frame->n);
			for (int k = 0; k < frame->n; ++k) 
			{
				void* ptr = alloc.Alloc(ActorSize());
				Actor* actor = new (ptr) Actor();
				actor->sym_id = is.UInt32();
				actor->trans = NodeTrans::LoadTrans(alloc, is);
				frame->actors[k] = actor;
			}
			layer->frames[j] = frame;
		}
	}
}

int NodeAnimation::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAnimation) - sizeof(Layer));
}

int NodeAnimation::ActorSize()
{
	return ALIGN_4BYTE(sizeof(Actor) + PTR_SIZE_DIFF);
}

int NodeAnimation::FrameSize()
{
	return ALIGN_4BYTE(sizeof(Frame) + PTR_SIZE_DIFF);
}

int NodeAnimation::LayerSize()
{
	return ALIGN_4BYTE(sizeof(Layer) + PTR_SIZE_DIFF);
}

}