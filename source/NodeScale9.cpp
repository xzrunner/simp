#include "NodeScale9.h"
#include "ImportStream.h"

#include <bimp/Allocator.h>
#include <bimp/bimp_typedef.h>

namespace simp
{

NodeScale9::NodeScale9(bimp::Allocator& alloc, ImportStream& is)
{
	type = is.UInt8();

	int n = 0;
	switch (type) {
	case 1:
		n = 9;
		break;
	case 2: case 3:
		n = 3;
		break;
	case 4:
		n = 6;
		break;
	case 5:
		n = 8;
		break;
	}

	left  = is.UInt16();
	right = is.UInt16();
	top   = is.UInt16();
	down  = is.UInt16();

	alloc.Alloc(GridSize() * n);	

	for (int i = 0; i < n; ++i)
	{
		Grid& g = grids[i];

		g.sym = is.UInt32();

		uint8_t trans = is.UInt8();
		g.dir = (trans & 0xf0) >> 4;
		g.mirror = trans & 0xf;
	}
}

int NodeScale9::Size()
{
	return ALIGN_4BYTE(sizeof(NodeScale9) - sizeof(Grid));
}

int NodeScale9::GridSize()
{
	return ALIGN_4BYTE(sizeof(Grid));
}

}