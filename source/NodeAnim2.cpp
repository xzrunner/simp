#include "NodeAnim2.h"

namespace simp
{

NodeAnim2::NodeAnim2(bimp::Allocator& alloc, ImportStream& is)
{
	
}

int NodeAnim2::Size()
{
	return sizeof(NodeAnim2) + PTR_SIZE_DIFF * 3;
}

int NodeAnim2::JointSize()
{
	return sizeof(Joint);
}

int NodeAnim2::SkinSize()
{
	return sizeof(Skin);
}

int NodeAnim2::SlotSize()
{
	return sizeof(Slot);
}

int NodeAnim2::TLJointSize()
{
	return sizeof(TL_Joint) - sizeof(JointSample);
}

int NodeAnim2::TLSkinSize()
{
	return sizeof(TL_Skin) - sizeof(SkinSample);
}

int NodeAnim2::TLDeformSize()
{
	return sizeof(DeformSample) - sizeof(float);
}

}