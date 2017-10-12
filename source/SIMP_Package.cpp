#include "SIMP_Package.h"
#include "PkgIdxLoader.h"
#include "simp_types.h"
#include "Page.h"
#include "PageAlloc.h"
#include "simp_define.h"
#include "PageVisitor.h"

#include <logger.h>
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
		bool loaded = page.m_page != NULL;
		if (!loaded) {
			LoadPage(i);
		}
		page.m_page->Traverse(visitor);
		if (!loaded) {
			UnloadPage(i);
		}
	}
}

void Package::Traverse(PageVisitor& visitor) const
{
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		const PageDesc& page = m_pages[i];
		if (page.m_page) {
			visitor.Visit(page.m_page);
		}
	}
}

uint32_t Package::QueryID(const std::string& name) const
{
	auto& itr = m_export_names.find(name.c_str());
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
	if (idx < 0 || idx >= static_cast<int>(m_pages.size())) {
		return;
	}

	m_pages[idx].m_filepath = path;
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
	auto& itr = m_export_names.begin();
	for ( ; itr != m_export_names.end(); ++itr) {
		names.push_back(itr->first.c_str());
	}
}

void Package::LoadIndex(const std::string& filepath)
{
	m_export_names.clear();
	m_pages.clear();

	PkgIdxLoader loader(filepath.c_str(), m_export_names, m_pages);
	loader.Load();

	m_version      = loader.GetVersion();
	m_scale        = loader.GetScale();
	m_ref_pkgs     = loader.GetRefPkgs();

	m_min_node_id = INT_MAX;
	m_max_node_id = -INT_MAX;
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		if (m_pages[i].m_min < m_min_node_id) {
			m_min_node_id = m_pages[i].m_min;
		}
		if (m_pages[i].m_max > m_max_node_id) {
			m_max_node_id = m_pages[i].m_max;
		}
	}
}

void Package::LoadIndex(fs_file* file, uint32_t offset)
{
	m_export_names.clear();
	m_pages.clear();

	PkgIdxLoader loader(file, offset, m_export_names, m_pages);
	loader.Load();

	m_version      = loader.GetVersion();
	m_scale        = loader.GetScale();
	m_ref_pkgs     = loader.GetRefPkgs();
	
	m_min_node_id = INT_MAX;
	m_max_node_id = -INT_MAX;
	for (int i = 0, n = m_pages.size(); i < n; ++i) {
		if (m_pages[i].m_min < m_min_node_id) {
			m_min_node_id = m_pages[i].m_min;
		}
		if (m_pages[i].m_max > m_max_node_id) {
			m_max_node_id = m_pages[i].m_max;
		}
	}
}

Page* Package::QueryPage(int id)
{
	if (m_pages.empty()) {
		return NULL;
	}

	int idx = -1;
	int start = 0;
	int end = m_pages.size() - 1;
	while (start <= end)
	{
		int mid = (start + end) / 2;
		const PageDesc& p = m_pages[mid];
		if (id >= p.m_min && id <= p.m_max) {
			idx = mid;
			break;
		} else if (id < p.m_min) {
			end = mid - 1;
		} else {
			start = mid + 1;
		}
	}

	if (idx == -1) {
		LOGD("query page fail, pkg %d, id %d, start %d, end %d\n", m_id, id, start, end);
		return NULL;
	}
	if (!m_pages[idx].m_page) {
		if(!LoadPage(idx)) {
			return NULL;
		}
	}

	return m_pages[idx].m_page;	
}

bool Package::LoadPage(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_pages.size())) {
		return true;
	}

	const PageDesc& desc = m_pages[idx];

	assert(!desc.m_page);

	bimp::Allocator* alloc = PageAlloc::Instance()->Create(desc.m_size);
	if(!alloc) {
		return false;
	}

	int sz = ALIGN_4BYTE(Page::Size());
	void* ptr = alloc->Alloc(sz);
	Page* page = new (ptr) Page(m_id, m_version, alloc, desc.m_min, desc.m_max);
 	page->Load(desc.m_filepath);

	desc.m_page = page;
	return true;
}

void Package::UnloadPage(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_pages.size()) || !m_pages[idx].m_page) {
		return;
	}

	m_pages[idx].m_page->~Page();
	// delete m_pages[idx].page;
	m_pages[idx].m_page = NULL;
}

/************************************************************************/
/* class Package::PageDesc                                              */
/************************************************************************/

Package::PageDesc::
PageDesc() 
	: m_size(0)
	, m_min(0)
	, m_max(0)
	, m_page(NULL) 
{
}

Package::PageDesc::
PageDesc(int size, int min, int max)
	: m_size(size)
	, m_min(min)
	, m_max(max)
	, m_page(NULL) 
{
}

Package::PageDesc::
~PageDesc()
{
	if(m_page) {
		m_page->~Page();
	}
	// delete page;
}

void Package::PageDesc::
ClearPage()
{
	if (m_page) {
		m_page->~Page();
		// delete page;
		m_page = 0;
	}
}

}