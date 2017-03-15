#ifndef _SIMP_PAGE_VISITOR_H_
#define _SIMP_PAGE_VISITOR_H_

namespace simp
{

class Page;

class PageVisitor
{
public:
	virtual ~PageVisitor() {}
	virtual void Visit(const Page* page) = 0;

}; // PageVisitor

}

#endif // _SIMP_PAGE_VISITOR_H_