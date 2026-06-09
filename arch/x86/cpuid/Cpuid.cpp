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

#include "Cpuid.h"
#include <MemoryOperations.h>

bool cpuid::cpuidOutRegs::isNull()
{
    if(eax and ebx and ecx and edx) return false;
    else return true;
}

void cpuid::acquireInformation(cpuid::BasicInfo &cpuidRes)
{
    cpuidOutRegs regs = cpuid(cpuidRes.initVal);

    cpuidRes.maxCmdForBasicInfo = regs.eax;
    uint32 seqRegs[3] = {regs.ebx, regs.edx, regs.ecx};
    for(uint8 i = 0; i <= 3; i++)
    {
        MemoryOperations::copy(
            &seqRegs[i],
            cpuidRes.manufacturerString + sizeof(uint32) * i,
            sizeof(uint32));
    }
    cpuidRes.manufacturerString[12] = '\0';

}

cpuid::cpuidOutRegs cpuid::cpuid(uint32 eax)
{
    cpuidOutRegs out;
    asm inline("mov %%eax, %[initValue]\n"
               "cpuid"
               : "=eax" (out.eax), "=ebx" (out.ebx), "=ecx" (out.ecx),
               "=edx" (out.edx)
               : [initValue] "r" (eax));
    return out;
}

cpuid::cpuidOutRegs cpuid::cpuid(uint32 eax, uint32 ecx)
{
    cpuidOutRegs out;
    asm inline("mov %%eax, %[initValue]\n"
               "mov %%ecx, %[addiValue]\n"
               "cpuid"
               : "=eax" (out.eax), "=ebx" (out.ebx), "=ecx" (out.ecx),
               "=edx" (out.edx)
               : [initValue] "r" (eax), [addiValue] "r" (ecx));
    return out;
}

void cpuid::acquireInformation(cpuid::VersionInfo &cpuidRes)
{
    cpuidOutRegs regs = cpuid(cpuidRes.initVal);

    cpuidRes.ecx = regs.ecx;
    cpuidRes.edx = regs.edx;

    cpuidRes.eax.steppingId = regs.eax;
    cpuidRes.eax.modelId = regs.eax >> 4;
    cpuidRes.eax.familyId = regs.eax >> 8;
    cpuidRes.eax.processorType = regs.eax >> 12;
    cpuidRes.eax.extendedModelId = regs.eax >> 16;
    cpuidRes.eax.extendedFamilyId = regs.eax >> 20;
}

void cpuid::acquireInformation(cpuid::ExtendedMaxInputValue &cpuidRes)
{
    cpuidOutRegs regs = cpuid(cpuidRes.initVal);

    cpuidRes.eax = regs.eax;
};

void cpuid::acquireInformation(cpuid::ExtendedAddressSize &cpuidRes)
{
    cpuid::ExtendedMaxInputValue extendedMaxValDetails;
    acquireInformation(extendedMaxValDetails);

    if(extendedMaxValDetails.eax >= cpuidRes.initVal)
    {
        cpuidOutRegs regs = cpuid(cpuidRes.initVal);
        cpuidRes.eax.physicalAddressBits = regs.eax;
        cpuidRes.eax.linearAddressBits = regs.eax >> 8;
        cpuidRes.isWbnoinvdAvailable = regs.ebx >> 9;
    }
    else cpuidRes.isValid = false;
};

uint8 cpuid::VersionInfo::getFamilyId()
{
    if(eax.familyId != 0x0F)
    {
        return eax.familyId;
    }
    else return eax.extendedFamilyId + eax.familyId;
};

uint8 cpuid::VersionInfo::getModelId()
{
    if(eax.familyId == 0x06 or eax.familyId == 0x0F)
    {
        return (static_cast<uint8>(eax.extendedModelId) << 4) +
            eax.modelId;
    }
    else return eax.modelId;
};

cpuid::Executor::Executor() : isCpuidAvailable(CPUIDCHK()) {};
