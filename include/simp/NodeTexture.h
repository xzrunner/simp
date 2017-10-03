#ifndef _SIMP_NODE_TEXTURE_H_
#define _SIMP_NODE_TEXTURE_H_

#include <cu/uncopyable.h>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class NodeTexture : private cu::Uncopyable
{
public:
	uint32_t n;
	uint32_t polys[1];

public:
	NodeTexture(bimp::Allocator& alloc, ImportStream& is);

	static int Size();

}; // NodeTexture

}

#endif // _SIMP_NODE_TEXTURE_H_