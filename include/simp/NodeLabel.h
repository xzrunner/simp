#ifndef _SIMP_NODE_LABEL_H_
#define _SIMP_NODE_LABEL_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class NodeLabel : private cu::Uncopyable
{
public:
	int			width;
	int			height;

	int			font;
	int			font_size;
	uint32_t	font_color;

	int			edge;
	float		edge_size;
	uint32_t	edge_color;

	int			align_hori;
	int			align_vert;

	float		space_hori;
	float		space_vert;

	int			richtext;
	int			overflow;

	const char* text;
	const char* tid;

public:
	NodeLabel(bimp::Allocator& alloc, ImportStream& is, bool long_text);

	static int Size();

}; // NodeLabel

}

#endif // _SIMP_NODE_LABEL_H_