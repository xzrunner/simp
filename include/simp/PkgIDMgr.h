#ifndef _SIMP_PKG_ID_MGR_H_
#define _SIMP_PKG_ID_MGR_H_

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>
#include <bimp/FileLoader.h>

namespace simp
{

class PkgIDMgr
{
public:
	void LoadPkgIDs(const CU_STR& filepath);
	int  QueryPkgID(const CU_STR& name) const;

	int GetPkgSize() const { return m_pkgs.size(); }
	void GetAllPkgNames(const char* names[]) const;

	void Clear();

private:
	struct PkgID
	{
		CU_STR path;
		int             id;
	};

	class PkgIDsLoader : public bimp::FileLoader
	{
	public:
		PkgIDsLoader(const CU_STR& filepath, CU_MAP<CU_STR, PkgID>& pkgs);

	protected:
		virtual void OnLoad(bimp::ImportStream& is) override;

	private:
		CU_MAP<CU_STR, PkgID>& m_pkgs;

	}; // PkgIDsLoader

private:
	CU_MAP<CU_STR, PkgID> m_pkgs;

	CU_SINGLETON_DECLARATION(PkgIDMgr);

}; // PkgIDMgr

}

#endif // _SIMP_PKG_ID_MGR_H_