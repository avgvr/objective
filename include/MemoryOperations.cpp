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

#include <MemoryOperations.h>

uint32 MemoryOperations::copy(void *source, void *destination, uint32 sizeBytes)
{
    if(source == nullptr || destination == nullptr) return NULL;

    uint8 *src = (uint8 *)source, *dst = (uint8*)destination;

    uint32 byteNumber = 1;
    while(byteNumber <= sizeBytes)
    {
        dst[byteNumber - 1] = src[byteNumber - 1];
        byteNumber++;
    }

    return byteNumber;
};

uint32 MemoryOperations::set(void *field, uint8 value, uint32 sizeBytes)
{
    if(field == nullptr) return NULL;

    uint8 *charField = (uint8*)field;

    uint32 byteNumber = 1;
    while(byteNumber <= sizeBytes)
    {
        charField[byteNumber - 1] = value;
        byteNumber++;
    }

    return byteNumber;
};
