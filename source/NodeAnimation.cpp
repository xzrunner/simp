#include <string.h>

#include "simp/NodeAnimation.h"
#include "simp/simp_define.h"
#include "simp/ImportStream.h"
#include "simp/NodeTrans.h"
#include "simp/from_int.h"

#include <bimp/typedef.h>
#include <bimp/Allocator.h>

#include <string.h>

namespace simp
{

NodeAnimation::NodeAnimation(bimp::Allocator& alloc, ImportStream& is)
{
	Init(alloc, is);
}

int NodeAnimation::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAnimation) - sizeof(Layer));
}

int NodeAnimation::ActorSize()
{
	return ALIGN_4BYTE(sizeof(Actor) + PTR_SIZE_DIFF);
}

int NodeAnimation::LerpSize()
{
	return ALIGN_4BYTE(sizeof(Lerp) - sizeof(uint32_t));	
}

int NodeAnimation::FrameSize()
{
	return ALIGN_4BYTE(sizeof(Frame) + PTR_SIZE_DIFF * 2);
}

int NodeAnimation::LayerSize()
{
	return ALIGN_4BYTE(sizeof(Layer) + PTR_SIZE_DIFF);
}

void NodeAnimation::Init(bimp::Allocator& alloc, ImportStream& is)
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
			frame->index = (int16_t)(is.UInt16());
			frame->tween = int2bool(is.UInt8());
			InitActors(frame, alloc, is);
			InitLerps(frame, alloc, is);
			layer->frames[j] = frame;
		}
	}
}

void NodeAnimation::InitActors(Frame* frame, bimp::Allocator& alloc, ImportStream& is)
{
	frame->actors_n = is.UInt16();
	frame->actors = static_cast<Actor**>(alloc.Alloc(SIZEOF_POINTER * frame->actors_n));
	memset(frame->actors, 0, SIZEOF_POINTER * frame->actors_n);
	for (int i = 0; i < frame->actors_n; ++i) 
	{
		void* ptr = alloc.Alloc(ActorSize());
		Actor* actor = new (ptr) Actor();
		actor->sym_id = is.UInt32();
		actor->trans = NodeTrans::LoadTrans(alloc, is);
		frame->actors[i] = actor;
	}
}

void NodeAnimation::InitLerps(Frame* frame, bimp::Allocator& alloc, ImportStream& is)
{
	frame->lerps_n = is.UInt16();
	frame->lerps = static_cast<Lerp**>(alloc.Alloc(SIZEOF_POINTER * frame->lerps_n));
	for (int i = 0; i < frame->lerps_n; ++i)
	{
		void* ptr = alloc.Alloc(LerpSize());
		Lerp* lerp = new (ptr) Lerp();
		lerp->spr_data = is.UInt16();
		lerp->type = is.UInt16();
		lerp->data_n = is.UInt16();
		alloc.Alloc(sizeof(uint32_t) * lerp->data_n);
		for (int j = 0; j < lerp->data_n; ++j) {
			lerp->data[j] = is.UInt32();
		}
		frame->lerps[i] = lerp;
	}
}

}