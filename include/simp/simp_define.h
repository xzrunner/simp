#ifndef _SIMP_DEFINES_H_
#define _SIMP_DEFINES_H_

#include <stdint.h>

#ifndef PTR_SIZE_DIFF
#define PTR_SIZE_DIFF (8 - sizeof(void *))
#endif // PTR_SIZE_DIFF

#ifndef SIZEOF_POINTER
#define SIZEOF_POINTER 8
#endif // SIZEOF_POINTER

namespace simp
{

static const uint32_t ANCHOR_ID		= 0xffffffff;
static const int SCREEN_SCALE		= 16;

static const int SIMP_PAGE_SIZE			= 1024 * 16;
static const int SIMP_PAGE_SIZE_MIN		= 1024 * 4;
static const int SIMP_PAGE_SIZE_MAX		= 1024 * 256;

static const uint32_t AUDIO_ID      = 0xfff;

}

#endif // _SIMP_DEFINES_H_