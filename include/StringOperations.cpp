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

#include "StringOperations.h"


MemArea::MemArea()
{
    start = final = NULL;
};

MemArea::MemArea(const uintptr start, const uintptr final)
{
    this->start = start;
    this->final = final;

    if(!valid())
    {
        *this = MemArea();
    }
};

uintptr MemArea::begin()
{
    return start;
}

uintptr MemArea::end()
{
    if(start == final) return start;
    else return final + 1;
};

bool MemArea::valid()
{
    if(start > final
        or start == NULL
        or final == NULL) return false;
    else return true;
};

uint32 StringOperations::sequenceSize(const char *chars)
{
    uint32 indx = 0;
    while(chars[indx++]){}

    return --indx;
};

void *StringOperations::findInMemory(const char *sequence, MemArea memory)
{
    if(sequence == nullptr or !memory.valid()) return NULL;

    uint32 seqIndx = 0;
    for(MemArea::iterator it = memory.begin(); it != memory.end(); it++)
    {
        char ch = sequence[seqIndx], chMem = (*((char *)it));

        if(ch == '\0') return (void *)(it - sequenceSize(sequence));
        else if(ch != chMem)
        {
            seqIndx = 0;
        }
        else if(ch == chMem)
        {
            seqIndx++;
        }
    }
    return NULL;
};
