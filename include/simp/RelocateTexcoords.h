#ifndef _SIMP_RELOCATE_TEXCOORDS_H_
#define _SIMP_RELOCATE_TEXCOORDS_H_

#include "NodeVisitor.h"

#include <CU_Singleton.h>

#include <set>
#include <map>

namespace simp
{

class Page;

class RelocateTexcoords
{
public:
	struct Item
	{
		int src_pkg;
		int src_tex;
		int src_lod;

		int dst_tex_id;
		int dst_w, dst_h;
		int dst_xmin, dst_ymin, dst_xmax, dst_ymax;
	};

public:
	void Add(const Item& item);
	void Delete(int pkg);

	void Do(const Page* page);

	void Clear();

	void SetReleaseTag();
	void ReleaseAfterLastTag();

private:
	static int CalcKey(int pkg, int tex);

	void DeletePkg(int pkg);

private:
	class Visitor : public NodeVisitor
	{
	public:
		Visitor(int pkg_id, const std::map<int, Item>& items);
		virtual void Visit(int id, int type, const void* node);
	private:
		int m_pkg_id;
		const std::map<int, Item>& m_items;
	}; // Visitor

private:
	std::set<int> m_pkgs;
	std::map<int, Item> m_items;

	bool m_release_tag;
	std::set<int> m_pkg_tag;

	SINGLETON_DECLARATION(RelocateTexcoords);

}; // RelocateTexcoords

}

#endif // _SIMP_RELOCATE_TEXCOORDS_H_