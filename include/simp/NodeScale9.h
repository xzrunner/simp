#ifndef _SIMP_NODE_SCALE9_H_
#define _SIMP_NODE_SCALE9_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class NodeScale9 : private cu::Uncopyable
{
public:
	struct Grid
	{
		uint32_t sym;
		uint16_t dir;
		uint16_t mirror;
	};

	uint32_t type;
	Grid grids[1];

public:
	NodeScale9(bimp::Allocator& alloc, ImportStream& is);
	
	static int Size();
	static int GridSize();

}; // NodeScale9

}

#endif // _SIMP_NODE_SCALE9_H_	