#ifndef _SIMP_MESH_TYPE_H_
#define _SIMP_MESH_TYPE_H_

namespace simp
{

enum MeshType
{
	MESH_UNKNOWN	= 0,

	MESH_STRIP		= 1,
	MESH_POINTS		= 2,

	MESH_TRIANGLES	= 3,
	MESH_SKIN		= 4,
	MESH_SKIN2		= 5,

};

}

#endif // _SIMP_MESH_TYPE_H_