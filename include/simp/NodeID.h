#ifndef _SIMP_NODE_ID_H_
#define _SIMP_NODE_ID_H_

#include <assert.h>
#include <stdint.h>

namespace simp
{

class NodeID
{
public:	
	static uint32_t ComposeID(uint32_t pkg_id, uint32_t node_id) {
		assert(pkg_id < (1 << PKG_ID_SIZE) && node_id < (1 << NODE_ID_SIZE));
		return (pkg_id << NODE_ID_SIZE) | node_id;
	}

	static uint32_t GetPkgID(uint32_t id)  { return (id & PKG_ID_MASK) >> NODE_ID_SIZE; }
	static uint32_t GetNodeID(uint32_t id) { return id & NODE_ID_MASK; }

private:
	static const int PKG_ID_SIZE		= 12;
	static const int NODE_ID_SIZE		= 20;
	static const uint32_t PKG_ID_MASK	= 0xfff00000;
	static const uint32_t NODE_ID_MASK	= 0x000fffff;

}; // NodeID

}

#endif // _SIMP_NODE_ID_H_