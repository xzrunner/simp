#ifndef _SIMP_RELOCATE_TEXCOORDS_H_
#define _SIMP_RELOCATE_TEXCOORDS_H_

#include "simp/NodeVisitor.h"

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

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
		Visitor(int pkg_id, const CU_MAP<int, Item>& items);
		virtual void Visit(int id, int type, const void* node) override;
	private:
		int m_pkg_id;
		const CU_MAP<int, Item>& m_items;
	}; // Visitor

private:
	CU_SET<int> m_pkgs;
	CU_MAP<int, Item> m_items;

	bool m_release_tag;
	CU_SET<int> m_pkg_tag;

	CU_SINGLETON_DECLARATION(RelocateTexcoords);

}; // RelocateTexcoords

}

#endif // _SIMP_RELOCATE_TEXCOORDS_H_