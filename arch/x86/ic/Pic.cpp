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

#include "Pic.h"


bool SingletonPic::isEntityAfter = false;

ICW1::ICW1(const uint8 icw)
{
    word = byteHigh | (icw & 0x0f);
};

ICW1::ICW1()
{
    word = wordDefault;
};

bool ICW1::isExpectIcw4()
{
    return word & IC4;
};

const uint8 ICW1::toByteMasterPic() const
{
    return word;
}

const uint8 ICW1::toByteSlavePic() const
{
    return word;
}

const uint8 ICW1::getNumber() const
{
    return 1;
};

ICW2::ICW2()
{
    lvtAddr = mngIdt.getIrqInterruptStart();
};

ICW2::ICW2(const uint8 icw)
{
    if(icw >= 0xff - Pic::numberIrq) lvtAddr = MngIdt::firstSystemIntNumber;
    else lvtAddr = icw;
};

const uint8 ICW2::getNumber() const
{
    return 2;
};

const uint8 ICW2::toByteMasterPic() const
{
    if(lvtAddr >= 0xff - Pic::numberIrq) return MngIdt::firstSystemIntNumber;
    else return lvtAddr;
};

const uint8 ICW2::toByteSlavePic() const
{
    if(lvtAddr >= 0xff - 8) return MngIdt::firstSystemIntNumber + 8;
    else return lvtAddr + 8;
};

ICW3::ICW3()
{
    irqConnPrimaryPic = irqTwoLine;
    irqConnSecondaryPic = irqTwoLineBinary;
};

ICW3::ICW3(const uint8 icwMaster)
{
    uint8 oneAmount = 0;
    uint8 icwMasterCpy = icwMaster;

    while(icwMasterCpy)
    {
        oneAmount += icwMasterCpy & 0x01;
        icwMasterCpy >>= 1;
    }

    if(oneAmount > 1)
    {
        *this = ICW3();
    }
    else
    {
        irqConnPrimaryPic = icwMaster;

        uint8 icwMasterBinary = 0;
        icwMasterCpy = icwMaster;

        while(icwMasterCpy)
        {
            icwMasterBinary++;
            icwMasterCpy >>= 1;
        }
        irqConnSecondaryPic = icwMasterBinary;
    }
};


const uint8 ICW3::toByteMasterPic() const
{
    return irqConnPrimaryPic;
};

const uint8 ICW3::toByteSlavePic() const
{
    return irqConnSecondaryPic;
};

const uint8 ICW3::getNumber() const
{
    return 3;
};

ICW4::ICW4()
{
    word = defaultWord;
};

ICW4::ICW4(uint8 icw)
{
    word = icw & 0x1f;
};

const uint8 ICW4::toByteMasterPic() const
{
    return word;
};

const uint8 ICW4::toByteSlavePic() const
{
    return word;
};

const uint8 ICW4::getNumber() const
{
    return 4;
};

bool Pic::isIcwsCorrect(const ICW *icws[numberWords])
{
    for(uint8 idx = 0; idx < numberWords; idx++)
    {
        const ICW *currIcw = icws[idx];
        if(currIcw->getNumber() != idx + 1)
        {
            return false;
        }
    }

    return true;
};

void Pic::disableIrq(uint8 irqNumber)
{
    bool isMasterPic = irqNumber < 8;
    uint8 irqBits = 0x1 << (isMasterPic ? irqNumber : irqNumber - 8);
    uint8 imr = PortOps::inb(isMasterPic ? PICM_DATA : PICS_DATA);

    uint8 port = isMasterPic ? PICM_DATA : PICS_DATA;
    PortOps::outb(port, imr | irqBits);
};

bool Pic::enableIrq(uint8 irqNumber)
{
    bool isMasterPic = irqNumber < 8;
    uint8 irqBits = ~(0x1 << (isMasterPic ? irqNumber : irqNumber - 8));
    uint8 imr = PortOps::inb(isMasterPic ? PICM_DATA : PICS_DATA);

    if(!mngIdt.isInterruptSet(mngIdt.getIrqInterruptStart() + irqNumber))
    {
        // ISR not set in IDT
        return true;
    }

    uint8 port = isMasterPic ? PICM_DATA : PICS_DATA;
    PortOps::outb(port, imr & irqBits);

    return false;
};

void Pic::disable()
{
    // Stop recive interrupts
    PortOps::outb(PICM_DATA, 0xff);
    PortOps::outb(PICS_DATA, 0xff);
};

Pic::Pic(const ICW *icws[numberWords])
{
    if(!isIcwsCorrect(icws))
    {
        *this = Pic();
        return;
    }

    // Set ICW1 to command registers
    PortOps::outb(PICM_CTRL, icws[NULL]->toByteMasterPic());
    PortOps::outb(PICS_CTRL, icws[NULL]->toByteSlavePic());

    for(uint8 idx = 1; idx < numberWords; idx++)
    {
        const ICW *currIcw = icws[idx];

        if((icws[0]->toByteMasterPic() & IC4) or idx < 3)
        {
            PortOps::outb(PICM_DATA, currIcw->toByteMasterPic());
            PortOps::outb(PICS_DATA, currIcw->toByteSlavePic());
        }
    }

    // Clear data registers
    PortOps::outb(PICM_DATA, 0x0);
    PortOps::outb(PICS_DATA, 0x0);

    // Set properties
    icw1 = ICW1(*(ICW1 *)icws[0]);
    icw2 = ICW2(*(ICW2 *)icws[1]);
    icw2 = ICW2(*(ICW2 *)icws[2]);
    icw2 = ICW2(*(ICW2 *)icws[3]);

    // Disable interrupt recieve
    Pic::disable();
};

Pic::Pic()
{
    ICW1 icw1;
    ICW2 icw2;
    ICW3 icw3;
    ICW4 icw4;

    const ICW *icws[numberWords] = {&icw1, &icw2, &icw3, &icw4};
    *this = Pic(icws);
};

uint8 Pic::getIsrMaster()
{
    PortOps::outb(PICM_CTRL, readIsr);
    return PortOps::inb(PICM_CTRL);
};

uint8 Pic::getIsrSlave()
{
    PortOps::outb(PICS_CTRL, readIsr);
    return PortOps::inb(PICS_CTRL);
};

void Pic::sendEoi()
{
    // Recognize is the interrupt running and on which PIC is it
    // Further, check AEOI bit in ICW4
    // Send EOI if AEOI bit not set
    uint16 isrFull = (uint16)getIsrSlave() << Pic::numberIrq | (uint16)getIsrMaster();
    bool intOnMaster = (isrFull & 0x00ff) > 0;
    bool intOnSlave = (isrFull & 0xff00) > 0;

    // Interrupt not executed - EOI not send
    if(intOnMaster == 0 and intOnSlave == 0) return;
    // Interrupt expected only on master PIC
    else if(intOnMaster and !intOnSlave)
    {
        PortOps::outb(PICM_CTRL, eoiCommand);
    }
    // Interrupt expected only on slave PIC
    else if(!intOnMaster and intOnSlave)
    {
        PortOps::outb(PICS_CTRL, eoiCommand);
    }
};

SingletonPic::SingletonPic() : Pic()
{
    if(isEntityAfter == false)
    {
        isEntityFirst = true;
        isEntityAfter = true;
    }
    else
    {
        isEntityFirst = false;
    }
};

SingletonPic::SingletonPic(const ICW *icws[numberWords]) : Pic(icws)
{
    if(isEntityAfter == false)
    {
        isEntityFirst = true;
        isEntityAfter = true;
    }
    else
    {
        isEntityFirst = false;
    }
};

Pic *SingletonPic::getInstance()
{
    if(isEntityFirst) return (Pic *)this;
    return nullptr;
};
