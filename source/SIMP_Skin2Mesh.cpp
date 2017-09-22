#include "SIMP_Skin2Mesh.h"
#include "ImportStream.h"
#include "simp_define.h"
#include "NodeArray.h"

#include <bimp/bimp_typedef.h>
#include <bimp/Allocator.h>

namespace simp
{

Skin2Mesh::Skin2Mesh(bimp::Allocator& alloc, ImportStream& is)
{
	type = MESH_SKIN2;

	uint16_t n;
	joints_n = NodeArray::Load8(alloc, is, 1, n);
	vertices_n = n;

	int count = 0;
	for (uint32_t i = 0; i < vertices_n; ++i) {
		count += joints_n[i];
	}

	joints = (Joint*)alloc.Alloc(JointSize() * count);
	for (int i = 0; i < count; ++i) 
	{
		joints[i].joint  = is.UInt16();
		joints[i].vx     = (int16_t)(is.UInt16());
		joints[i].vy     = (int16_t)(is.UInt16());
		joints[i].weight = (int16_t)(is.UInt16());
	}

	texcoords = NodeArray::Load16(alloc, is, 2, n);
	texcoords_n = n;
	triangle = NodeArray::Load16(alloc, is, 1, n);
	triangle_n = n;
}

int Skin2Mesh::Size()
{
	return ALIGN_4BYTE(sizeof(Skin2Mesh) + PTR_SIZE_DIFF * 4);
}

int Skin2Mesh::JointSize()
{
	return ALIGN_4BYTE(sizeof(Joint));
}

}