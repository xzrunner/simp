#include "SIMP_Package.h"
#include "ImportStream.h"
#include "simp_types.h"
#include "Page.h"
#include "PageAlloc.h"
#include "simp_define.h"
#include "PageVisitor.h"

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
	, m_version(0)
	, m_min_node_id(0)
	, m_max_node_id(0)
	, m_scale(1)
{
	LoadIndex(filepath);
}

Package::Package(fs_file* file, uint32_t offset, int id)
	: m_id(id)
	, m_version(0)
	, m_min_node_id(0)
	, m_max_node_id(0)
	, m_scale(1)
{
	LoadIndex(file, offset);		
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

void Package::Traverse(PageVisitor& visitor) const
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		const PageDesc& page = m_pages[i];
		if (page.page) {
			visitor.Visit(page.page);
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

void Package::SetPagePath(int idx, const bimp::FilePath& path)
{
	if (idx < 0 || idx >= m_pages.size()) {
		return;
	}

	m_pages[idx].filepath = path;
}

void Package::ClearPages()
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		m_pages[i].ClearPage();
	}
}

void Package::GetExportNames(std::vector<std::string>& names) const
{
	names.reserve(m_export_names.size());
	std::map<std::string, uint32_t>::const_iterator itr 
		= m_export_names.begin();
	for ( ; itr != m_export_names.end(); ++itr) {
		names.push_back(itr->first);
	}
}

void Package::LoadIndex(const std::string& filepath)
{
	m_export_names.clear();
	m_pages.clear();

	PageDescLoader loader(filepath, m_export_names, m_pages, m_scale, m_ref_pkgs);
	loader.Load();

	m_version = loader.GetVersion();

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

void Package::LoadIndex(fs_file* file, uint32_t offset)
{
	m_export_names.clear();
	m_pages.clear();

	PageDescLoader loader(file, offset, m_export_names, m_pages, m_scale, m_ref_pkgs);
	loader.Load();

	m_version = loader.GetVersion();

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

	if (idx == -1) {
		fault("query page fail, pkg %d, id %d, start %d, end %d\n", m_id, id, start, end);
	}
	if (!m_pages[idx].page) {
		if(!LoadPage(idx)) {
			return NULL;
		}
	}

	return m_pages[idx].page;	
}

bool Package::LoadPage(int idx) const
{
	if (idx < 0 || idx >= m_pages.size()) {
		return true;
	}

	const PageDesc& desc = m_pages[idx];

	assert(!desc.page);

	bimp::Allocator* alloc = PageAlloc::Instance()->Create(desc.size);
	if(!alloc) {
		return false;
	}

	int sz = ALIGN_4BYTE(Page::Size());
	void* ptr = alloc->Alloc(sz);
	Page* page = new (ptr) Page(m_id, m_version, alloc, desc.min, desc.max);
 	page->Load(desc.filepath);

	desc.page = page;
	return true;
}

void Package::UnloadPage(int idx) const
{
	if (idx < 0 || idx >= m_pages.size() || !m_pages[idx].page) {
		return;
	}

	m_pages[idx].page->~Page();
	// delete m_pages[idx].page;
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
	if(page) {
		page->~Page();
	}
	// delete page;
}

void Package::PageDesc::
ClearPage()
{
	if (page) {
		page->~Page();
		// delete page;
		page = 0;
	}
}

/************************************************************************/
/* class Package::PageDescLoader                                        */
/************************************************************************/

Package::PageDescLoader::
PageDescLoader(const std::string& filepath, std::map<std::string, uint32_t>& export_names, 
			   std::vector<PageDesc>& pages, float& scale, std::vector<int>& ref_pkgs)
	: FileLoader(filepath)
	, m_version(0)
	, m_export_names(export_names)
	, m_pages(pages)
	, m_scale(scale)
	, m_ref_pkgs(ref_pkgs)
{
}

Package::PageDescLoader::
PageDescLoader(fs_file* file, uint32_t offset, std::map<std::string, uint32_t>& export_names, 
			   std::vector<PageDesc>& pages, float& scale, std::vector<int>& ref_pkgs)
	: FileLoader(file, offset)
	, m_version(0)
	, m_export_names(export_names)
	, m_pages(pages)
	, m_scale(scale)
	, m_ref_pkgs(ref_pkgs)
{
}

void Package::PageDescLoader::OnLoad(bimp::ImportStream& is)
{
	int export_n = 0;
	int version_flag = is.UInt16();
	if (version_flag == 0xffff) {
		m_version = is.UInt16();
		export_n = is.UInt16();
	} else {
		export_n = version_flag;
	}

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
		page.size = is.UInt32() * 2;
		page.min = is.UInt32();
		page.max = is.UInt32();
		m_pages.push_back(page);
	}

	// scale
	if (!is.Empty()) {
		m_scale = is.Float();
	}

	// ref pkgs
	if (!is.Empty()) 
	{
		uint16_t num = is.UInt16();
		m_ref_pkgs.reserve(num);
		for (int i = 0; i < num; ++i) {
			uint16_t pkg = is.UInt16();
			m_ref_pkgs.push_back(pkg);
		}
	}
}

}