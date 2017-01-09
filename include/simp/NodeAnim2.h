#ifndef _SIMP_NODE_ANIM2_H_
#define _SIMP_NODE_ANIM2_H_

#include <CU_Uncopyable.h>
#include <SM_Vector.h>
#include <rigging.h>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;
class NodeTrans;

class NodeAnim2 : private cu::Uncopyable
{
public:
	struct Srt
	{
		sm::vec2 trans;
		float    rot;
		sm::vec2 scale;
	};

	struct Joint
	{
		uint16_t parent;
		uint16_t padding;
		Srt      local;
	};

	struct Skin
	{
		uint32_t node;
		int      type;
		Srt      local;
	};

	struct Slot
	{
		uint16_t joint;
		uint16_t skin;
	};

	struct JointSample
	{
		uint16_t time;
		uint8_t	 lerp;
		uint8_t  padding;
		float	 data;
	};

	struct TL_Joint
	{
		int type;
		int dims_count[DIM_COUNT];
		int count;
		JointSample samples[1];
	};

	struct SkinSample
	{
		uint16_t time;
		uint16_t skin;
	};

	struct TL_Skin
	{
		int count;
		SkinSample samples[1];
	};

	struct DeformSample
	{
		uint16_t time;
		uint16_t offset;
		int count;
		float vertices[1];
	};

	struct TL_Deform
	{
		int count;
		DeformSample samples[1];
	};

public:
	uint16_t root;	

	uint16_t joint_count;
	uint16_t skin_count;
	uint16_t slot_count;

	uint16_t tl_joint_count;
	uint16_t tl_skin_count;
	uint16_t tl_deform_count;
	uint16_t padding;

	Joint* joints;
	Skin*  skins;
	Slot*  slots;

	TL_Joint*  tl_joints;
	TL_Skin*   tl_skins;
	TL_Deform* tl_deforms;

public:
	NodeAnim2(bimp::Allocator& alloc, ImportStream& is);

	static int Size();

	static int JointSize();
	static int SkinSize();
	static int SlotSize();

	static int TLJointSize();
	static int TLSkinSize();
	static int TLDeformSize();

}; // NodeAnim2

}

#endif // _SIMP_NODE_ANIM2_H_