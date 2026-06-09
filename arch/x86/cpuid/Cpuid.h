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

#ifndef CPUID_H
#define CPUID_H
#include <Types.h>
#include <MemoryOperations.h>

// To declare cpuid::Executor::Executor()
#include "../../../kernel/details/Platform.h"


namespace cpuid
{

#ifdef __cplusplus
    extern "C" {
#endif
    /**
     * Existence check on cpuid command
     *
     * @return true if cpuid available, false otherwise
     */
    bool CPUIDCHK();
#ifdef __cplusplus
};
#endif

/**
 * Structure to keep results of cpuid execution command
 */
struct cpuidOutRegs
{
    uint32 eax;
    uint32 ebx;
    uint32 ecx;
    uint32 edx;
    /**
     * Check on null
     *
     * @return True if all fields are null, false otherwise
     */
    bool isNull();
};

/**
 * Perform cpuid instruction with only leaf
 *
 * @param initial value passes to eax register
 *
 * @return output registers (eax, ebx, ecx, edx)
 */
inline cpuidOutRegs cpuid(uint32 eax);


/**
 * Perform cpuid instruction with leaf and subleaf
 *
 * @param initial value passes to eax register
 * @param number of subleaf passes to ecx register
 *
 * @return output registers (eax, ebx, ecx, edx)
 */
inline cpuidOutRegs cpuid(uint32 eax, uint32 ecx);

/**
 * Data representation for cpuid command.
 * Used to inherits from classes which commands are passing only initial value
 *
 * @note Only for inheritance
 */
struct CmdWithNoSubleaf
{
    const uint32 initVal;
    // This field is always handle by validator function. See `valid`
    // function description below
    bool isValid;

    CmdWithNoSubleaf() = delete;

protected:
    CmdWithNoSubleaf(uint32 cmdPass) : initVal(cmdPass), isValid(true) {};
};

/**
 * This class used for additional value for subleaf
 *
 * @note Also for inheritance purposes only
 */
struct CmdWithSubleaf : public CmdWithNoSubleaf
{
    const uint32 subleaf;

    CmdWithSubleaf() = delete;

protected:
    CmdWithSubleaf(uint32 cmdp, uint32 subleafp) : CmdWithNoSubleaf(cmdp), 
        subleaf(subleafp){};
};

/**
 * Provides details for cpuid(0h) instruction
 */
struct BasicInfo : public CmdWithNoSubleaf
{
    constexpr static uint32 initialValue = 0;
    uint32 maxCmdForBasicInfo;
    char manufacturerString[13];

    BasicInfo() : CmdWithNoSubleaf(initialValue) {};
};

/**
 * Provides details for cpuid(1h) instruction
 */
struct VersionInfo : public CmdWithNoSubleaf
{
    constexpr static uint32 initialValue = 0;
    VersionInfo() : CmdWithNoSubleaf(initialValue) {};

    inline uint8 getFamilyId();
    inline uint8 getModelId();
private:
    struct eaxVersionInformation
    {
        uint8 steppingId : 4,
            modelId : 4,
            familyId : 4,
            processorType : 2,
            extendedModelId : 4,
            extendedFamilyId : 4;
    };

    struct ebxVersionInformation
    {
        uint8 brandIndex,
            lineSize,
            maxNumberId,
            initialApicId;
    };

public:
    eaxVersionInformation eax;
    ebxVersionInformation ebx;
    uint32 ecx, edx;

    // ECX register flags
    enum class EcxFlags : uint32
    {
        SSE3 = 1,
        TablePCLMULQDQ = (1 << 1),
        DTES64 = (1 << 2),
        MONITOR = (1 << 3),
        DS_CPL = (1 << 4),
        VMX = (1 << 5),
        SMX = (1 << 6),
        EIST = (1 << 7),
        TM2 = (1 << 8),
        SSSE3 = (1 << 9),
        SNXT_ID = (1 << 10),
        SDBG = (1 << 11),
        FMA = (1 << 12),
        CMPXCHG16B = (1 << 13),
        xTPR_UPDATECONTROL = (1 << 14),
        PDCM = (1 << 15),
        PCID = (1 << 17),
        DCA = (1 << 18),
        SSE4_1 = (1 << 19),
        SSE4_2 = (1 << 20),
        x2APIC = (1 << 21),
        MOVBE = (1 << 22),
        POPCNT = (1 << 23),
        TSC_DEADLINE = (1 << 24),
        AESNI = (1 << 25),
        XSAVE = (1 << 26),
        OSXSAVE = (1 << 27),
        AVX = (1 << 28),
        F16C = (1 << 29),
        RDRAND = (1 << 30),
    };

    // EDX register flags
    enum class EdxFlags : uint32
    {
        FPU = 1,
        VME = (1 << 1),
        DE = (1 << 2),
        PSE = (1 << 3),
        TSC = (1 << 4),
        MSR = (1 << 5),
        PAE = (1 << 6),
        MCE = (1 << 7),
        CX8 = (1 << 8),
        APIC = (1 << 9),
        SEP = (1 << 11),
        MTRR = (1 << 12),
        PGE = (1 << 13),
        MCA = (1 << 14),
        CMOV = (1 << 15),
        PAT = (1 << 16),
        PSE_36 = (1 << 17),
        PSN = (1 << 18),
        CLFSH = (1 << 19),
        DS = (1 << 21),
        ACPI = (1 << 22),
        MMX = (1 << 23),
        FXSR = (1 << 24),
        SSE = (1 << 25),
        SSE2 = (1 << 26),
        SS = (1 << 27),
        HTT = (1 << 28),
        TM = (1 << 29),
        PBE = static_cast<uint32>(1 << 31),
    };
};

inline uint32 operator&(uint32 r, VersionInfo::EdxFlags flag)
{
    return r & static_cast<uint32>(flag);
};

inline uint32 operator&(uint32 r, cpuid::VersionInfo::EcxFlags flag)
{
    return r & static_cast<uint32>(flag);
};

struct ExtendedAddressSize : public  CmdWithNoSubleaf
{
    constexpr static uint32 initialValue = 0x80000008;

    ExtendedAddressSize() : CmdWithNoSubleaf(initialValue){};

private:
    struct AddressSize
    {
        uint8 physicalAddressBits,
            linearAddressBits;
    };

public:
    AddressSize eax;
    bool isWbnoinvdAvailable;
};

struct ExtendedMaxInputValue : public CmdWithNoSubleaf
{
    constexpr static uint32 initialValue = 0x80000000;

    ExtendedMaxInputValue() : CmdWithNoSubleaf(initialValue){};

    uint32 eax;
};

/**
 * Getters are acquire information from cpuid instruction
 *     and then put in some cpuid command instance
 *
 * @param cpuid command which needs to acquire processor details by one of some
 *     commands
 */
void acquireInformation(BasicInfo &cpuidRes);
void acquireInformation(VersionInfo &cpuidRes);
void acquireInformation(ExtendedAddressSize &cpuidRes);
void acquireInformation(ExtendedMaxInputValue &cpuidRes);

/**
 * Validators are check filling correctness for some cpuid command instance
 *
 * @param[out] cpuid command instance (as ins)
 *
 * @result precised `ins.isValid` value
 */
bool validate(/*Reference command type*/);


};
#endif
