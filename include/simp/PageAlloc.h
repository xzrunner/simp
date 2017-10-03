#ifndef _SIMP_PAGE_ALLOC_H_
#define _SIMP_PAGE_ALLOC_H_

#include <cu/cu_macro.h>

#include <vector>

namespace bimp { class Allocator; }

namespace simp
{

class PageAlloc
{
public:
	bimp::Allocator* Create(int size);
	bool Release(bimp::Allocator*);

	void Clear();

private:
	class Freelist
	{
	public:
		Freelist(int size);
		~Freelist();

		void Clear();

	private:
		int size;
		std::vector<bimp::Allocator*> freelist;

		friend class PageAlloc;

	}; // Freelist

	static int CalcIndex(int size);

private:
	std::vector<Freelist*> m_freelists;
	
	CU_SINGLETON_DECLARATION(PageAlloc);

}; // PageAlloc

}

#endif // _SIMP_PAGE_ALLOC_H_
