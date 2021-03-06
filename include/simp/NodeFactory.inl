#ifndef _SIMP_NODE_FACTORY_INL_
#define _SIMP_NODE_FACTORY_INL_

#include "simp/Package.h"
#include "simp/NodeID.h"
#include "simp/simp_types.h"
#include "simp/simp_define.h"

#include <assert.h>

namespace simp
{

inline
const Package* NodeFactory::QueryPkg(int pkg_id) const
{
	int idx = m_hash_id.Query(pkg_id);
	if (idx >= 0 && idx < static_cast<int>(m_pkgs.size())) {
		return m_pkgs[idx].pkg.get();
	} else {
		return nullptr;
	}
}

inline
const Package* NodeFactory::QueryPkg(const CU_STR& pkg_name) const
{
	int idx = m_hash_name.Query(pkg_name);
	if (idx >= 0 && idx < static_cast<int>(m_pkgs.size())) {
		return m_pkgs[idx].pkg.get();
	} else {
		return nullptr;
	}
}

inline
const void* NodeFactory::Create(uint32_t id, int* type)
{
	uint32_t pkg_id = NodeID::GetPkgID(id);
	if (pkg_id == AUDIO_ID) {
		*type = TYPE_AUDIO;
		return nullptr;
	}

	uint32_t node_id = NodeID::GetNodeID(id);
	int idx = m_hash_id.Query(pkg_id);
	if (idx >= 0 && idx < static_cast<int>(m_pkgs.size())) {
		return m_pkgs[idx].pkg->QueryNode(node_id, type);
	} else {
		return nullptr;
	}
}

/************************************************************************/
/* class NodeFactory::Hash                                              */
/************************************************************************/

template <typename T>
NodeFactory::Hash<T>::Hash(int hash_sz)
	: m_hash_sz(hash_sz)
{
	m_hash = new CU_VEC<std::pair<T, int>>[m_hash_sz];
}

template <typename T>
NodeFactory::Hash<T>::~Hash()
{
	delete[] m_hash;
}

template <typename T>
bool NodeFactory::Hash<T>::Insert(const T& key, int val)
{
	int idx = GetHashVal(key);
	for (int i = 0, n = m_hash[idx].size(); i < n; ++i) {
		if (m_hash[idx][i].first == key) {
			return false;
		}
	}
	m_hash[idx].push_back(std::make_pair(key, val));
	return true;
}

template <typename T>
int NodeFactory::Hash<T>::Query(const T& key) const
{
	int idx = GetHashVal(key);
	for (int i = 0, n = m_hash[idx].size(); i < n; ++i) {
		if (m_hash[idx][i].first == key) {
			return m_hash[idx][i].second;
		}
	}
	return -1;
}

template <typename T>
bool NodeFactory::Hash<T>::Remove(const T& key)
{
	int idx = GetHashVal(key);
	typename CU_VEC<std::pair<T, int> >::iterator itr = m_hash[idx].begin();
	for ( ; itr != m_hash[idx].end(); ++itr) {
		if (itr->first == key) {
			m_hash[idx].erase(itr);
			return true;
		}
	}
	return false;
}

template <typename T>
void NodeFactory::Hash<T>::Clear()
{
	for (int i = 0; i < m_hash_sz; ++i) {
		m_hash[i].clear();
	}
}

}

#endif // _SIMP_NODE_FACTORY_INL_
