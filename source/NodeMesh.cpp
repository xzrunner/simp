#include "NodeMesh.h"
#include "ImportStream.h"
#include "simp_define.h"

#include "PointsMesh.h"
#include "TrianglesMesh.h"
#include "Skin2Mesh.h"

#include <bimp/typedef.h>
#include <bimp/Allocator.h>

namespace simp
{

NodeMesh::NodeMesh(bimp::Allocator& alloc, ImportStream& is)
	: shape(NULL)
{
	base_id = is.UInt32();
	
	int type = is.UInt8();
	switch (type)
	{
	case MESH_POINTS:
		{
			void* ptr = alloc.Alloc(PointsMesh::Size());
			shape = new (ptr) PointsMesh(alloc, is);
		}
		break;
	case MESH_TRIANGLES:
		{
			void* ptr = alloc.Alloc(TrianglesMesh::Size());
			shape = new (ptr) TrianglesMesh(alloc, is);
		}
		break;
	case MESH_SKIN2:
		{
			void* ptr = alloc.Alloc(Skin2Mesh::Size());
			shape = new (ptr) Skin2Mesh(alloc, is);
		}
		break;
	}
}

int NodeMesh::Size()
{
	return ALIGN_4BYTE(sizeof(NodeMesh) + PTR_SIZE_DIFF);
}

}