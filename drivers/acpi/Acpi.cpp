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

#include "Acpi.h"


const char *Rsdp::signatureString = "RSD PTR ";
const uint8 Rsdp::validFieldRevision = 2;
const uintptr Rsdp::romAreaStart = 0xe0000;
const uintptr Rsdp::romAreaEnd = 0xfffff;
const uintptr Rsdp::ebdaAreaStart = 0x80000;
const uintptr Rsdp::ebdaAreaEnd = 0x803ff;
const uint8 Rsdp::checksunFieldSize = 20;
const uint8 Entry::size = 4;
const uint8 sizeWithoutIcs = 44;

Rsdt *Rsdp::getRsdt() const
{
    Rsdt *rsdtPtr = (Rsdt *)(rsdtAddress);
    if(rsdtPtr == nullptr or checksumCheck()) return nullptr;
    else return rsdtPtr;
};

Xsdt *Rsdp::getXsdt() const
{
    Xsdt *xsdtPtr = (Xsdt *)(xsdtAddress);
    if(xsdtPtr == nullptr or checksumCheck()) return nullptr;
    else return xsdtPtr;
};


const uint32 DescHeader::getLength() const
{
    return length;
};

const Entry *Rsdt::getEntry(uint8 index) const
{
    uint32 entriesAmount = (header.getLength() - sizeof(DescHeader)) / Entry::size;

    if(index >= entriesAmount 
        or header.checksumCheck()) return nullptr;
    else return ((const Entry **)&entries)[index];
};

uint8 Rsdp::checksumCheck() const
{
    uint8 check = 0;
    for(uint32 offset = 0; offset < checksunFieldSize; offset++)
    {
        check += *(uint8 *)((uintptr)this + offset);
    }
    return check;
};

uint8 DescHeader::checksumCheck() const
{
    uint8 check = 0;
    for(uint32 offset = 0; offset < length; offset++)
    {
        check += *(uint8 *)((uintptr)this + offset);
    }
    return check;
};

const Ics *Madt::getIcs(uint8 index) const
{
    if(header.checksumCheck()) return nullptr;

    uint32 icsBytesRemain = header.getLength() - ::sizeWithoutIcs;
    uint16 indexAmass = 0;
    const Ics *icsPtr = (Ics *)(&icsBegin);

    while(icsBytesRemain > 0)
    {
        if(indexAmass == index) return icsPtr;
        else
        {
            icsBytesRemain -= icsPtr->getLength();
            indexAmass++;
            icsPtr = (Ics *)((uintptr)icsPtr + icsPtr->getLength());
        }
    }

    return nullptr;
};
