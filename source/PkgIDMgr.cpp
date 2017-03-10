#include "PkgIDMgr.h"

#include <bimp/BIMP_ImportStream.h>

namespace simp
{

SINGLETON_DEFINITION(PkgIDMgr);

PkgIDMgr::PkgIDMgr()
{
}

void PkgIDMgr::LoadPkgIDs(const std::string& filepath)
{
	m_pkgs.clear();
	PkgIDsLoader loader(filepath, m_pkgs);
	loader.Load();
}

int PkgIDMgr::QueryPkgID(const std::string& name) const
{
	std::map<std::string, PkgID>::const_iterator itr 
		= m_pkgs.find(name);
	if (itr != m_pkgs.end()) {
		return itr->second.id;
	} else {
		return -1;
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
PkgIDsLoader(const std::string& filepath, std::map<std::string, PkgID>& pkgs)
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
		std::string name = is.String();
		PkgID pkg;
		pkg.path = is.String();
		pkg.id = is.UInt16();
		m_pkgs.insert(std::make_pair(name, pkg));
	}
}

}