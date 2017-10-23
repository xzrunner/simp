#ifndef _SIMP_NODE_AUDIO_SPR_H_
#define _SIMP_NODE_AUDIO_SPR_H_

#include <cu/uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeAudioSpr : private cu::Uncopyable
{
public:
	uint32_t sym;

public:
	NodeAudioSpr(ImportStream& is);

	static int Size();

}; // NodeAudioSpr

}

#endif // _SIMP_NODE_AUDIO_SPR_H_