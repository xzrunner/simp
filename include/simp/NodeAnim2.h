#ifndef _SIMP_NODE_ANIM2_H_
#define _SIMP_NODE_ANIM2_H_

#include <CU_Uncopyable.h>
#include <SM_Vector.h>

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

		void Load(ImportStream& is);
	};

	struct Joint
	{
		uint16_t parent;
		uint16_t chlidren_count;
		Srt      local;

		void Load(ImportStream& is);

		static int Size();
	};

	struct IK
	{
		uint16_t joints[2];
		uint16_t target;
		uint16_t bend_positive;
		float	 length[2];

		void Load(ImportStream& is);

		static int Size();
	};

	struct Skin
	{
		uint32_t node;
		int      type;
		Srt      local;

		void Load(ImportStream& is);

		static int Size();
	};

	struct Slot
	{
		uint16_t joint;
		uint16_t skin;

		void Load(ImportStream& is);

		static int Size();
	};

	struct JointSample
	{
		uint16_t time;
		uint8_t	 lerp;
		uint8_t  padding;
		float	 data;

		static int Size();
	};

	static const int DIM_COUNT = 7;
	struct TL_Joint
	{
		uint8_t type;
		uint8_t dims_count[DIM_COUNT];
		JointSample samples[1];

		void Load(bimp::Allocator& alloc, ImportStream& is);

		static int Size();
	};

	struct SkinSample
	{
		uint16_t time;
		uint16_t skin;

		static int Size();
	};

	struct TL_Skin
	{
		int count;
		SkinSample samples[1];

		void Load(bimp::Allocator& alloc, ImportStream& is);

		static int Size();
	};

	struct DeformSample
	{
		uint16_t time;
		uint16_t offset;
		int count;
		float vertices[1];

		static int Size();
	};

	struct TL_Deform
	{
		int count;
		uint32_t padding;
		DeformSample** samples;

		void Load(bimp::Allocator& alloc, ImportStream& is);

		static int Size();
	};

public:
	uint16_t root;	

	uint16_t joint_count;
	uint16_t ik_count;
	uint16_t skin_count;
	uint16_t slot_count;
	uint16_t padding[3];

	Joint* joints;
	IK*    iks;
	Skin*  skins;
	Slot*  slots;

	TL_Joint**  tl_joints;
	TL_Skin**   tl_skins;
	TL_Deform** tl_deforms;

public:
	NodeAnim2(bimp::Allocator& alloc, ImportStream& is);

	static int Size();

}; // NodeAnim2

}

#endif // _SIMP_NODE_ANIM2_H_