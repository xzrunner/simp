#ifndef _SIMP_DEFINES_H_
#define _SIMP_DEFINES_H_

#include <stdint.h>

namespace simp
{

static const int PTR_SIZE_DIFF		= (8 - sizeof(void *));
static const int SIZEOF_POINTER		= 8;

static const uint32_t ANCHOR_ID		= 0xffffffff;
static const int SCREEN_SCALE		= 16;

static const int SIMP_PAGE_SIZE			= 1024 * 16;
static const int SIMP_PAGE_SIZE_MIN		= 1024 * 4;
static const int SIMP_PAGE_SIZE_MAX		= 1024 * 64;

}

#endif // _SIMP_DEFINES_H_