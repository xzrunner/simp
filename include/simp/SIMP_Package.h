#ifndef _SIMP_PACKAGE_H_
#define _SIMP_PACKAGE_H_

#include <cu/uncopyable.h>
#include <memmgr/Allocator.h>
#include <bimp/FilePath.h>

#include <string>
#include <vector>
#include <map>

#include <stdint.h>

namespace bimp { class ImportStream; }

struct fs_file;

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

public:
	class PageDesc
	{
	public:
		PageDesc();
		PageDesc(int size, int min, int max);
		~PageDesc();

		void ClearPage();

	private:
		bimp::FilePath m_filepath;

		int m_size;
		int m_min, m_max;

		mutable Page* m_page;

		friend class Package;

	}; // PageDesc

private:
	void LoadIndex(const std::string& filepath);
	void LoadIndex(fs_file* file, uint32_t offset);

	Page* QueryPage(int id);
	bool LoadPage(int idx) const;
	void UnloadPage(int idx) const;

protected:
	std::map<mm::AllocString, uint32_t> m_export_names;

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