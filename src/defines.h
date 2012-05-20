#ifndef __DEFINES_H__
#define __DEFINES_H__

#ifdef _WIN32
# include <windows.h>
#elif __unix__
# include <dirent.h>
#else
// MacOS things?
#endif

#ifdef _WIN32
typedef signed __int64		int64;
typedef signed __int32		int32;
typedef signed __int16		int16;
typedef signed __int8		int8;
typedef unsigned __int64	uint64;
typedef unsigned __int32	uint32;
typedef unsigned __int16	uint16;
typedef unsigned __int8		uint8;
#elif __unix__
#include <stdint.h>
typedef int64_t				int64;
typedef int32_t				int32;
typedef int16_t				int16;
typedef int8_t				int8;
typedef uint64_t			uint64;
typedef uint32_t			uint32;
typedef uint16_t			uint16;
typedef uint8_t				uint8;
#endif

#define __STORMLIB_SELF__
#include "StormLib.h"

#include <cstdio>
#include <string>
#include <vector>
using namespace std;

#endif

