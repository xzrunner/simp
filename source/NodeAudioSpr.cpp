#include "simp/NodeAudioSpr.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

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