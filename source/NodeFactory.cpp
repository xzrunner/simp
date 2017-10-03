#include "NodeFactory.h"
#include "SIMP_Package.h"
#include "PageAlloc.h"

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

bool NodeFactory::AddPkg(Package* pkg, const std::string& pkg_name, int pkg_id)
{
	PkgWrap wrap;
	wrap.pkg  = pkg;
	wrap.name = pkg_name;
	wrap.id   = pkg_id;

	int idx = m_pkgs.size();

	if (!m_hash_id.Insert(pkg_id, idx) || !m_hash_name.Insert(pkg_name, idx)) {
		return false;
	}

	m_pkgs.push_back(wrap);
	return true;
}

uint32_t NodeFactory::GetNodeID(const std::string& pkg_name, const std::string& node_name) const
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
	for (int i = 0, n = m_pkgs.size(); i < n; ++i) {
		delete m_pkgs[i].pkg;
	}
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
	: NodeFactory::Hash<std::string>(hash_sz)
{
}

int NodeFactory::HashName::GetHashVal(const std::string& name) const
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