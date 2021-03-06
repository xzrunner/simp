#include "simp/PageAlloc.h"
#include "simp/simp_define.h"

#include <bimp/Allocator.h>

namespace simp
{

CU_SINGLETON_DEFINITION(PageAlloc)

PageAlloc::PageAlloc()
{
	int size = SIMP_PAGE_SIZE_MIN;
	while (size <= SIMP_PAGE_SIZE_MAX) {
		m_freelists.push_back(new Freelist(size));
		size *= 2;
	}
}

bimp::Allocator* PageAlloc::Create(int size)
{
	int idx = CalcIndex(size);
	if (idx == -1) {
		return nullptr;
	}

	bimp::Allocator* ret = nullptr;
	if (!m_freelists[idx]->freelist.empty()) {
		ret = m_freelists[idx]->freelist.back();
		ret->Reset();
		m_freelists[idx]->freelist.pop_back();
	} else {
		ret = new bimp::Allocator(size);
	}
	return ret;
}

bool PageAlloc::Release(bimp::Allocator* alloc)
{
	if (!alloc) {
		return false;
	}

	int sz = alloc->GetSize();
	int idx = CalcIndex(sz);
	if (idx == -1) {
		return false;
	}
	m_freelists[idx]->freelist.push_back(alloc);
	return true;
}

void PageAlloc::Clear()
{
	for (int i = 0, n = m_freelists.size(); i < n; ++i) {
		m_freelists[i]->Clear();
	}
}

int PageAlloc::CalcIndex(int size)
{
	int idx = 0;
	while (SIMP_PAGE_SIZE_MIN << idx != size) {
		idx++;
		if (SIMP_PAGE_SIZE_MIN << idx > SIMP_PAGE_SIZE_MAX) {
			return -1;
		}
	}
	return idx;
}

/************************************************************************/
/* class PageAlloc::Freelist                                            */
/************************************************************************/

PageAlloc::Freelist::
Freelist(int size) 
	: size(size) 
{
}

PageAlloc::Freelist::
~Freelist()
{
	Clear();
}

void PageAlloc::Freelist::
Clear()
{
	for (int i = 0, n = freelist.size(); i < n; ++i) {
		delete freelist[i];
	}
	freelist.clear();
}

}