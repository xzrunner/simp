#ifndef _SIMP_SKIN2_MESH_H_
#define _SIMP_SKIN2_MESH_H_

#include "simp/MeshShape.h"

#include <stdint.h>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream;

class Skin2Mesh : public MeshShape
{
public:
	struct Joint
	{
		uint16_t joint;
		int16_t  vx, vy;
		int16_t  weight;
	};

public:
	uint32_t vertices_n;
	uint32_t texcoords_n;
	uint32_t triangle_n;

	uint8_t*  joints_n;
	Joint*    joints;

	uint16_t* texcoords;

	uint16_t* triangle;

public:
	Skin2Mesh(bimp::Allocator& alloc, ImportStream& is);

	static int Size();

	static int JointSize();

}; // Skin2Mesh

}

#endif // _SIMP_SKIN2_MESH_H_