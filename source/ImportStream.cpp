#include "simp/ImportStream.h"

#include <bimp/typedef.h>

#include <fault.h>

namespace simp
{

ImportStream::ImportStream(const char* data, int sz)
	: bimp::ImportStream(data, sz)
	, m_current_id(-1)
{
}

uint32_t ImportStream::RGBA()
{
	if (m_size < 4) {
		fault("Invalid import stream (%d)", m_current_id);
	}

	uint8_t b[4];
	b[0] = (uint8_t)*(m_stream);
	b[1] = (uint8_t)*(m_stream + 1);
	b[2] = (uint8_t)*(m_stream + 2);
	b[3] = (uint8_t)*(m_stream + 3);
	m_stream += 4;
	m_size -= 4;

	return b[0] | (uint32_t)b[1] << 8 | (uint32_t)b[2] << 16 | (uint32_t)b[3] << 24;
}

uint32_t ImportStream::ABGR()
{
	if (m_size < 4) {
		fault("Invalid import stream (%d)", m_current_id);
	}

	uint8_t b[4];
	b[0] = (uint8_t)*(m_stream);
	b[1] = (uint8_t)*(m_stream + 1);
	b[2] = (uint8_t)*(m_stream + 2);
	b[3] = (uint8_t)*(m_stream + 3);
	m_stream += 4;
	m_size -= 4;

	return b[3] | (uint32_t)b[2] << 8 | (uint32_t)b[1] << 16 | (uint32_t)b[0] << 24;
}

}