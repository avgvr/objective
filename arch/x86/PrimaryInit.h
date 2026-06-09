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

#include "gdt/Gdt.h"
#include "io/VGADisplay.h"
#include "idt/Idt.h"
#include "ic/Pic.h"
#include <containers/Tuple.h>


struct InterruptFrame
{
#ifdef X86_64
    uint64 ip;
    uint64 cs;
    uint64 flags;
#else
    uint32 ip;
    uint32 cs;
    uint32 flags;
#endif
};

using isr = void (*)(const InterruptFrame *frame);
using isrExc= void (*)(const InterruptFrame *frame, uword errorCode);
using null = decltype(nullptr);
using InterruptFuncs = parameters<repeat<8, isr>::toList,
                        isrExc,
                        null,
                        repeat<5, isrExc>::toList,
                        null,
                        isr,
                        isrExc,
                        repeat<3, isr>::toList,
                        isrExc,
                        repeat<10, null>::toList,
                        repeat<2, isr>::toList
                        >::submitTo<Tuple>;
struct isrPerform
{
    static void perform(isr funp, DescInterrupt32 &desc);
    static void perform(isrExc funp, DescInterrupt32 &desc);
    static void perform(null null, DescInterrupt32 &desc);
};

class PrimaryInitialization
{
    private:
        SingletonPic picSingle;
    protected:
        virtual void processorInitialize();
        virtual void devicesInitialize();
};
