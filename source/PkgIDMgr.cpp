#include "PkgIDMgr.h"

#include <bimp/BIMP_ImportStream.h>

namespace simp
{

CU_SINGLETON_DEFINITION(PkgIDMgr);

PkgIDMgr::PkgIDMgr()
{
}

void PkgIDMgr::LoadPkgIDs(const CU_STR& filepath)
{
	m_pkgs.clear();
	PkgIDsLoader loader(filepath.c_str(), m_pkgs);
	loader.Load();
}

int PkgIDMgr::QueryPkgID(const CU_STR& name) const
{
	auto itr = m_pkgs.find(CU_STR(name));
	if (itr != m_pkgs.end()) {
		return itr->second.id;
	} else {
		return -1;
	}
}

void PkgIDMgr::GetAllPkgNames(const char* names[]) const
{
	auto itr = m_pkgs.begin();
	for (int i = 0; itr != m_pkgs.end(); ++itr, ++i) {
		names[i] = itr->first.c_str();
	}
}

void PkgIDMgr::Clear()
{
	m_pkgs.clear();
}

/************************************************************************/
/* class PkgIDMgr::PkgIDsLoader                                         */
/************************************************************************/

PkgIDMgr::PkgIDsLoader::
PkgIDsLoader(const std::string& filepath, CU_MAP<CU_STR, PkgID>& pkgs)
	: FileLoader(filepath)
	, m_pkgs(pkgs)
{
}

void PkgIDMgr::PkgIDsLoader::
OnLoad(bimp::ImportStream& is)
{
	int n = is.UInt16();
	for (int i = 0; i < n; ++i) 
	{
		CU_STR name(is.String());
		PkgID pkg;
		pkg.path = is.String();
		pkg.id = is.UInt16();
		m_pkgs.insert(std::make_pair(name, pkg));
	}
}

}