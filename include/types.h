#ifndef TYPES_H
#define TYPES_H

#define __IAPC

#define NULL 0

#if defined(__IAPC)
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;

#if defined(__x86_64__)
    typedef uint64 uword;
#else
    typedef uint32 uword;
#endif

#if defined(__x86_64) || defined(__i686)
    typedef uint64 uintptr;
#else
    typedef uint32 uintptr;
#endif
#else
    #error IA-PC only support
#endif

#endif
