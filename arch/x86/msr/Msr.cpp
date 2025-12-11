#include "Msr.h"
#include "../cpuid/Cpuid.h"

Msr::Msr()
{
    cpuid::VersionInfo vinfo;
    cpuid::Executor executor;

    executor.cpuid(vinfo);

    msrSupport = vinfo.edx & cpuid::VersionInfo::EdxFlags::MSR;
};

bool Msr::rdmsr(MsrCommand *cmd)
{
    if(!msrSupport or cmd == nullptr) return true;

    uint64 rdmsrResult = 0;
    uint32 regs[2];
    asm(
        "mov %2, %%ecx\n\t"
        "rdmsr\n\t"
        "mov %%eax, %0\n\t"
        "mov %%edx, %1\n\t"
        : "=&r"(regs[0]), "=&r"(regs[1])
        : "r"(cmd->getCommand()));

    MemoryOperations::copy(regs, &rdmsrResult, sizeof(uint64));
    cmd->decode(rdmsrResult);

    return false;
};

bool Msr::wrmsr(MsrCommand *cmd)
{
    if(!msrSupport or cmd == nullptr) return true;

    uint32 regs[2];
    uint64 msr = cmd->getMsr();

    MemoryOperations::copy(&msr, regs, sizeof(uint64));

    asm(
        "mov %0, %%eax\n\t"
        "mov %1, %%edx\n\t"
        "mov %2, %%ecx\n\t"
        "wrmsr\n\t"
        : "=r"(regs[0]), "=r"(regs[1])
        : "r"(cmd->getCommand()));

    return false;
};

Ia32ApicBase::Ia32ApicBase()
{
    cpuid::Executor executor;
    cpuid::VersionInfo verInfo;
    cpuid::ExtendedAddressSize addrSize;
    executor.cpuid(verInfo);
    executor.cpuid(addrSize);

    if(verInfo.isValid
        and verInfo.edx & cpuid::VersionInfo::EdxFlags::PAE
        and !addrSize.isValid)
    {
        maxPhyAddr = 36;
    }
    else if(!addrSize.isValid)
    {
        maxPhyAddr = 32;
    }
    else if(addrSize.isValid)
    {
        maxPhyAddr = addrSize.eax.physicalAddressBits;
    }
};

uint32 Ia32ApicBase::getCommand()
{
    return 0x1b;
};

void Ia32ApicBase::decode(uint64 rdmsrResult)
{
    bspFlag = rdmsrResult & BSP_FLAG;
    enableFlag = rdmsrResult & APIC_ENABLE_FLAG;
    setBaseField(rdmsrResult);
};

uint64 Ia32ApicBase::getMsr()
{
    uint64 msr = 0;

    MemoryOperations::copy(&baseField, &msr, sizeof(uint64));
    msr |= bspFlag << 8;
    msr |= enableFlag << 11;

    return msr;
};

void Ia32ApicBase::setBaseField(uint64 fieldAddr)
{
    uint64 mask = BASE_FIELD_MASK_LOW | 
                    ((UINT64_HIGH<< (maxPhyAddr - 32)) ^ UINT64_HIGH);

    baseField = fieldAddr & mask;
};

void Ia32ApicBase::setBspFlag(bool flag)
{
    bspFlag = flag;
};

void Ia32ApicBase::setEnableFlag(bool flag)
{
    enableFlag = flag;
};
