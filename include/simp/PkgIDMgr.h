#ifndef _SIMP_PKG_ID_MGR_H_
#define _SIMP_PKG_ID_MGR_H_

#include <cu/cu_macro.h>

#include <bimp/FileLoader.h>

#include <map>
#include <string>
#include <vector>

namespace simp
{

class PkgIDMgr
{
public:
	void LoadPkgIDs(const std::string& filepath);
	int  QueryPkgID(const std::string& name) const;

	int GetPkgSize() const { return m_pkgs.size(); }
	void GetAllPkgNames(const char* names[]) const;

	void Clear();

private:
	struct PkgID
	{
		std::string path;
		int         id;
	};

	class PkgIDsLoader : public bimp::FileLoader
	{
	public:
		PkgIDsLoader(const bimp::ResString& filepath, std::map<std::string, PkgID>& pkgs);

	protected:
		virtual void OnLoad(bimp::ImportStream& is);

	private:
		std::map<std::string, PkgID>& m_pkgs;

	}; // PkgIDsLoader

private:
	std::map<std::string, PkgID> m_pkgs;

	CU_SINGLETON_DECLARATION(PkgIDMgr);

}; // PkgIDMgr

}

#endif // _SIMP_PKG_ID_MGR_H_