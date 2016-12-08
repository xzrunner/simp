#ifndef _SIMP_NODE_FACTORY_H_
#define _SIMP_NODE_FACTORY_H_

#include <CU_Uncopyable.h>
#include <CU_Singleton.h>

#include <string>
#include <vector>

#include <stdint.h>

namespace simp
{

class Package;

class NodeFactory : private cu::Uncopyable
{
public:
	bool AddPkg(Package* pkg, const std::string& pkg_name, int pkg_id);
	const Package* QueryPkg(int node_id) const;
	const Package* QueryPkg(const std::string& pkg_name) const;

	const void* Create(uint32_t id, int* type);

	uint32_t GetNodeID(const std::string& pkg_name, const std::string& node_name) const;

private:
	static const int PKG_ID_SIZE		= 12;
	static const int NODE_ID_SIZE		= 20;
	static const uint32_t PKG_ID_MASK	= 0xfff00000;
	static const uint32_t NODE_ID_MASK	= 0x000fffff;

	uint32_t GetPkgID(uint32_t id) const {
		return (id & PKG_ID_MASK) >> NODE_ID_SIZE;
	}
	uint32_t GetNodeID(uint32_t id) const {
		return id & NODE_ID_MASK;
	}

private:
	struct PkgWrap
	{
		Package* pkg;
		std::string name;
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

	protected:
		virtual int GetHashVal(const T& key) const = 0;

		int GetHashSize() const { return m_hash_sz; }

	private:
		int m_hash_sz;

		std::vector<std::pair<T, int> >* m_hash;

	}; // Hash

	class HashID : public Hash<uint32_t>
	{
	public:
		HashID(int hash_sz);
	protected:
		virtual int GetHashVal(const uint32_t& id) const;
	}; // HashID

	struct HashName : public Hash<std::string>
	{
	public:
		HashName(int hash_sz);
	protected:
		int GetHashVal(const std::string& name) const;
	}; // HashName

private:
	std::vector<PkgWrap> m_pkgs;

	HashID   m_hash_id;
	HashName m_hash_name;

	SINGLETON_DECLARATION(NodeFactory);

}; // NodeFactory

}

#include "NodeFactory.inl"

#endif // _SIMP_NODE_FACTORY_H_
