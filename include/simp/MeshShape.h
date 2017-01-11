#ifndef _SIMP_MESH_SHAPE_H_
#define _SIMP_MESH_SHAPE_H_

#include "MeshType.h"

namespace simp
{

class MeshShape
{
public:
	virtual ~MeshShape() {}
	virtual MeshType Type() const = 0;

}; // MeshShape

}

#endif // _SIMP_MESH_SHAPE_H_