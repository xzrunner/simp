#include "SIMP_Package.h"
#include "ImportStream.h"
#include "simp_types.h"
#include "Page.h"
#include "PageAlloc.h"
#include "simp_define.h"

#include <fs_file.h>
#include <fault.h>
#include <bimp/bimp_typedef.h>
#include <bimp/Allocator.h>

#include <sstream>
#include <climits>

#include <stddef.h>
#include <assert.h>

namespace simp
{

Package::Package(const std::string& filepath, int id)
	: m_id(id)
	, m_min_node_id(0)
	, m_max_node_id(0)
{
	LoadIndex(filepath);
}

Package::~Package()
{
}

void Package::Traverse(NodeVisitor& visitor) const
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) 
	{
		const PageDesc& page = m_pages[i];
		bool loaded = page.page != NULL;
		if (!loaded) {
			LoadPage(i);
		}
		page.page->Traverse(visitor);
		if (!loaded) {
			UnloadPage(i);
		}
	}
}

uint32_t Package::QueryID(const std::string& name) const
{
	std::map<std::string, uint32_t>::const_iterator itr 
		= m_export_names.find(name);
	if (itr != m_export_names.end()) {
		return itr->second;
	} else {
		return 0xffffffff;
	}
}

const void* Package::QueryNode(uint32_t id, int* type)
{
	*type = TYPE_INVALID;
	Page* page = QueryPage(id);
	if (page) {
		return page->Query(id, type);
	} else {
		return NULL;
	}	
}

void Package::SetPagePath(int idx, const std::string& path)
{
	if (idx < 0 || idx >= m_pages.size()) {
		return;
	}

	m_pages[idx].filepath = path;
}

void Package::LoadIndex(const std::string& filepath)
{
	m_export_names.clear();
	m_pages.clear();

	PageDescLoader loader(filepath, m_export_names, m_pages);
	loader.Load();

	m_min_node_id = INT_MAX;
	m_max_node_id = -INT_MAX;
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		if (m_pages[i].min < m_min_node_id) {
			m_min_node_id = m_pages[i].min;
		}
		if (m_pages[i].max > m_max_node_id) {
			m_max_node_id = m_pages[i].max;
		}
	}
}

Page* Package::QueryPage(int id)
{
	int idx = -1;
	int start = 0;
	int end = m_pages.size() - 1;
	while (start <= end)
	{
		int mid = (start + end) / 2;
		const PageDesc& p = m_pages[mid];
		if (id >= p.min && id <= p.max) {
			idx = mid;
			break;
		} else if (id < p.min) {
			end = mid - 1;
		} else {
			start = mid + 1;
		}
	}

	assert(idx != -1);
	if (!m_pages[idx].page) {
		LoadPage(idx);
	}

	return m_pages[idx].page;	
}

void Package::LoadPage(int idx) const
{
	if (idx < 0 || idx >= m_pages.size()) {
		return;
	}

	const PageDesc& desc = m_pages[idx];

	assert(!desc.page);

	bimp::Allocator* alloc = PageAlloc::Instance()->Create(desc.size);

	int sz = ALIGN_4BYTE(Page::Size());
	void* ptr = alloc->Alloc(sz);
	Page* page = new (ptr) Page(m_id, alloc, desc.min, desc.max);
 	page->Load(desc.filepath);

	desc.page = page;
}

void Package::UnloadPage(int idx) const
{
	if (idx < 0 || idx >= m_pages.size() || !m_pages[idx].page) {
		return;
	}

	delete m_pages[idx].page;
	m_pages[idx].page = NULL;
}

/************************************************************************/
/* class Package::PageDesc                                              */
/************************************************************************/

Package::PageDesc::
PageDesc() 
	: page(NULL) 
{
}

Package::PageDesc::
~PageDesc()
{
	delete page;
}

/************************************************************************/
/* class Package::PageDescLoader                                        */
/************************************************************************/

Package::PageDescLoader::
PageDescLoader(const std::string& filepath, std::map<std::string, uint32_t>& export_names, 
			   std::vector<PageDesc>& pages)
	: FileLoader(filepath)
	, m_export_names(export_names)
	, m_pages(pages)
{
}

void Package::PageDescLoader::OnLoad(bimp::ImportStream& is)
{
	int export_n = is.UInt16();
	for (int i = 0; i < export_n; ++i)
	{
		std::string name = is.String();
		uint32_t id = is.UInt32();
		m_export_names.insert(std::make_pair(name, id));
	}

	int page_n = is.UInt16();
	m_pages.reserve(page_n);
	for (int i = 0; i < page_n; ++i)
	{
		PageDesc page;
		page.size = is.UInt32();
		page.min = is.UInt32();
		page.max = is.UInt32();
		m_pages.push_back(page);
	}
}

}