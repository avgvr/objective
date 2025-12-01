#include "Cpuinfo.h"


inline Cpuinfo::cpuidFunctionIndex operator++(Cpuinfo::cpuidFunctionIndex& command)
{
    command = Cpuinfo::cpuidFunctionIndex(int(command) + 1);
    switch(int(command))
    {
        case 0x8: command = Cpuinfo::DirectCacheAccessInfo; break;
        case 0xC: command = Cpuinfo::ExtendedStateInfo; break;
        case 0xE: command = Cpuinfo::RDTMonitoring; break;
        case 0x11: command = Cpuinfo::SGXInfo; break;
        case 0x13: command = Cpuinfo::TraceInfo; break;
        default: 
        {
            uint32 commandInt(command);
            if(commandInt > Cpuinfo::SOCInfo 
                        and commandInt < Cpuinfo::UnimplementedStart)
            {
                command = Cpuinfo::UnimplementedStart;
            }
            else if(commandInt > Cpuinfo::UnimplementedStart 
                        and commandInt < Cpuinfo::UnimplementedEnd)
            {
                command = Cpuinfo::UnimplementedEnd;
            }
            else if(commandInt > Cpuinfo::UnimplementedEnd
                        and commandInt < Cpuinfo::ExtendedCpuidStart)
            {
                command = Cpuinfo::ExtendedCpuidStart;
            }
            else if(commandInt > Cpuinfo::ExtendedCpuidEnd)
            {
                command = Cpuinfo::BasicInfo;
            }
            break;
        }
    };

    return command;
};

Cpuinfo::Cpuinfo()
{
    eax = ebx = ecx = edx = 0;
    cpuidAvailable = CPUIDCHK();
    isBrandStringSupported = false;
    maximumInputValue = maximumInputValueExtended = 0;
    displayModel = displayFamily = stepping = 0;
    brandIndex = clflushLineSize = maxNumberApicsIds = initialApicId = 0;
    maxNumberProcessorsIds = 0;
    currentCpuType = processorType::NotRead;
    serialNumber = 0;

    MemoryOperations::set(vendorString, '\0', vendorStringSize);
    MemoryOperations::set(brandString, '\0', brandStringSize);

    dataFilling(Cpuinfo::BasicInfo, Cpuinfo::ExtendedCpuidEnd);
};

bool Cpuinfo::isEaxValueCorrect(const cpuidFunctionIndex eaxInitValue)
{
    if(!cpuidAvailable) return false;

    else if(eaxInitValue > maximumInputValue
            and eaxInitValue < ExtendedCpuidStart) return false;

    else if(eaxInitValue > maximumInputValueExtended
                and eaxInitValue > ExtendedCpuidStart) return false;

    else if(eaxInitValue == 0x8
            or eaxInitValue == 0xC
            or eaxInitValue == 0xE
            or eaxInitValue == 0x11
            or eaxInitValue == 0x13) return false;

    else return true;
};

void Cpuinfo::setBasicData()
{
    maximumInputValue = eax;
        
    uint32 vendorStringInt[3] = {ebx, edx, ecx};

    MemoryOperations::copy(vendorStringInt, vendorString, sizeof(uint32) * 3);
};

void Cpuinfo::setVersionData()
{
    uint8 steppingId, modelId, familyId, modelIdExt, familyIdExt; 

    steppingId = eax & 0xf;
    modelId = (eax >> 4) & 0xf;
    familyId = (eax >> 8) & 0xf;
    modelIdExt = (eax >> 16) & 0xf;
    familyIdExt = (eax >> 20) & 0xf;

    stepping = steppingId;
    currentCpuType = (processorType)((eax >> 12) & 0x3);

    if(familyId != 0xf)
    {
        displayFamily = familyId;
    }
    else displayFamily = familyIdExt + familyId;

    if(familyId == 0x6 or familyId == 0xf)
    {
        displayModel = (modelIdExt << 4) + modelId;
    }
    else displayModel = modelId;

    brandIndex = (uint8)ebx;
    clflushLineSize = ((uint8)(ebx >> 8)) << 3;
    maxNumberProcessorsIds = (ebx >> 16) & 0x0F;
    initialApicId = ebx >> 24;

    if(edx & Cpuinfo::HTT)
    {
        uint8 powerOfTwo = 1;
        for(; powerOfTwo < maxNumberProcessorsIds; powerOfTwo *= 2);
        if(maxNumberProcessorsIds == NULL) maxNumberApicsIds = NULL;
        else maxNumberApicsIds = powerOfTwo;
    }
};

void Cpuinfo::setInternalData()
{
    uint32 regs[4] = {eax, ebx, ecx, edx};
    uint8 idxDesc = 0;

    for(uint8 idx = 0; idx < 4; idx++)
    {
        uint32 &reg = regs[idx];
        for(uint8 bytes = 0; bytes < 4; bytes++)
        {
            internalInfoDescriptors[idxDesc++] = reg & 0xff;
            reg >>= 8;
        }
    }
};

void Cpuinfo::setSerialData()
{
    if(cpuid(VersionInfo) 
       || !(edx & Cpuinfo::PSN)
       || cpuid(SerialNumberInfo)) return;

    serialNumber = ((uint64)ecx) | (((uint64)edx) << 32);
};

void Cpuinfo::setExtendedBasicData()
{
    if(eax & Cpuinfo::ExtendedBasicInfo)
    {
        maximumInputValueExtended = eax;
        isBrandStringSupported = true;
    }
    else isBrandStringSupported = false;
};

void Cpuinfo::setBrandString(brandStringPart part)
{
    if((part == Cpuinfo::brandString_low or
       part == Cpuinfo::brandString_middle or
       part == Cpuinfo::brandString_hight)
            and isBrandStringSupported)
    {
        uint8 partUint = uint8(part);
        uint32 regs[4] = {eax, ebx, ecx, edx};

        MemoryOperations::copy(regs, &brandString[partUint], sizeof(uint32) * 4);
    }
};

bool Cpuinfo::decodeCpuidResult(const cpuidFunctionIndex eaxValue)
{
    switch(eaxValue)
    {
        case Cpuinfo::BasicInfo: setBasicData(); break;
        case Cpuinfo::VersionInfo: setVersionData(); break;
        case Cpuinfo::InternalInfo: setInternalData(); break;
        case Cpuinfo::SerialNumberInfo: setSerialData(); break;
        case Cpuinfo::MonitorMwaitInfo: break;
        case Cpuinfo::ThermalPowerInfo: break;
        case Cpuinfo::DirectCacheAccessInfo: break;
        case Cpuinfo::PerformanceMonitoring: break;
        case Cpuinfo::ClockInfo: break;
        case Cpuinfo::FrequencyInfo: break;
        case Cpuinfo::ExtendedBasicInfo: setExtendedBasicData(); break;
        case Cpuinfo::MoreFeatureInfo: break;
        case Cpuinfo::BrandStringLow: setBrandString(brandString_low); break;
        case Cpuinfo::BrandStringMiddle: setBrandString(brandString_middle); break;
        case Cpuinfo::BrandStringHight: setBrandString(brandString_hight); break;
        case Cpuinfo::CacheInfo: break;
        case Cpuinfo::MiscFeatureInfo: break;
        case Cpuinfo::AddressSize: break;
        default: return true; break;
   };

   return false;
};

bool Cpuinfo::decodeCpuidCommand(const cpuidFunctionIndex eaxValue)
{
    if(cpuid(eaxValue))
    {
       return true; 
    }
    else
    {
       return decodeCpuidResult(eaxValue);  
    }
};

bool Cpuinfo::cpuid(const cpuidFunctionIndex eaxValue)
{
    // Clear registers
    eax = ebx = ecx = edx = 0;

    if(!isEaxValueCorrect(eaxValue)
            or isCommandSupportSubLeaf(eaxValue)) return true;

    asm("movl %[eaxValue], %%eax\n\t"
        "movl %0, %%eax\n\t"
        "cpuid\n\t"
        "movl %%eax, %0\n\t"
        "movl %%ebx, %1\n\t"
        "movl %%ecx, %2\n\t"
        "movl %%edx, %3\n\t"
        : "=r" (eax), "=r" (ebx), "=r" (ecx), "=r" (edx)
        : [eaxValue] "r" (eaxValue));

    return false;
};

bool Cpuinfo::cpuid(const cpuidFunctionIndex eaxValue, uint32 ecxValue)
{
    // Clear registers
    eax = ebx = ecx = edx = 0;

    if(!isEaxValueCorrect(eaxValue) 
        or !isCommandSupportSubLeaf(eaxValue)) return true;

    asm("movl %[eaxValue], %%eax\n\t"
        "movl %[ecxValue], %%ecx\n\t"
        "cpuid\n\t"
        "movl %%eax, %0\n\t"
        "movl %%ebx, %1\n\t"
        "movl %%ecx, %2\n\t"
        "movl %%edx, %3\n\t"
        : "=r" (eax), "=r" (ebx), "=r" (ecx), "=r" (edx)
        : [eaxValue] "r" (eaxValue), [ecxValue] "r" (ecxValue));

    return false;
};

bool Cpuinfo::isCommandSupportSubLeaf(const cpuidFunctionIndex eaxValue)
{
    switch(eaxValue)
    {
        case DeterministicCacheInfo: return true; break;
        case FeatureInfo: return true; break;
        case ExtendedTopologyInfo: return true; break;
        case ExtendedStateInfo: return true; break;
        case RDTMonitoring: return true; break;
        case AllocationInfo: return true; break;
        case SGXInfo: return true; break;
        case TraceInfo: return true; break;
        case SOCInfo: return true; break;
        default: return false; break;
    };
};

const char* Cpuinfo::getVendorString()
{
    const char* vendorStringInvalid = "VendorStringInvalid";

    if(maximumInputValue != NULL) return (const char *)vendorString;
    else return vendorStringInvalid;
};

void Cpuinfo::dataFilling(cpuidFunctionIndex startValue, 
                          cpuidFunctionIndex endValue)
{
    cpuidFunctionIndex command = startValue;
    uint32 ecx = 0;

    do{
        if(isEaxValueCorrect(command))
        {
            if(isCommandSupportSubLeaf(command))
            {
                cpuid(command, ecx);
            }
            else decodeCpuidCommand(command);
        }
    }while(++command <= endValue and command != Cpuinfo::BasicInfo);
};

cpuidOutputRegisters Cpuinfo::getRegs()
{
    return {eax, ebx, ecx, edx};
};

uint8 Cpuinfo::getDisplayFamily()
{
   return displayFamily;
};

uint8 Cpuinfo::getDisplayModel()
{
    return displayModel;
};

uint8 Cpuinfo::getStepping()
{
    return stepping;
};

Cpuinfo::processorType Cpuinfo::getProcessorType()
{
    return currentCpuType;
};

uint8 Cpuinfo::getMaxFunctionIndex()
{
    return maximumInputValue;
}; 

uint32 Cpuinfo::getMaxFunctionIndexExtended()
{
    return maximumInputValueExtended;
};

const char* Cpuinfo::getBrandString()
{
    return (const char*)brandString;
};

uint8* Cpuinfo::getInternalDescriptors()
{
    return internalInfoDescriptors;
};
