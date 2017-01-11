#ifndef _SIMP_NODE_ARRAY_H_
#define _SIMP_NODE_ARRAY_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class NodeArray
{
public:
	static uint8_t*  Load8(bimp::Allocator& alloc, ImportStream& is, int count, uint16_t& array_n);
	static uint16_t* Load16(bimp::Allocator& alloc, ImportStream& is, int count, uint16_t& array_n);
	static uint32_t* Load32(bimp::Allocator& alloc, ImportStream& is, int count, uint16_t& array_n);	

}; // NodeArray

}

#endif // _SIMP_NODE_ARRAY_H_