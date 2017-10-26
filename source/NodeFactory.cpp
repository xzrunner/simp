#include "simp/NodeFactory.h"
#include "simp/Package.h"
#include "simp/PageAlloc.h"

#include <assert.h>

namespace simp
{

CU_SINGLETON_DEFINITION(NodeFactory);

NodeFactory::NodeFactory()
	: m_hash_id(197)
	, m_hash_name(197)
{
}

void NodeFactory::Traverse(PageVisitor& visitor) const
{
	for (int i = 0, n = m_pkgs.size(); i < n; ++i) {
		if (m_pkgs[i].pkg) {
			m_pkgs[i].pkg->Traverse(visitor);
		}
	}
}

bool NodeFactory::AddPkg(PackagePtr& pkg, const CU_STR& pkg_name, int pkg_id)
{
	int idx = m_pkgs.size();
	if (!m_hash_id.Insert(pkg_id, idx) || !m_hash_name.Insert(pkg_name, idx)) {
		return false;
	}
	m_pkgs.emplace_back(pkg, pkg_name, pkg_id);
	return true;
}

uint32_t NodeFactory::GetNodeID(const CU_STR& pkg_name, const CU_STR& node_name) const
{
	int idx = m_hash_name.Query(pkg_name);
	if (idx < 0 || idx >= static_cast<int>(m_pkgs.size())) {
		return 0xffffffff;
	}

	assert(m_pkgs[idx].name == pkg_name);

	uint32_t node_id = m_pkgs[idx].pkg->QueryID(node_name);
	if (node_id == 0xffffffff) {
		return 0xffffffff;
	}

	uint32_t pkg_id = m_pkgs[idx].id;
	return NodeID::ComposeID(pkg_id, NodeID::GetNodeID(node_id));
}

void NodeFactory::Clear()
{
	m_pkgs.clear();	

	m_hash_id.Clear();
	m_hash_name.Clear();
}

void NodeFactory::ClearPkgPages(int pkg_id)
{
	int idx = m_hash_id.Query(pkg_id);
	if (idx >= 0 && idx < static_cast<int>(m_pkgs.size())) {
		m_pkgs[idx].pkg->ClearPages();
	}
}

/************************************************************************/
/* class NodeFactory::HashID                                            */
/************************************************************************/

NodeFactory::HashID::HashID(int hash_sz)
	: NodeFactory::Hash<uint32_t>(hash_sz)
{
}

int NodeFactory::HashID::GetHashVal(const uint32_t& id) const
{
	return id % GetHashSize();
}

/************************************************************************/
/* class NodeFactory::HashName                                          */
/************************************************************************/

NodeFactory::HashName::HashName(int hash_sz)
	: NodeFactory::Hash<CU_STR>(hash_sz)
{
}

int NodeFactory::HashName::GetHashVal(const CU_STR& name) const
{
	const char* str = name.c_str();

	// BKDR Hash Function
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;

	while (*str) {
		hash = hash * seed + (*str++);
	}

	return (hash & 0x7FFFFFFF) % GetHashSize();
}

}