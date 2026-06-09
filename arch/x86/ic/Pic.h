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

#ifndef PIC_H
#define PIC_H

#include "../idt/Idt.h"
#include "../port/Operations"
#include "IPic.h"
#include <ISingleton.h>

class ICW1 : public ICW
{
    private:
        uint8 word;
    public:
        // Const part of ICW1 is high byte
        const static uint8 byteHigh = 0x10;
        // Inittialize bit and ICW4 recieve bit
        const static uint8 wordDefault = byteHigh | 0x01;
        ICW1();
        ICW1(const uint8 icw);
        bool isExpectIcw4();
        virtual const uint8 toByteMasterPic() const override final;
        virtual const uint8 toByteSlavePic() const override final;
        virtual const uint8 getNumber() const override final;
};

class ICW2 : public ICW
{
    private:
        uint8 lvtAddr;
    public:
        ICW2();
        ICW2(const uint8 icw);
        virtual const uint8 toByteMasterPic() const override final;
        virtual const uint8 toByteSlavePic() const override final;
        virtual const uint8 getNumber() const override final;
};

class ICW3 : public ICW
{
    private:
        const static uint8 irqTwoLine = 0x4;
        const static uint8 irqTwoLineBinary = 0x2;
        uint8 irqConnPrimaryPic;
        uint8 irqConnSecondaryPic;
    public:
        ICW3();
        ICW3(const uint8 icwMaster);
        virtual const uint8 toByteMasterPic() const override final;
        virtual const uint8 toByteSlavePic() const override final;
        virtual const uint8 getNumber() const override final;
};

class ICW4 : public ICW
{
    private:
        static const uint8 defaultWord = 0x01;
        uint8 word;
    public:
        ICW4();
        ICW4(const uint8 icw);
        virtual const uint8 toByteMasterPic() const override final;
        virtual const uint8 toByteSlavePic() const override final;
        virtual const uint8 getNumber() const override final;
};

class Pic
{
    protected:

        static const uint8 numberWords = 4;
        static const uint8 readIrr = 0xa;
        static const uint8 readIsr = 0xb;
        static const uint8 eoiCommand = 0x20;

    private:
        ICW1 icw1;
        ICW2 icw2;
        ICW3 icw3;
        ICW4 icw4;
        bool isIcwsCorrect(const ICW *icws[numberWords]);

    protected:
        Pic();
        Pic(const ICW *icws[numberWords]);
        Pic(const Pic &) = default;

    public:
        static const uint8 numberIrq = 16;
        // Valid equals from 0 to 7 and from 8 to 15
        // Return true if ISR not set, false is success
        bool enableIrq(uint8 irqNumber);
        void disableIrq(uint8 irqNumber);
        static uint8 getIsrMaster();
        static uint8 getIsrSlave();
        static uint8 getIrrMaster();
        static void sendEoi();
        // Set IMR to -xff
        static void disable();
};

// Be more careful with instance of Pic.
// When SingletonPic instance been deleted, Pic instance not be more exist
class SingletonPic : public Pic, public ISingleton<Pic>
{
    private:
        static bool isEntityAfter;
        bool isEntityFirst;

    public:
        SingletonPic();
        SingletonPic(const ICW *icws[numberWords]);
        virtual Pic *getInstance() override final;
};

#endif
