#include "StaticAlloc.h"
#include "simp_define.h"

#include <bimp/Allocator.h>

namespace simp
{

SINGLETON_DEFINITION(StaticAlloc)

StaticAlloc::StaticAlloc()
{
}

bimp::Allocator* StaticAlloc::Create()
{
	bimp::Allocator* ret = NULL;
	if (!m_freelist.empty()) {
		ret = m_freelist.back();
		m_freelist.pop_back();
	} else {
		return new bimp::Allocator(PAGE_SIZE);
	}
	return ret;
}

void StaticAlloc::Release(bimp::Allocator* alloc)
{
	if (alloc) {
		m_freelist.push_back(alloc);
	}
}

}