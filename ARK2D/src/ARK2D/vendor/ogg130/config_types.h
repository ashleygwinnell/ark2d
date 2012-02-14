#ifndef __CONFIG_TYPES_H__
#define __CONFIG_TYPES_H__

/* these are filled in by configure */
#define INCLUDE_INTTYPES_H INCLUDE_INTTYPES_H
#define INCLUDE_STDINT_H INCLUDE_STDINT_H
#define INCLUDE_SYS_TYPES_H INCLUDE_SYS_TYPES_H

#if INCLUDE_INTTYPES_H
	#include <inttypes.h>
#endif

#if INCLUDE_STDINT_H
	#include <stdint.h>
#endif

#if INCLUDE_SYS_TYPES_H
	#include <sys/types.h>
#endif

/*typedef int16_t ogg_int16_t;
typedef uint16_t ogg_uint16_t;
typedef int32_t ogg_int32_t;
typedef uint32_t ogg_uint32_t;
typedef int64_t ogg_int64_t;*/


#if defined(ARK2D_ANDROID)
	#include <inttypes.h>
	typedef int16_t ogg_int16_t;
	typedef uint16_t ogg_uint16_t;
	typedef int32_t ogg_int32_t;
	typedef uint32_t ogg_uint32_t;
	typedef int64_t ogg_int64_t;
#else
	typedef signed short ogg_int16_t;
	typedef unsigned short ogg_uint16_t;
	typedef int ogg_int32_t;
	typedef unsigned int ogg_uint32_t;
	typedef int64_t ogg_int64_t;
#endif



/*
typedef SIZE16 ogg_int16_t;
typedef USIZE16 ogg_uint16_t;
typedef SIZE32 ogg_int32_t;
typedef USIZE32 ogg_uint32_t;
typedef SIZE64 ogg_int64_t;*/


#endif
