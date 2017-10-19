#include "PkgIdxLoader.h"
#include "ImportStream.h"

namespace simp
{

PkgIdxLoader::PkgIdxLoader(const std::string& filepath,
	                       CU_MAP<CU_STR, uint32_t>& export_names,
	                       CU_VEC<Package::PageDesc>& pages,
                           CU_VEC<int>& ref_pkgs)
	: FileLoader(filepath)
	, m_version(0)
	, m_export_names(export_names)
	, m_pages(pages)
	, m_scale(1)
	, m_ref_pkgs(ref_pkgs)
{
}

PkgIdxLoader::PkgIdxLoader(fs_file* file, uint32_t offset, 
	                       CU_MAP<CU_STR, uint32_t>& export_names,
	                       CU_VEC<Package::PageDesc>& pages,
	                       CU_VEC<int>& ref_pkgs)
	: FileLoader(file, offset)
	, m_version(0)
	, m_export_names(export_names)
	, m_pages(pages)
	, m_scale(1)
	, m_ref_pkgs(ref_pkgs)
{
}

void PkgIdxLoader::OnLoad(bimp::ImportStream& is)
{
	is.UInt16();	// todo: version flag
	m_version = is.UInt16();
	if (m_version < 2) {
		return;
	}

	int export_n = is.UInt16();
	for (int i = 0; i < export_n; ++i)
	{
		CU_STR name(is.String());
		uint32_t id = is.UInt32();
		m_export_names.insert(std::make_pair(name, id));
	}

	int page_n = is.UInt16();
	m_pages.reserve(page_n);
	for (int i = 0; i < page_n; ++i)
	{
		int size = is.UInt32();
		int min = is.UInt32();
		int max = is.UInt32();
		m_pages.push_back(Package::PageDesc(size, min, max));
	}

	// scale
	m_scale = is.Float();

	// ref pkgs
	uint16_t ref_n = is.UInt16();
	m_ref_pkgs.reserve(ref_n);
	for (int i = 0; i < ref_n; ++i) {
		uint16_t pkg = is.UInt16();
		m_ref_pkgs.push_back(pkg);
	}
}

}