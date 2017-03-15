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
	Page(int pkg_id, bimp::Allocator* alloc, int begin_id, int end_id);
	~Page();

	void Traverse(NodeVisitor& visitor) const;

	void Load(const std::string& filepath);

	const void* Query(uint32_t id, int* type) const;

	int GetPkgID() const { return m_pkg_id; }

	int GetBeginID() const { return m_begin_id; }

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
	static const uint32_t FLAG_TEXCOORDS_RELOCATED = 0x00000001;

#define SIMP_FLAG_METHOD(name, bit) \
	bool Is##name() const { \
		return (m_flags & bit) != 0; \
	} \
	void Set##name(bool flag) const { \
		if (flag) { \
			m_flags |= bit; \
		} else { \
			m_flags &= ~bit; \
		} \
	}

public:
	SIMP_FLAG_METHOD(TexcoordsRelocated, FLAG_TEXCOORDS_RELOCATED)

private:
	int m_pkg_id;

	int m_begin_id, m_end_id;

	mutable uint32_t m_flags;

	bimp::Allocator* m_alloc;

	uint8_t* m_types;
	void** m_nodes;

}; // Page

}

#endif // _SIMP_PAGE_H_