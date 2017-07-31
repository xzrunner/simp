#ifndef _SIMP_PKG_IDX_LOADER_H_
#define _SIMP_PKG_IDX_LOADER_H_

#include "SIMP_Package.h"

#include <bimp/FileLoader.h>

#include <map>
#include <vector>

namespace simp
{

class PkgIdxLoader : public bimp::FileLoader
{
public:
	PkgIdxLoader(const std::string& filepath);
	PkgIdxLoader(fs_file* file, uint32_t offset);

	int GetVersion() const { return m_version; }

	const std::map<std::string, uint32_t>& GetExportNames() const { return m_export_names; }

	const std::vector<Package::PageDesc>& GetPages() const { return m_pages; }

	float GetScale() const { return m_scale; }

	const std::vector<int>& GetRefPkgs() const { return m_ref_pkgs; }

protected:
	virtual void OnLoad(bimp::ImportStream& is);

private:
	int m_version;

	std::map<std::string, uint32_t> m_export_names;

	std::vector<Package::PageDesc> m_pages;

	float m_scale;

	std::vector<int> m_ref_pkgs;

}; // PkgIdxLoader

}

#endif // _SIMP_PKG_IDX_LOADER_H_