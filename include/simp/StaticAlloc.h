#ifndef _SIMP_STATIC_ALLOC_H_
#define _SIMP_STATIC_ALLOC_H_

#include <CU_Singleton.h>

#include <vector>

namespace bimp { class Allocator; }

namespace simp
{

class StaticAlloc
{
public:
	bimp::Allocator* Create();
	void Release(bimp::Allocator*);

private:
	std::vector<bimp::Allocator*> m_freelist;

	SINGLETON_DECLARATION(StaticAlloc);

}; // StaticAlloc

}

#endif // _SIMP_STATIC_ALLOC_H_
