#ifndef _SIMP_NODE_ICON_H_
#define _SIMP_NODE_ICON_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class NodeIcon : private cu::Uncopyable
{
public:
	uint32_t base_id;

	uint16_t type;

	uint16_t vertices_n;
	uint16_t vertices[1];

public:
	NodeIcon(bimp::Allocator& alloc, ImportStream& is);

	static int Size();

}; // NodeIcon

}

#endif // _SIMP_NODE_ICON_H_