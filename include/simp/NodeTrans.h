#ifndef _SIMP_NODE_TRANS_H_
#define _SIMP_NODE_TRANS_H_

#include <cu/uncopyable.h>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class NodeTrans : private cu::Uncopyable
{
public:
	NodeTrans();

	static int Size();

	static NodeTrans* LoadTrans(bimp::Allocator& alloc, ImportStream& is);

public:
	// geometry
	static const int SCALE_MASK			= 1 << 1;
	static const int SHEAR_MASK			= 1 << 2;
	static const int OFFSET_MASK		= 1 << 3;
	static const int POSITION_MASK		= 1 << 4;
	static const int ANGLE_MASK			= 1 << 5;

	// color
	static const int COL_MUL_MASK		= 1 << 11;
	static const int COL_ADD_MASK		= 1 << 12;
	static const int COL_R_MASK			= 1 << 13;
	static const int COL_G_MASK			= 1 << 14;
	static const int COL_B_MASK			= 1 << 15;

	// shader
	static const int BLEND_MASK			= 1 << 21;
	static const int FAST_BLEND_MASK	= 1 << 22;
	static const int FILTER_MASK		= 1 << 23;	
	static const int CAMERA_MASK		= 1 << 24;
	static const int DOWNSMAPLE_MASK    = 1 << 25;

	// other
	static const int ACTOR_MASK         = 1 << 28;
	static const int INTEGRATE_MASK     = 1 << 29;

public:
	const char* name;

	uint32_t type;
	
	uint32_t data[1];

}; // NodeTrans

}

#endif // _SIMP_NODE_TRANS_H_