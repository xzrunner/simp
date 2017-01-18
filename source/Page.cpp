#include "Page.h"
#include "ImportStream.h"
#include "simp_types.h"
#include "simp_define.h"
#include "PageAlloc.h"
#include "NodeFactory.h"
#include "NodeVisitor.h"
#include "RelocateTexcoords.h"

#include "NodePicture.h"
#include "NodeScale9.h"
#include "NodeScale9Spr.h"
#include "NodeIcon.h"
#include "NodeIconSpr.h"
#include "NodeTexture.h"
#include "NodeTextureSpr.h"
#include "NodeLabel.h"
#include "NodeComplex.h"
#include "NodeComplexSpr.h"
#include "NodeAnimation.h"
#include "NodeAnimationSpr.h"
#include "NodeParticle3d.h"
#include "NodeParticle3dSpr.h"
#include "NodeParticle2d.h"
#include "NodeParticle2dSpr.h"
#include "NodeShape.h"
#include "NodeShapeSpr.h"
#include "NodeMesh.h"
#include "NodeMeshSpr.h"
#include "NodeMask.h"
#include "NodeMaskSpr.h"
#include "NodeTrail.h"
#include "NodeTrailSpr.h"
#include "NodeAnim2.h"
#include "NodeAnim2Spr.h"

#include <bimp/FileLoader.h>
#include <bimp/typedef.h>
#include <bimp/Allocator.h>

#include <new>
#include <string.h>

#include <assert.h>

namespace simp
{

Page::Page(int pkg_id, bimp::Allocator* alloc, int begin_id, int end_id)
	: m_pkg_id(pkg_id)
	, m_alloc(alloc)
	, m_begin_id(begin_id)
	, m_end_id(end_id)
{
}

Page::~Page()
{
	PageAlloc::Instance()->Release(m_alloc);
}

void Page::Traverse(NodeVisitor& visitor) const
{
	for (int id = m_begin_id; id < m_end_id; ++id) {
		int idx = id - m_begin_id;
		visitor.Visit(id, m_types[idx], m_nodes[idx]);
	}
}

void Page::Load(const std::string& filepath)
{
	Loader loader(filepath, this);
	loader.Load();

	RelocateTexcoords::Instance()->Do(this);
}

const void* Page::Query(uint32_t id, int* type) const
{
	assert(id >= m_begin_id && id <= m_end_id);
	int idx = id - m_begin_id;
	*type = m_types[idx];
	return m_nodes[idx];
}

int Page::Size()
{
	return sizeof(Page) + PTR_SIZE_DIFF * 3;
}

/************************************************************************/
/* class Page::Loader                                                   */
/************************************************************************/

Page::Loader::
Loader(const std::string& filepath, Page* page)
	: bimp::FileLoader(filepath)
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
		m_page->m_nodes[idx] = CreateNode(type, alloc, is);
	}
}

void* Page::Loader::
CreateNode(uint8_t type, bimp::Allocator& alloc, bimp::ImportStream& is)
{
	void* ret = NULL;

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
			ret = new (ptr) NodeLabel(alloc, _is);
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
	default:
		assert(0);
	}

	return ret;
}

}