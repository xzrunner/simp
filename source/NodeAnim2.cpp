#include "simp/NodeAnim2.h"
#include "simp/ImportStream.h"
#include "simp/simp_define.h"

#include <bimp/typedef.h>
#include <bimp/Allocator.h>

namespace simp
{

NodeAnim2::NodeAnim2(bimp::Allocator& alloc, ImportStream& is)
{
	root = is.UInt16();
	// joints
	joint_count = is.UInt16();
	joints = static_cast<Joint*>(alloc.Alloc(Joint::Size() * joint_count));
	for (int i = 0; i < joint_count; ++i) {
		joints[i].Load(is);
	}

	// iks
	ik_count = is.UInt16();
	iks = static_cast<IK*>(alloc.Alloc(IK::Size() * ik_count));
	for (int i = 0; i < ik_count; ++i) {
		iks[i].Load(is);
	}

	// skins
	skin_count = is.UInt16();
	skins = static_cast<Skin*>(alloc.Alloc(Skin::Size() * skin_count));
	for (int i = 0; i < skin_count; ++i) {
		skins[i].Load(is);
	}

	// slots
	slot_count = is.UInt16();
	slots = static_cast<Slot*>(alloc.Alloc(Slot::Size() * slot_count));
	for (int i = 0; i < slot_count; ++i) {
		slots[i].Load(is);
	}

	// tl_joints
	tl_joints = static_cast<TL_Joint**>(alloc.Alloc(sizeof(TL_Joint*) * joint_count));
	for (int i = 0; i < joint_count; ++i) 
	{
		tl_joints[i] = static_cast<TL_Joint*>(alloc.Alloc(TL_Joint::Size()));
		tl_joints[i]->Load(alloc, is);
	}

	// tl_skins
	tl_skins = static_cast<TL_Skin**>(alloc.Alloc(sizeof(TL_Skin*) * slot_count));
	for (int i = 0; i < slot_count; ++i)
	{
		tl_skins[i] = static_cast<TL_Skin*>(alloc.Alloc(TL_Skin::Size()));
		tl_skins[i]->Load(alloc, is);
	}

	// tl_deforms
	tl_deforms = static_cast<TL_Deform**>(alloc.Alloc(sizeof(TL_Deform*) * skin_count));
	for (int i = 0; i < skin_count; ++i)
	{
		tl_deforms[i] = static_cast<TL_Deform*>(alloc.Alloc(TL_Deform::Size()));
		tl_deforms[i]->Load(alloc, is);
	}

	// curves
	curve_count = is.UInt16();
	curves = static_cast<Curve*>(alloc.Alloc(sizeof(Curve) * curve_count));
	for (int i = 0; i < curve_count; ++i) {
		curves[i].Load(is);
	}
}

int NodeAnim2::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAnim2) + PTR_SIZE_DIFF * 8);
}

/************************************************************************/
/* struct NodeAnim2::Srt                                                */
/************************************************************************/

void NodeAnim2::Srt::
Load(ImportStream& is)
{
	trans.x = is.Float();
	trans.y = is.Float();
	rot     = is.Float();
	scale.x = is.Float();
	scale.y = is.Float();
}

/************************************************************************/
/* struct NodeAnim2::Joint                                              */
/************************************************************************/

void NodeAnim2::Joint::
Load(ImportStream& is)
{
	parent         = is.UInt16();
	chlidren_count = is.UInt16();
	local.Load(is);
}

int NodeAnim2::Joint::Size()
{
	return ALIGN_4BYTE(sizeof(Joint));
}

/************************************************************************/
/* struct NodeAnim2::IK                                                 */
/************************************************************************/

void NodeAnim2::IK::
Load(ImportStream& is)
{
	joints[0] = is.UInt16();
	joints[1] = is.UInt16();

	target = is.UInt16();
	bend_positive = is.UInt16();

	length[0] = is.Float();
	length[1] = is.Float();
}

int NodeAnim2::IK::Size()
{
	return ALIGN_4BYTE(sizeof(IK));
}

/************************************************************************/
/* struct NodeAnim2::Skin                                               */
/************************************************************************/

void NodeAnim2::Skin::
Load(ImportStream& is)
{
	node = is.UInt32();
	type = is.UInt8();
	local.Load(is);
}

int NodeAnim2::Skin::
Size()
{
	return ALIGN_4BYTE(sizeof(Skin));
}

/************************************************************************/
/* struct NodeAnim2::Slot                                               */
/************************************************************************/

void NodeAnim2::Slot::
Load(ImportStream& is)
{
	joint = is.UInt16();
	skin  = is.UInt16();
}

int NodeAnim2::Slot::
Size()
{
	return ALIGN_4BYTE(sizeof(Slot));
}

/************************************************************************/
/* struct NodeAnim2::Curve                                              */
/************************************************************************/

void NodeAnim2::Curve::
Load(ImportStream& is)
{
	x0 = is.Float();
	y0 = is.Float();
	x1 = is.Float();
	y1 = is.Float();
}

int NodeAnim2::Curve::
Size()
{
	return ALIGN_4BYTE(sizeof(Curve));
}

/************************************************************************/
/* struct NodeAnim2::JointSample                                        */
/************************************************************************/

int NodeAnim2::JointSample::
Size()
{
	return sizeof(JointSample);
}

/************************************************************************/
/* struct NodeAnim2::TL_Joint                                           */
/************************************************************************/

void NodeAnim2::TL_Joint::
Load(bimp::Allocator& alloc, ImportStream& is)
{
	type = is.UInt8();
	int count = 0;
	for (int i = 0; i < DIM_COUNT; ++i) {
		dims_count[i] = is.UInt8();
		count += dims_count[i];
	}

	alloc.Alloc(JointSample::Size() * count);
	for (int i = 0; i < count; ++i)
	{
		samples[i].time  = is.UInt16();
		samples[i].lerp  = is.UInt8();
		samples[i].curve = is.UInt8();
		samples[i].data  = is.Float();
	}
}

int NodeAnim2::TL_Joint::
Size()
{
	return ALIGN_4BYTE(sizeof(TL_Joint) - sizeof(JointSample));
}

/************************************************************************/
/* struct NodeAnim2::SkinSample                                         */
/************************************************************************/

int NodeAnim2::SkinSample::
Size()
{
	return sizeof(SkinSample);
}

/************************************************************************/
/* struct NodeAnim2::TL_Skin                                            */
/************************************************************************/

void NodeAnim2::TL_Skin::
Load(bimp::Allocator& alloc, ImportStream& is)
{
	count = is.UInt16();
	alloc.Alloc(SkinSample::Size() * count);
	for (int i = 0; i < count; ++i) {
		samples[i].time = is.UInt16();
		samples[i].skin = is.UInt16();
	}
}

int NodeAnim2::TL_Skin::
Size()
{
	return ALIGN_4BYTE(sizeof(TL_Skin) - sizeof(SkinSample));
}

/************************************************************************/
/* struct NodeAnim2::DeformSample                                       */
/************************************************************************/

int NodeAnim2::DeformSample::
Size()
{
	return ALIGN_4BYTE(sizeof(DeformSample) - sizeof(float));
}

/************************************************************************/
/* struct NodeAnim2::TL_Deform                                          */
/************************************************************************/

void NodeAnim2::TL_Deform::
Load(bimp::Allocator& alloc, ImportStream& is)
{
	count = is.UInt16();
	samples = static_cast<DeformSample**>(alloc.Alloc(sizeof(DeformSample*) * count));
	for (int i = 0; i < count; ++i) 
	{
		uint16_t time   = is.UInt16();
		uint16_t offset = is.UInt16();
		uint8_t  curve  = is.UInt8();
		int count = is.UInt16();
		int sz = DeformSample::Size() + sizeof(float) * count * 2;
		samples[i] = static_cast<DeformSample*>(alloc.Alloc(sz));
		samples[i]->time   = time;
		samples[i]->offset = offset;
		samples[i]->curve  = curve;
		samples[i]->count  = count;
		for (int j = 0, n = count * 2; j < n; ++j) {
			samples[i]->vertices[j] = is.Float();
		}
	}
}

int NodeAnim2::TL_Deform::
Size()
{
	return ALIGN_4BYTE(sizeof(TL_Deform) + PTR_SIZE_DIFF);
}

}