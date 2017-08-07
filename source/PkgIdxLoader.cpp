#include "PkgIdxLoader.h"
#include "ImportStream.h"

namespace simp
{

PkgIdxLoader::PkgIdxLoader(const std::string& filepath)
	: FileLoader(filepath)
	, m_version(0)
	, m_scale(1)
{
}

PkgIdxLoader::PkgIdxLoader(fs_file* file, uint32_t offset)
	: FileLoader(file, offset)
	, m_version(0)
	, m_scale(1)
{
}

void PkgIdxLoader::OnLoad(bimp::ImportStream& is)
{
	int export_n = 0;
	int version_flag = is.UInt16();
	if (version_flag == 0xffff) {
		m_version = is.UInt16();
		export_n = is.UInt16();
	} else {
		export_n = version_flag;
	}

	if (m_version < 2) {
		return;
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