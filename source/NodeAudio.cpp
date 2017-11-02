#include "simp/NodeAudio.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeAudio::NodeAudio(ImportStream& is)
{
	audio_id = is.UInt32();
}

int NodeAudio::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAudio));
}

}