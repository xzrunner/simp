#include "RelocateTexcoords.h"
#include "NodeID.h"
#include "Page.h"
#include "simp_types.h"
#include "NodePicture.h"
#include "NodeFactory.h"

namespace simp
{

SINGLETON_DEFINITION(RelocateTexcoords);

RelocateTexcoords::RelocateTexcoords()
	: m_release_tag(false)
{
}
	
void RelocateTexcoords::Add(const Item& item)
{
	m_pkgs.insert(item.src_pkg);

	int id = CalcKey(item.src_pkg, item.src_tex);
	std::pair<std::map<int, Item>::iterator, bool> ret 
		= m_items.insert(std::make_pair(id, item));
	//	todo
	//	assert(ret.second);
	if (ret.second && m_release_tag) {
		m_pkg_tag.insert(item.src_pkg);
	}
}

void RelocateTexcoords::Delete(int pkg)
{
	DeletePkg(pkg);

	if (m_release_tag) 
	{
		std::set<int>::iterator itr = m_pkg_tag.begin();
		while (itr != m_pkg_tag.end())
		{
			if (NodeID::GetPkgID(*itr) == pkg) {
				m_pkg_tag.erase(itr++);
			} else {
				++itr;
			}
		}
	}
}

void RelocateTexcoords::Do(const Page* page)
{
	if (m_pkgs.find(page->GetPkgID()) == m_pkgs.end()) {
		return;
	}
	if (page->IsTexcoordsRelocated()) {
		return;
	}

	Visitor visitor(page->GetPkgID(), m_items);
	const_cast<Page*>(page)->Traverse(visitor);
	page->SetTexcoordsRelocated(true);
}

void RelocateTexcoords::Clear()
{
	m_pkgs.clear();
	m_items.clear();
}

void RelocateTexcoords::SetReleaseTag()
{
	m_release_tag = true;
}

void RelocateTexcoords::ReleaseAfterLastTag()
{
	std::set<int>::iterator itr = m_pkg_tag.begin();
	for ( ; itr != m_pkg_tag.end(); ++itr) 
	{
		int pkg_id = *itr;
		DeletePkg(pkg_id);
		NodeFactory::Instance()->ClearPkgPages(pkg_id);
	}
	m_pkg_tag.clear();
	m_release_tag = false;
}

int RelocateTexcoords::CalcKey(int pkg, int tex)
{
	return NodeID::ComposeID(pkg, tex);
}

void RelocateTexcoords::DeletePkg(int pkg)
{
	m_pkgs.erase(pkg);

	std::map<int, Item>::iterator itr = m_items.begin();
	while (itr != m_items.end())
	{
		int id = itr->first;
		if (NodeID::GetPkgID(id) == pkg) {
			m_items.erase(itr++);
		} else {
			++itr;
		}
	}
}

/************************************************************************/
/* class RelocateTexcoords::Visitor                                     */
/************************************************************************/

RelocateTexcoords::Visitor::Visitor(int pkg_id, const std::map<int, Item>& items)
	: m_pkg_id(pkg_id)
	, m_items(items)
{
}

void RelocateTexcoords::Visitor::Visit(int id, int type, const void* node)
{
	if (type != TYPE_IMAGE) {
		return;
	}

	const NodePicture* pic = static_cast<const NodePicture*>(node);
	int key = CalcKey(m_pkg_id, pic->texid);
	std::map<int, Item>::const_iterator itr = m_items.find(key);
	if (itr == m_items.end()) {
		return;
	}

	const Item& dst = itr->second;

	NodePicture* new_pic = const_cast<NodePicture*>(pic);
	new_pic->texid = dst.dst_tex_id + NodePicture::MAX_IN_PKG;
	new_pic->region[0] += dst.dst_xmin;
	new_pic->region[1] += dst.dst_ymin;
	new_pic->region[2] += dst.dst_xmin;
	new_pic->region[3] += dst.dst_ymin;
}

}