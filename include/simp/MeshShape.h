#ifndef _SIMP_MESH_SHAPE_H_
#define _SIMP_MESH_SHAPE_H_

#include "simp/MeshType.h"

namespace simp
{

class MeshShape
{
public:
	MeshType type;

public:
	MeshShape() : type(MESH_UNKNOWN) {}
	
}; // MeshShape

}

#endif // _SIMP_MESH_SHAPE_H_