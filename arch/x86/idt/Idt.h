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

#ifndef IDT_H
#define IDT_H


#include "../gdt/Gdt.h"
#include <MemoryOperations.h>

// Defines

// Attributes
#define TSK_GATE 0x5
#define INT_GATE_16 0x6
#define TRAP_GATE_16 0x7
#define INT_GATE_32 0xe
#define TRAP_GATE_32 0xf
#define PRESENT_BIT 0x80
#define RING_KERN 0
#define RING_DRIVERSH 0x20
#define RING_DRIVERSL 0x40
#define RING_APP 0x60

//--------

class DescInterrupt32
{
    private:
        uint16 offsetLow;
        uint16 segmentSelector;
        uint8 attributes;
        uint16 offsetHight;
    public:
        DescInterrupt32() = default;
        DescInterrupt32(const DescInterrupt32 &) = default; 
        DescInterrupt32(uint32 offset, uint16 selector, uint8 attr);
        void setOffset(uint32 offset);
        void setGateType(uint8 gateType);
        void setDpl(uint8 dpl);
        uint64 getDescriptor() const;
        uint32 getOffset() const;
        static DescInterrupt32 InterruptKernelLong(uint32 offset,
                                                    uint16 selector);
        static DescInterrupt32 TrapKernelLong(uint32 offset,
                                                    uint16 selector);
        static DescInterrupt32 TaskKernel(uint16 selector);
};

class MngIdt
{
    private:
        const static uint8 reservedIntSize = 12;
        constexpr static uint8 reservedInts[reservedIntSize] =
        {
            9, 15, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
        };
        // If false interrupt number isn't reserved, true - reserved
        bool isIntReserved(uint8 interrupt);
        const static uint8 size = 255;
        static uint64 idt[size];
        static bool isInitialize;
        static bool idtLoaded;
        uint8 currIdx;
        // Start number of interrupts that service IRQ
        // Accpet this number as first interrupt descriptor
        // with dpl = 2 or 3 (drivers)
        uint8 irqInterruptStart;
    public:
        const static uint8 firstSystemIntNumber = 0x20;
        MngIdt();
        MngIdt(const MngIdt &) = delete;
        bool addDescriptor(DescInterrupt32 desc);
        bool setDescriptor(DescInterrupt32 desc, uint8 idx);
        bool canDescriptorWrite(uint8 idx);
        bool loadIdt();
        static MngIdt &getInstance();
        uint8 getIrqInterruptStart();
        bool isInterruptSet(uint8 numberInterrupt);
};

extern MngIdt mngIdt;

#endif
