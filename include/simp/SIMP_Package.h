#ifndef _SIMP_PACKAGE_H_
#define _SIMP_PACKAGE_H_

#include <CU_Uncopyable.h>
#include <bimp/FileLoader.h>
#include <bimp/FilePath.h>

#include <string>
#include <vector>
#include <map>

#include <stdint.h>

namespace bimp { class ImportStream; }

namespace simp
{

class Image;
class Page;
class NodeVisitor;
class PageVisitor;

class Package : private cu::Uncopyable
{
public:
	Package(const std::string& filepath, int id);
	Package(fs_file* file, uint32_t offset, int id);
	~Package();

	void Traverse(NodeVisitor& visitor) const;
	void Traverse(PageVisitor& visitor) const;

	uint32_t QueryID(const std::string& name) const;

	const void* QueryNode(uint32_t id, int* type);

	int GetPageCount() const { return m_pages.size(); }

	void SetPagePath(int idx, const bimp::FilePath& path);

	int GetMaxNodeID() const { return m_max_node_id; }

	void ClearPages();

	void GetExportNames(std::vector<std::string>& names) const;

	float GetScale() const { return m_scale; }

	const std::vector<int>& GetRefPkgs() const { return m_ref_pkgs; }

private:
	void LoadIndex(const std::string& filepath);
	void LoadIndex(fs_file* file, uint32_t offset);

	Page* QueryPage(int id);
	bool LoadPage(int idx) const;
	void UnloadPage(int idx) const;

private:
	class PageDesc
	{
	public:
		PageDesc();
		~PageDesc();

		void ClearPage();

	private:
		bimp::FilePath filepath;

		int size;
		int min, max;

		mutable Page* page;

		friend class Package;

	}; // PageDesc

	class PageDescLoader : public bimp::FileLoader
	{
	public:
		PageDescLoader(const std::string& filepath, std::map<std::string, uint32_t>& export_names,
			std::vector<PageDesc>& pages, float& scale, std::vector<int>& ref_pkgs);
		PageDescLoader(fs_file* file, uint32_t offset, std::map<std::string, uint32_t>& export_names,
			std::vector<PageDesc>& pages, float& scale, std::vector<int>& ref_pkgs);

		int GetVersion() const { return m_version; }

	protected:
		virtual void OnLoad(bimp::ImportStream& is);

	private:
		int m_version;

		std::map<std::string, uint32_t>& m_export_names;

		std::vector<PageDesc>& m_pages;

		float& m_scale;

		std::vector<int>& m_ref_pkgs;

	}; // PageDescLoader

protected:
	std::map<std::string, uint32_t> m_export_names;

	std::vector<PageDesc> m_pages;

private:
	int m_id;

	int m_version;

	int m_min_node_id, m_max_node_id;

	float m_scale;

	std::vector<int> m_ref_pkgs;

}; // Package

}

#endif // _SIMP_PACKAGE_H_