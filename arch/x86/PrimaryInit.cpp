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

#include "PrimaryInit.h"

// Test ISRs define start
__attribute__((interrupt)) void isr0(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "Divide error" << '\n';
};

__attribute__((interrupt)) void isr1(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "Debug exception" << '\n';
};

__attribute__((interrupt)) void isr2(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "NMI interrupt" << '\n';
};

__attribute__((interrupt)) void isr3(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "Breakpoint" << '\n';
};

__attribute__((interrupt)) void isr4(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "Overflow" << '\n';
};

__attribute__((interrupt)) void isr5(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "Bound range exceed" << '\n';
};

__attribute__((interrupt)) void isr6(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "Invalid opcode" << '\n';
};

__attribute__((interrupt)) void isr7(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "No math coprocessor" << '\n';
};

__attribute__((interrupt)) void isr8(const struct InterruptFrame *frame, const uword errorCode)
{
    asm("cli\n\t");
    out::display << "Double fault" << '\n';
};

__attribute__((interrupt)) void isr10(const struct InterruptFrame *frame, const uword errorCode)
{
    asm("cli\n\t");
    out::display << "Invalid TSS" << "\n";
};

__attribute__((interrupt)) void isr11(const struct InterruptFrame *frame, const uword errorCode)
{
    asm("cli\n\t");
    out::display << "Segment not present" << "\n";
};

__attribute__((interrupt)) void isr12(const struct InterruptFrame *frame, const uword errorCode)
{
    asm("cli\n\t");
    out::display << "Stack segment fault" << "\n";
};

__attribute__((interrupt)) void isr13(const struct InterruptFrame *frame, const uword errorCode)
{
    asm("cli\n\t");
    out::display << "#GP" << "\n";
};

__attribute__((interrupt)) void isr14(const struct InterruptFrame *frame, const uword errorCode)
{
    asm("cli\n\t");
    out::display << "Page fault" << "\n";
};

__attribute__((interrupt)) void isr16(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "Math fault" << "\n";
};

__attribute__((interrupt)) void isr17(const struct InterruptFrame *frame, const uword errorCode)
{
    asm("cli\n\t");
    out::display << "Alignment check" << "\n";
};

__attribute__((interrupt)) void isr18(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "Machine check" << "\n";
};

__attribute__((interrupt)) void isr19(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "SIMD point exception" << "\n";
};

__attribute__((interrupt)) void isr20(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "Virtualization exception" << "\n";
};

__attribute__((interrupt)) void isr21(const struct InterruptFrame *frame, const uword errorCode)
{
    asm("cli\n\t");
    out::display << "Control protection exception" << "\n";
};

__attribute__((interrupt)) void isrUser(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "User-defined interrupt" << "\n";
};

__attribute__((interrupt)) void isrKeyboard(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "KINT " << (uint32)PortOps::inb(0x60);

    Pic::sendEoi();
};
__attribute__((interrupt)) void isrTimer(const struct InterruptFrame *frame)
{
    asm("cli\n\t");
    out::display << "Timer interrupt" << "\n";

    Pic::sendEoi();
};
//Test ISRs define end -------------------

void isrPerform::perform(null null, DescInterrupt32 &desc) {};

void isrPerform::perform(isr funp, DescInterrupt32 &desc)
{
    desc.setOffset((uint32)funp);
    mngIdt.addDescriptor(desc);
};

void isrPerform::perform(isrExc funp, DescInterrupt32 &desc)
{
    desc.setOffset((uint32)funp);
    mngIdt.addDescriptor(desc);
};

void PrimaryInitialization::processorInitialize()
{
    MngIdt &mngIdt = MngIdt::getInstance();
    MngGdt &mngGdt = MngGdt::getInstance();

    mngGdt.addDescriptor(DescSeg32::flatCodeKernel());
    mngGdt.addDescriptor(DescSeg32::flatDataKernel());
    mngGdt.loadGdt();

    InterruptFuncs intf = InterruptFuncs
                              (
                                isr0, isr1, isr2, isr3,
                                isr4, isr5, isr6, isr7,
                                isr8, nullptr, isr10, isr11,
                                isr12, isr13, isr14, nullptr,
                                isr16, isr17, isr18, isr19,
                                isr20, isr21, nullptr, nullptr,
                                nullptr, nullptr, nullptr, nullptr,
                                nullptr, nullptr, nullptr, nullptr,
                                isrTimer, isrKeyboard
                              );

    DescInterrupt32 descInt = DescInterrupt32::TrapKernelLong
                              (
                                NULL,
                                mngGdt.getCodeSegmentKernel()
                              );
    intf.foreach<isrPerform>(descInt);
    mngIdt.loadIdt();

};

void PrimaryInitialization::devicesInitialize()
{
    out::display = VGADisplay();
    picSingle = SingletonPic();

    Pic *pic = picSingle.getInstance();

    uint8 intn = mngIdt.getIrqInterruptStart();
    while(intn < mngIdt.getIrqInterruptStart() + Pic::numberIrq)
    {
        pic->enableIrq(intn++ - mngIdt.getIrqInterruptStart());
    }
};
