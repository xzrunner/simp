#ifndef _SIMP_NODE_AUDIO_H_
#define _SIMP_NODE_AUDIO_H_

#include <cu/uncopyable.h>

#include <stdint.h>

namespace simp
{

class ImportStream;

class NodeAudio : private cu::Uncopyable
{
public:
	uint32_t audio_id;

public:
	NodeAudio(ImportStream& is);

	static int Size();

}; // NodeAudio

}

#endif // _SIMP_NODE_AUDIO_H_