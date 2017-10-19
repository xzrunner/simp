#ifndef _SIMP_PAGE_H_
#define _SIMP_PAGE_H_

#include <cu/uncopyable.h>
#include <bimp/FileLoader.h>

#include <stdint.h>

namespace bimp { class Allocator; class FilePath; }

namespace simp
{

class ImportStream;
class NodeVisitor;

class Page : private cu::Uncopyable
{
public:
	Page(int pkg_id, int pkg_version, bimp::Allocator* alloc, int begin_id, int end_id);
	~Page();

	void Traverse(NodeVisitor& visitor) const;

	void Load(const bimp::FilePath& filepath);

	const void* Query(uint32_t id, int* type) const;

	int GetPkgID() const { return m_pkg_id; }

	int GetBeginID() const { return m_begin_id; }

	static int Size();

private:
	class Loader : public bimp::FileLoader
	{
	public:
		Loader(int pkg_version, const std::string& filepath, Page* page);
		Loader(int pkg_version, fs_file* file, uint32_t offset, Page* page);

	protected:
		virtual void OnLoad(bimp::ImportStream& is);

	private:
		void* CreateNode(uint8_t type, bimp::Allocator& alloc, bimp::ImportStream& is) const;

	private:
		int m_pkg_version;

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
	uint16_t m_pkg_id;
	uint16_t m_pkg_version;

	int m_begin_id, m_end_id;

	mutable uint32_t m_flags;

	bimp::Allocator* m_alloc;

	uint8_t* m_types;
	void** m_nodes;

}; // Page

}

#endif // _SIMP_PAGE_H_