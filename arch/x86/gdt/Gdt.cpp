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

#include "Gdt.h"


uint64 MngGdt::gdt[MngGdt::size];
bool MngGdt::isInitialize = false;
bool MngGdt::gdtLoaded = false;
MngGdt mngGdt;

MngGdt::MngGdt()
{
    if(!isInitialize)
    {
        MemoryOperations::set(gdt, 0, sizeof(uint64) * size);
        currIdx = 1;
        gdtLoaded = false;
        isInitialize = true;
        codeSegmentKern = NULL;
    }
};

DescSeg32::DescSeg32(uint32 base, uint32 limit, uint8 access, uint8 flags)
{
    this->baseLow = base & 0x0000FFFF;
    this->baseMiddle = (base & 0x00FF0000) >> 16;
    this->baseHight = (base & 0xFF000000) >> 24;

    this->limitLow = limit & 0x0000FFFF;
    this->limitMiddle = (limit & 0x000F0000) >> 16;

    this->access = access;
    this->flags = flags & 0x000E;
};

uint64 DescSeg32::getDescriptor()
{
    uint64 descriptor = 0;

    descriptor |= (uint64)limitLow;
    descriptor |= (uint64)baseLow << 16;
    descriptor |= (uint64)baseMiddle << 32;
    descriptor |= (uint64)access << 40;
    descriptor |= (uint64)limitMiddle << 48;
    descriptor |= (uint64)flags << 52;
    descriptor |= (uint64)baseHight << 56;

    return descriptor;
};

uint64 DescSeg32::flatCodeKernel()
{
    return DescSeg32(0, 0xFFFFF, CODE_ER | PRESENT | NSYSTEMSEG
                                    , PAGEGRAN | HIGHSIZE).getDescriptor();    
};

uint64 DescSeg32::flatDataKernel()
{
    return DescSeg32(0, 0xFFFFF, DATA_RW | PRESENT | NSYSTEMSEG
                                    , PAGEGRAN | HIGHSIZE).getDescriptor();    
}

uint64 DescSeg32::flatCodeUser()
{

    return DescSeg32(0, 0xFFFFF, CODE_ER | PRESENT | NSYSTEMSEG | USER
                                    , PAGEGRAN | HIGHSIZE).getDescriptor();    
};

uint64 DescSeg32::flatDataUser()
{

    return DescSeg32(0, 0xFFFFF, DATA_RW | PRESENT | NSYSTEMSEG | USER
                                    , PAGEGRAN | HIGHSIZE).getDescriptor();    
};

bool MngGdt::addDescriptor(uint64 desc)
{
    if(currIdx >= size or currIdx == 0 or gdtLoaded)
    {
        return false;
    }

    if(desc == DescSeg32::flatCodeKernel()) codeSegmentKern = currIdx * 8;
    this->gdt[currIdx++] = desc;
    return true;
};

bool MngGdt::loadGdt()
{
    if(gdtLoaded)
    {
        return false;
    }

    // Set pointer
    uint64 descriptor = 0;
    uint16 size = sizeof(uint64) * this->size - 1;
    uint32 offset = (uint32)(&gdt);

    descriptor |= offset;
    descriptor <<= 16;
    descriptor |= size;

    SWITCHTOGDT(descriptor);

    gdtLoaded = 1;
    return true;
};

// Get static instance of MngGdt initialized once
MngGdt &MngGdt::getInstance()
{
    if(!isInitialize) mngGdt = MngGdt();
    return mngGdt;
};

uint16 MngGdt::getCodeSegmentKernel()
{
    return codeSegmentKern;
};
