#ifndef _SIMP_PAGE_H_
#define _SIMP_PAGE_H_

#include <CU_Uncopyable.h>
#include <bimp/FileLoader.h>

#include <string>

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;
class NodeVisitor;

class Page : private cu::Uncopyable
{
public:
	Page(bimp::Allocator* alloc, int begin_id, int end_id);
	~Page();

	void Traverse(NodeVisitor& visitor) const;

	void Load(const std::string& filepath);

	const void* Query(uint32_t id, int* type) const;

	static int Size();

private:
	class Loader : public bimp::FileLoader
	{
	public:
		Loader(const std::string& filepath, Page* page);

	protected:
		virtual void OnLoad(bimp::ImportStream& is);

	private:
		static void* CreateNode(uint8_t type, bimp::Allocator& alloc, bimp::ImportStream& is);

	private:
		Page* m_page;

	}; // Loader

private:
	bimp::Allocator* m_alloc;

	int m_begin_id, m_end_id;	

	uint8_t* m_types;
	void** m_nodes;

}; // Page

}

#endif // _SIMP_PAGE_H_