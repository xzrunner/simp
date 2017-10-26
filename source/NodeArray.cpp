#include "simp/NodeArray.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>
#include <bimp/Allocator.h>

namespace simp
{

uint8_t* NodeArray::Load8(bimp::Allocator& alloc, ImportStream& is, int count, uint16_t& array_n)
{
	array_n = is.UInt16();
	uint8_t* array = static_cast<uint8_t*>(alloc.Alloc(ALIGN_4BYTE(sizeof(uint8_t) * count* array_n)));
	for (int i = 0, n = count * array_n; i < n; ++i) {
		array[i] = is.UInt8();
	}
	return array;
}

uint16_t* NodeArray::Load16(bimp::Allocator& alloc, ImportStream& is, int count, uint16_t& array_n)
{
	array_n = is.UInt16();
	uint16_t* array = static_cast<uint16_t*>(alloc.Alloc(ALIGN_4BYTE(sizeof(uint16_t) * count* array_n)));
	for (int i = 0, n = count * array_n; i < n; ++i) {
		array[i] = is.UInt16();
	}
	return array;
}

uint32_t* NodeArray::Load32(bimp::Allocator& alloc, ImportStream& is, int count, uint16_t& array_n)
{
	array_n = is.UInt16();
	uint32_t* array = static_cast<uint32_t*>(alloc.Alloc(ALIGN_4BYTE(sizeof(uint32_t) * count* array_n)));
	for (int i = 0, n = count * array_n; i < n; ++i) {
		array[i] = is.UInt32();
	}
	return array;
}

}