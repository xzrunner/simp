#ifndef _SIMP_NODE_PICTURE_H_
#define _SIMP_NODE_PICTURE_H_

#include <CU_Uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodePicture : private cu::Uncopyable
{
public:
	uint16_t texid;
	uint16_t lod;
	uint16_t region[4];
	int16_t offx, offy;

public:
	static const int MAX_IN_PKG = 4096;

public:
	NodePicture(ImportStream& is);

	static int Size();

}; // NodePicture

}

#endif // _SIMP_NODE_PICTURE_H_