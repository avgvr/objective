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

#include "Idt.h"


uint64 MngIdt::idt[MngIdt::size];
bool MngIdt::isInitialize = false;
bool MngIdt::idtLoaded = false;
MngIdt mngIdt;

DescInterrupt32::DescInterrupt32(uint32 offset, uint16 selector, uint8 attr)
{
    uint8 gateType = attr & 0x0f;

    if((gateType == TSK_GATE and offset != 0)
        or gateType < TSK_GATE
        or (gateType > TRAP_GATE_16 and gateType < INT_GATE_32)
        or gateType > TRAP_GATE_32
        or !(attr & PRESENT_BIT))
    {
        offsetLow = 0;
        offsetHight = 0;
        segmentSelector = 0;
        attributes = 0;
    }
    else
    {
        offsetLow = offset & 0xffff;
        offsetHight = (offset & 0xffff0000) >> 16;
        segmentSelector = selector;
        attributes = attr;
    }
};

uint64 DescInterrupt32::getDescriptor() const
{
    uint64 desc = 0;

    desc |= offsetLow;
    desc |= (uint32)segmentSelector << 16;
    desc |= (uint64)attributes << 40;
    desc |= (uint64)offsetHight << 48;

    return desc;
};

uint32 DescInterrupt32::getOffset() const
{
    return offsetLow | offsetHight << 16;
};

DescInterrupt32 DescInterrupt32::InterruptKernelLong(uint32 offset, uint16 selector)
{
    return DescInterrupt32(
                          offset,
                          selector,
                          PRESENT_BIT | RING_KERN | INT_GATE_32
                          );
};

DescInterrupt32 DescInterrupt32::TrapKernelLong(uint32 offset, uint16 selector)
{
    return DescInterrupt32(
                          offset,
                          selector,
                          PRESENT_BIT | RING_KERN | TRAP_GATE_32
                          );
};

DescInterrupt32 DescInterrupt32::TaskKernel(uint16 selector)
{
    return DescInterrupt32(
                          NULL,
                          selector,
                          PRESENT_BIT | RING_KERN | TSK_GATE
                          );
};

bool MngIdt::isIntReserved(uint8 interrupt)
{
    for(uint8 indx = 0; indx < reservedIntSize; indx++)
    {
        if(reservedInts[indx] == interrupt) return true;
    }
    return false;
};

MngIdt::MngIdt()
{
    if(!isInitialize)
    {
        MemoryOperations::set(idt, 0, sizeof(uint64) * size);
        currIdx = NULL;
        idtLoaded = false;
        isInitialize = true;
        irqInterruptStart = firstSystemIntNumber;
    }
};

bool MngIdt::canDescriptorWrite(uint8 idx)
{
    if(idt[idx] or idtLoaded) return false;
    else return true;
};

bool MngIdt::addDescriptor(DescInterrupt32 desc)
{
    uint64 descqw = desc.getDescriptor();

    if(!canDescriptorWrite(currIdx) or isIntReserved(currIdx)
        or desc.getOffset() == NULL) return false;

    idt[currIdx] = descqw;

    uint8 attributes = descqw >> 40;

    if((attributes & RING_DRIVERSH or
       attributes & RING_DRIVERSL) and
       (irqInterruptStart == firstSystemIntNumber))
    {
        irqInterruptStart = (uint8)(descqw >> 16);
    }

    while(isIntReserved(++currIdx));

    return true;
};

bool MngIdt::setDescriptor(DescInterrupt32 desc, uint8 idx)
{
    if(!canDescriptorWrite(idx)) return false;

    idt[idx] = desc.getDescriptor();
    return true;
};

bool MngIdt::loadIdt()
{
    if(idtLoaded) return false;

    //Set descriptor
    uint64 descriptor = 0;
    uint16 size = sizeof(uint64) * this->size - 1;
    uint32 offset = (uint32)(&idt);

    descriptor |= offset;
    descriptor <<= 16;
    descriptor |= size;

    // Load descriptor in LDTR
    asm ("lidt %0" :: "m"(descriptor));

    idtLoaded = true;
    return true;
};

// Get static instance of MngIdt initialized once
MngIdt &MngIdt::getInstance()
{
    if(!isInitialize) mngIdt = MngIdt();
    return mngIdt;
};

void DescInterrupt32::setOffset(uint32 offset)
{
    uint8 gateType = attributes & 0x0f;

    if(!(gateType == TSK_GATE and offset != 0))
    {
        offsetLow = offset & 0xffff;
        offsetHight = (offset & 0xffff0000) >> 16;
    }
};
void DescInterrupt32::setGateType(uint8 gateType)
{
    if(gateType < TSK_GATE
        or (gateType > TRAP_GATE_16 and gateType < INT_GATE_32)
        or gateType > TRAP_GATE_32
    ) return;
    else if(gateType == TSK_GATE)
    {
        offsetLow = offsetHight = 0;
    }

    attributes &= 0xfff0;
    attributes |= (gateType & 0x0f);
};

void DescInterrupt32::setDpl(uint8 dpl)
{
    attributes = (dpl & 0x03) << 5;
};

bool MngIdt::isInterruptSet(uint8 numberInterrupt)
{
    return idt[numberInterrupt];
};

uint8 MngIdt::getIrqInterruptStart()
{
    return irqInterruptStart;
};
