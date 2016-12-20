#ifndef _SIMP_NODE_VISITOR_H_
#define _SIMP_NODE_VISITOR_H_

namespace simp
{

class NodeVisitor
{
public:
	virtual ~NodeVisitor() {}
	virtual void Visit(int id, int type, const void* node) = 0;

}; // NodeVisitor

}

#endif // _SIMP_NODE_VISITOR_H_