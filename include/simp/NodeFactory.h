#ifndef _SIMP_NODE_FACTORY_H_
#define _SIMP_NODE_FACTORY_H_

#include "simp/Package.h"

#include <cu/uncopyable.h>
#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

#include <stdint.h>

namespace simp
{

class PageVisitor;

class NodeFactory
{
public:
	void Traverse(PageVisitor& visitor) const;

	bool AddPkg(PackagePtr& pkg, const CU_STR& pkg_name, int pkg_id);
	const Package* QueryPkg(int pkg_id) const;
	const Package* QueryPkg(const CU_STR& pkg_name) const;

	const void* Create(uint32_t id, int* type);

	uint32_t GetNodeID(const CU_STR& pkg_name, const CU_STR& node_name) const;

	void Clear();
	void ClearPkgPages(int pkg_id);

private:
	struct PkgWrap
	{
		PkgWrap(PackagePtr& pkg, const CU_STR& pkg_name, int pkg_id)
			: pkg(std::move(pkg)), name(pkg_name), id(pkg_id) {}

		PackagePtr pkg;
		CU_STR name;
		int id;
	};

	template <typename T>
	class Hash : private cu::Uncopyable
	{
	public:
		Hash(int hash_sz);
		~Hash();

		bool Insert(const T& key, int val);
		int  Query(const T& key) const;

		bool Remove(const T& key);

		void Clear();

	protected:
		virtual int GetHashVal(const T& key) const = 0;

		int GetHashSize() const { return m_hash_sz; }

	private:
		int m_hash_sz;

		CU_VEC<std::pair<T, int> >* m_hash;

	}; // Hash

	class HashID : public Hash<uint32_t>
	{
	public:
		HashID(int hash_sz);
	protected:
		virtual int GetHashVal(const uint32_t& id) const override;
	}; // HashID

	struct HashName : public Hash<CU_STR>
	{
	public:
		HashName(int hash_sz);
	protected:
		int GetHashVal(const CU_STR& name) const;
	}; // HashName

private:
	CU_VEC<PkgWrap> m_pkgs;

	HashID   m_hash_id;
	HashName m_hash_name;

	CU_SINGLETON_DECLARATION(NodeFactory);

}; // NodeFactory

}

#include "simp/NodeFactory.inl"

#endif // _SIMP_NODE_FACTORY_H_
