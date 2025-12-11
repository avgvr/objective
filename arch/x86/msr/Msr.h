// Tasks:
//         1) Implement msr write and save functions
//         2) Ia32ApicBase field

#include <MemoryOperations.h>

#ifndef MSR_H
#define MSR_H

// defines for Ia32ApicBase

#define BSP_FLAG 0x100
#define APIC_ENABLE_FLAG 0x800
#define BASE_FIELD_MASK_LOW 0xfffff000
#define UINT64_HIGH 0xffffffff00000000

//------------------------

class MsrCommand
{
    public:
        virtual uint32 getCommand() = 0;
        virtual void decode(uint64 rdmsrResult) = 0;
        virtual uint64 getMsr() = 0;
};

class Ia32ApicBase : public MsrCommand
{
    private:
        // Get through Cpuinfo instance
        uint8 maxPhyAddr;
        bool bspFlag, enableFlag;
        uint64 baseField;

    public:
        Ia32ApicBase();
        virtual uint32 getCommand() override final;
        virtual void decode(uint64 rdmrResult) override final;
        virtual uint64 getMsr() override final;
        void setBspFlag(bool flag);
        void setEnableFlag(bool flag);
        void setBaseField(uint64 fieldAddr);
};

class Msr
{
    private:
        bool msrSupport;
    public:
        Msr();
        // Return true if MSR not supported (CPUID.01H:EDX[5] = 0)
        bool rdmsr(MsrCommand *cmd);
        // Return value same as rdmsr method
        bool wrmsr(MsrCommand *cmd);
};

#endif
