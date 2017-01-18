#ifndef _SIMP_PAGE_ALLOC_H_
#define _SIMP_PAGE_ALLOC_H_

#include <CU_Singleton.h>

#include <vector>

namespace bimp { class Allocator; }

namespace simp
{

class PageAlloc
{
public:
	bimp::Allocator* Create(int size);
	bool Release(bimp::Allocator*);

private:
	struct Freelist
	{
		int size;
		std::vector<bimp::Allocator*> freelist;

		Freelist(int size) : size(size) {}
	};

	static int CalcIndex(int size);

private:
	std::vector<Freelist*> m_freelists;
	
	SINGLETON_DECLARATION(PageAlloc);

}; // PageAlloc

}

#endif // _SIMP_PAGE_ALLOC_H_
