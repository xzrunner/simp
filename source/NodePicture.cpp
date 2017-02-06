#include "NodePicture.h"
#include "ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodePicture::NodePicture(ImportStream& is)
{
	texid = is.UInt8();
	for (int i = 0; i < 4; ++i) {
		region[i] = is.UInt16();
	}
	offx = static_cast<int16_t>(is.UInt16());
	offy = static_cast<int16_t>(is.UInt16());
}

int NodePicture::Size()
{
	return ALIGN_4BYTE(sizeof(NodePicture));
}

}