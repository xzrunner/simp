#include "simp/Page.h"
#include "simp/ImportStream.h"
#include "simp/simp_types.h"
#include "simp/simp_define.h"
#include "simp/PageAlloc.h"
#include "simp/NodeFactory.h"
#include "simp/NodeVisitor.h"
#include "simp/RelocateTexcoords.h"

#include "simp/NodePicture.h"
#include "simp/NodeScale9.h"
#include "simp/NodeScale9Spr.h"
#include "simp/NodeIcon.h"
#include "simp/NodeIconSpr.h"
#include "simp/NodeTexture.h"
#include "simp/NodeTextureSpr.h"
#include "simp/NodeLabel.h"
#include "simp/NodeComplex.h"
#include "simp/NodeComplexSpr.h"
#include "simp/NodeAnimation.h"
#include "simp/NodeAnimationSpr.h"
#include "simp/NodeParticle3d.h"
#include "simp/NodeParticle3dSpr.h"
#include "simp/NodeParticle2d.h"
#include "simp/NodeParticle2dSpr.h"
#include "simp/NodeShape.h"
#include "simp/NodeShapeSpr.h"
#include "simp/NodeMesh.h"
#include "simp/NodeMeshSpr.h"
#include "simp/NodeMask.h"
#include "simp/NodeMaskSpr.h"
#include "simp/NodeTrail.h"
#include "simp/NodeTrailSpr.h"
#include "simp/NodeAnim2.h"
#include "simp/NodeAnim2Spr.h"
#include "simp/NodeAudioSpr.h"

#include <fs_file.h>
#include <bimp/FileLoader.h>
#include <bimp/typedef.h>
#include <bimp/Allocator.h>

#include <new>

#include <string.h>
#include <assert.h>
#include <fault.h>

namespace simp
{

Page::Page(int pkg_id, int pkg_version, bimp::Allocator* alloc, int begin_id, int end_id)
	: m_pkg_id(pkg_id)
	, m_pkg_version(pkg_version)
	, m_begin_id(begin_id)
	, m_end_id(end_id)
	, m_flags(0)
	, m_alloc(alloc)
{
}

Page::~Page()
{
	PageAlloc::Instance()->Release(m_alloc);
}

void Page::Traverse(NodeVisitor& visitor) const
{
	for (int id = m_begin_id; id <= m_end_id; ++id) {
		int idx = id - m_begin_id;
		visitor.Visit(id, m_types[idx], m_nodes[idx]);
	}
}

void Page::Load(const bimp::FilePath& filepath)
{
	if (filepath.IsSingleFile()) {
		Loader loader(m_pkg_version, filepath.GetFilepath().c_str(), this);
		loader.Load();
	} else {
		fs_file* file = fs_open(filepath.GetFilepath().c_str(), "rb");
		Loader loader(m_pkg_version, file, filepath.GetOffset(), this);
		loader.Load();
		fs_close(file);
	}

	RelocateTexcoords::Instance()->Do(this);
}

const void* Page::Query(uint32_t id, int* type) const
{
	assert(static_cast<int>(id) >= m_begin_id && static_cast<int>(id) <= m_end_id);
	int idx = id - m_begin_id;
	*type = m_types[idx];
	return m_nodes[idx];
}

int Page::Size()
{
	return ALIGN_4BYTE(sizeof(Page) + PTR_SIZE_DIFF * 3);
}

/************************************************************************/
/* class Page::Loader                                                   */
/************************************************************************/

Page::Loader::
Loader(int pkg_version, const CU_STR& filepath, Page* page)
	: bimp::FileLoader(filepath)
	, m_pkg_version(pkg_version)
	, m_page(page)
{
}

Page::Loader::
Loader(int pkg_version, fs_file* file, uint32_t offset, Page* page)
	: bimp::FileLoader(file, offset)
	, m_pkg_version(pkg_version)
	, m_page(page)
{
}

void Page::Loader::
OnLoad(bimp::ImportStream& is)
{
	bimp::Allocator& alloc = *(m_page->m_alloc);

	int n = m_page->m_end_id - m_page->m_begin_id + 1;
	int align_n = ALIGN_4BYTE(n);
	m_page->m_types = static_cast<uint8_t*>(alloc.Alloc(sizeof(uint8_t) * align_n));
	memset(m_page->m_types, 0, sizeof(uint8_t) * align_n);
	m_page->m_nodes = static_cast<void**>(alloc.Alloc(SIZEOF_POINTER * n));
	memset(m_page->m_nodes, 0, SIZEOF_POINTER * n);

	while (!is.Empty())
	{
		uint32_t id = is.UInt32();
		uint32_t node_id = NodeID::GetNodeID(id);

		int idx = node_id - m_page->m_begin_id;
		uint8_t type = is.UInt8();
		m_page->m_types[idx] = type;
		void* node = CreateNode(type, alloc, is);
		if (!node) {
			fault("++ create node %d fail, pkg %d\n", id, NodeID::GetPkgID(id));
		}
		m_page->m_nodes[idx] = node;
	}
}

void* Page::Loader::
CreateNode(uint8_t type, bimp::Allocator& alloc, bimp::ImportStream& is) const
{
	void* ret = nullptr;

	ImportStream& _is = static_cast<ImportStream&>(is);

	switch (type)
	{
	case TYPE_IMAGE:
		{
			void* ptr = alloc.Alloc(NodePicture::Size());
			ret = new (ptr) NodePicture(_is);
		}
		break;
	case TYPE_SCALE9:
		{
			void* ptr = alloc.Alloc(NodeScale9::Size());
			ret = new (ptr) NodeScale9(alloc, _is);
		}
		break;
	case TYPE_SCALE9_SPR:
		{
			void* ptr = alloc.Alloc(NodeScale9Spr::Size());
			ret = new (ptr) NodeScale9Spr(_is);
		}
		break;
	case TYPE_ICON:
		{
			void* ptr = alloc.Alloc(NodeIcon::Size());
			ret = new (ptr) NodeIcon(alloc, _is);
		}
		break;
	case TYPE_ICON_SPR:
		{
			void* ptr = alloc.Alloc(NodeIconSpr::Size());
			ret = new (ptr) NodeIconSpr(_is);
		}
		break;
	case TYPE_TEXTURE:
		{
			void* ptr = alloc.Alloc(NodeTexture::Size());
			ret = new (ptr) NodeTexture(alloc, _is);
		}
		break;
	case TYPE_TEXTURE_SPR:
		{
			void* ptr = alloc.Alloc(NodeTextureSpr::Size());
			ret = new (ptr) NodeTextureSpr(_is);
		}
		break;
	case TYPE_LABEL:
		{
			void* ptr = alloc.Alloc(NodeLabel::Size());
			ret = new (ptr) NodeLabel(alloc, _is, m_pkg_version >= 1);
		}
		break;
	case TYPE_COMPLEX:
		{
			void* ptr = alloc.Alloc(NodeComplex::Size());
			ret = new (ptr) NodeComplex(alloc, _is);
		}
		break;
	case TYPE_COMPLEX_SPR:
		{
			void* ptr = alloc.Alloc(NodeComplexSpr::Size());
			ret = new (ptr) NodeComplexSpr(_is);
		}
		break;
	case TYPE_ANIMATION:
		{
			void* ptr = alloc.Alloc(NodeAnimation::Size());
			ret = new (ptr) NodeAnimation(alloc, _is);
		}
		break;
	case TYPE_ANIM_SPR:
		{
			void* ptr = alloc.Alloc(NodeAnimationSpr::Size());
			ret = new (ptr) NodeAnimationSpr(_is);
		}
		break;
	case TYPE_PARTICLE3D:
		{
			void* ptr = alloc.Alloc(NodeParticle3d::Size());
			ret = new (ptr) NodeParticle3d(alloc, _is);
		}
		break;
	case TYPE_P3D_SPR:
		{
			void* ptr = alloc.Alloc(NodeParticle3dSpr::Size());
			ret = new (ptr) NodeParticle3dSpr(_is);
		}
		break;
	case TYPE_PARTICLE2D:
		{
			void* ptr = alloc.Alloc(NodeParticle2d::Size());
			ret = new (ptr) NodeParticle2d(alloc, _is);
		}
		break;
	case TYPE_P2D_SPR:
		{
			void* ptr = alloc.Alloc(NodeParticle2dSpr::Size());
			ret = new (ptr) NodeParticle2dSpr(_is);
		}
		break;
	case TYPE_SHAPE:
		{
			void* ptr = alloc.Alloc(NodeShape::Size());
			ret = new (ptr) NodeShape(alloc, _is);
		}
		break;
	case TYPE_SHAPE_SPR:
		{
			void* ptr = alloc.Alloc(NodeShapeSpr::Size());
			ret = new (ptr) NodeShapeSpr(_is);
		}
		break;
	case TYPE_MESH:
		{
			void* ptr = alloc.Alloc(NodeMesh::Size());
			ret = new (ptr) NodeMesh(alloc, _is);
		}
		break;
	case TYPE_MESH_SPR:
		{
			void* ptr = alloc.Alloc(NodeMeshSpr::Size());
			ret = new (ptr) NodeMeshSpr(alloc, _is);
		}
		break;
	case TYPE_MASK:
		{
			void* ptr = alloc.Alloc(NodeMask::Size());
			ret = new (ptr) NodeMask(_is);
		}
		break;
	case TYPE_MASK_SPR:
		{
			void* ptr = alloc.Alloc(NodeMaskSpr::Size());
			ret = new (ptr) NodeMaskSpr(_is);
		}
		break;
	case TYPE_TRAIL:
		{
			void* ptr = alloc.Alloc(NodeTrail::Size());
			ret = new (ptr) NodeTrail(alloc, _is);
		}
		break;
	case TYPE_TRAIL_SPR:
		{
			void* ptr = alloc.Alloc(NodeTrailSpr::Size());
			ret = new (ptr) NodeTrailSpr(_is);
		}
		break;
	case TYPE_ANIM2:
		{
			void* ptr = alloc.Alloc(NodeAnim2::Size());
			ret = new (ptr) NodeAnim2(alloc, _is);
		}
		break;
	case TYPE_ANIM2_SPR:
		{
			void* ptr = alloc.Alloc(NodeAnim2Spr::Size());
			ret = new (ptr) NodeAnim2Spr(_is);
		}
		break;
	case TYPE_AUDIO_SPR:
		{
			void* ptr = alloc.Alloc(NodeAudioSpr::Size());
			ret = new (ptr) NodeAudioSpr(_is);
		}
		break;
	}

	return ret;
}

}