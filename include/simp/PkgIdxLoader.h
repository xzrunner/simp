#ifndef _SIMP_PKG_IDX_LOADER_H_
#define _SIMP_PKG_IDX_LOADER_H_

#include "SIMP_Package.h"

#include <bimp/FileLoader.h>

namespace simp
{

class PkgIdxLoader : public bimp::FileLoader
{
public:
	PkgIdxLoader(const std::string& filepath,
		CU_MAP<CU_STR, uint32_t>& export_names,
		CU_VEC<Package::PageDesc>& pages,
		CU_VEC<int>& ref_pkgs);
	PkgIdxLoader(fs_file* file, uint32_t offset, 
		CU_MAP<CU_STR, uint32_t>& export_names,
		CU_VEC<Package::PageDesc>& pages,
		CU_VEC<int>& ref_pkgs);

	int GetVersion() const { return m_version; }

	float GetScale() const { return m_scale; }

protected:
	virtual void OnLoad(bimp::ImportStream& is);

private:
	int m_version;

	CU_MAP<CU_STR, uint32_t>& m_export_names;

	CU_VEC<Package::PageDesc>& m_pages;

	float m_scale;

	CU_VEC<int>& m_ref_pkgs;

}; // PkgIdxLoader

}

#endif // _SIMP_PKG_IDX_LOADER_H_