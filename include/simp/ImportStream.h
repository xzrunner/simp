#ifndef _SIMP_IMPORT_STREAM_H_
#define _SIMP_IMPORT_STREAM_H_

#include <bimp/ImportStream.h>

#include <string>

namespace bimp { class Allocator; }

namespace simp
{

class ImportStream : public bimp::ImportStream
{
public:
	ImportStream(const char* data, int sz);

	uint32_t RGBA();
	uint32_t ABGR();

private:
	int m_current_id;

}; // ImportStream

}

#endif // _SIMP_IMPORT_STREAM_H_