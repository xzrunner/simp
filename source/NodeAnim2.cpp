#include "NodeAnim2.h"

#include <bimp/typedef.h>

namespace simp
{

NodeAnim2::NodeAnim2(bimp::Allocator& alloc, ImportStream& is)
{
	
}

int NodeAnim2::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAnim2) + PTR_SIZE_DIFF * 3);
}

int NodeAnim2::JointSize()
{
	return ALIGN_4BYTE(sizeof(Joint));
}

int NodeAnim2::SkinSize()
{
	return ALIGN_4BYTE(sizeof(Skin));
}

int NodeAnim2::SlotSize()
{
	return ALIGN_4BYTE(sizeof(Slot));
}

int NodeAnim2::TLJointSize()
{
	return ALIGN_4BYTE(sizeof(TL_Joint) - sizeof(JointSample));
}

int NodeAnim2::TLSkinSize()
{
	return ALIGN_4BYTE(sizeof(TL_Skin) - sizeof(SkinSample));
}

int NodeAnim2::TLDeformSize()
{
	return ALIGN_4BYTE(sizeof(DeformSample) - sizeof(float));
}

}