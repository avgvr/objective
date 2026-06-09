/**
 * Copyright (c) 2026 Alexey Gavrilov <alexey.gavrilov@mail.com>
 *
 * This file is part of ObjectiveOS.
 *
 * ObjectiveOS is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * ObjectiveOS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ObjectiveOS. If not, see <https://www.gnu.org/licenses/>. 
 */

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
