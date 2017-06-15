#ifndef _SIMP_PACKAGE_H_
#define _SIMP_PACKAGE_H_

#include <CU_Uncopyable.h>
#include <bimp/FileLoader.h>

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
	~Package();

	void Traverse(NodeVisitor& visitor) const;
	void Traverse(PageVisitor& visitor) const;

	uint32_t QueryID(const std::string& name) const;

	const void* QueryNode(uint32_t id, int* type);

	int GetPageCount() const { return m_pages.size(); }

	void SetPagePath(int idx, const std::string& path);

	int GetMaxNodeID() const { return m_max_node_id; }

	void ClearPages();

	void GetExportNames(std::vector<std::string>& names) const;

	float GetScale() const { return m_scale; }

private:
	void LoadIndex(const std::string& filepath);

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
		std::string filepath;

		int size;
		int min, max;

		mutable Page* page;

		friend class Package;

	}; // PageDesc

	class PageDescLoader : public bimp::FileLoader
	{
	public:
		PageDescLoader(const std::string& filepath, std::map<std::string, uint32_t>& export_names,
			std::vector<PageDesc>& pages, float& scale);

	protected:
		virtual void OnLoad(bimp::ImportStream& is);

	private:
		std::map<std::string, uint32_t>& m_export_names;

		std::vector<PageDesc>& m_pages;

		float& m_scale;

	}; // PageDescLoader

protected:
	std::map<std::string, uint32_t> m_export_names;

	std::vector<PageDesc> m_pages;

private:
	int m_id;

	int m_min_node_id, m_max_node_id;

	float m_scale;

}; // Package

}

#endif // _SIMP_PACKAGE_H_