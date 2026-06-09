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

#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <Types.h>


class MemArea
{
    private:
        uintptr start, final;
    public:
        MemArea();
        MemArea(const uintptr start, const uintptr final);
        uintptr begin();
        uintptr end();
        bool valid();

        typedef uintptr iterator;
};

class StringOperations
{
    public:
        // Return the starting address where is sequence is allocated
        // else NULL
        static void *findInMemory(const char *sequence, MemArea memory);
        // Return size of ASCIZ sequence without 0-symbol, NULL on failure
        static uint32 sequenceSize(const char *chars);
};

#endif
