#include "NodeAudioSpr.h"
#include "ImportStream.h"

#include <bimp/bimp_typedef.h>

namespace simp
{

NodeAudioSpr::NodeAudioSpr(ImportStream& is)
{
	sym = is.UInt32();
}

int NodeAudioSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAudioSpr));
}

}