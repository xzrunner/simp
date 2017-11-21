#include "simp/NodeAudioSpr.h"
#include "simp/ImportStream.h"

#include <bimp/typedef.h>

namespace simp
{

NodeAudioSpr::NodeAudioSpr(ImportStream& is)
{
	sym = is.UInt32();

	volume   = is.Float();

	offset   = is.Float();
	duration = is.Float();

	fade_in  = is.Float();
	fade_out = is.Float();
}

int NodeAudioSpr::Size()
{
	return ALIGN_4BYTE(sizeof(NodeAudioSpr));
}

}