#include <Types.h>


class Ics
{
    protected:
        const uint8 type;
        const uint8 length;
    public:
        Ics() = delete;
        Ics(const Ics &) = delete;
        uint8 getLength() const;
};

class LapicStruct : public Ics
{
    private:
        const uint8 acpiProcUid;
        const uint8 apicId;
        const uint32 flags;
    public:
        LapicStruct() = delete;
        LapicStruct(const LapicStruct &) = delete;
};

class IoapicStruct : public Ics
{
    private:
        const uint8 ioapicId;
        const uint8 reserved;
        const uint32 ioapicAddr;
        const uint32 globalSysIntBase;
    public:
        IoapicStruct() = delete;
        IoapicStruct(const IoapicStruct &) = delete;
};

class IsoStruct : public Ics
{
    private:
        const uint8 bus;
        const uint8 source;
        const uint32 globalSysInt;
        const uint16 flags;
    public:
        IsoStruct() = delete;
        IsoStruct(const IsoStruct &) = delete;
};

class NmiSourceStruct: public Ics
{
    private:
        const uint16 flags;
        const uint32 globalSysInt;
    public:
        NmiSourceStruct() = delete;
        NmiSourceStruct(const NmiSourceStruct &) = delete;
};

class LapicNmiStruct : public Ics
{
    private:
        const uint8 acpiProcUid;
        const uint16 flags;
        const uint8 localApicLint;
    public:
        LapicNmiStruct() = delete;
        LapicNmiStruct(const LapicNmiStruct &) = delete;
};

class LapicAddrOverrrideStruct : public Ics
{
    private:
        const uint16 reserved;
        const uint64 lapicAddr;
    public:
        LapicAddrOverrrideStruct() = delete;
        LapicAddrOverrrideStruct(const LapicAddrOverrrideStruct &) = delete;
};

class IosapicStruct : public Ics
{
    private:
        const uint8 ioapicId;
        const uint8 reserved;
        const uint32 globSysIntBase;
        const uint64 iosapicAddr;
    public:
        IosapicStruct() = delete;
        IosapicStruct(const IosapicStruct &) = delete;
};

class LsapicStruct : public Ics
{
    private:
        const uint8 acpiProcId;
        const uint8 lsapicId;
        const uint8 lsapicUid;
        const uint8 reserved[3];
        const uint32 flags;
        const uint32 acpiProcUidValue;
        const uint8 acpiProcUidStringBegin;
    public:
        LsapicStruct() = delete;
        LsapicStruct(const LsapicStruct &) = delete;
};

class PlatformIntSourceStruct : public Ics
{
    private:
        const uint16 flags;
        const uint8 intType;
        const uint8 procId;
        const uint8 procEid;
        const uint8 iosapicVector;
        const uint32 globSysInt;
        const uint32 platformIntSourceFlags;
    public:
        PlatformIntSourceStruct() = delete;
        PlatformIntSourceStruct(const PlatformIntSourceStruct &) = delete;
};

class ProcLxapicStruct : public Ics
{
    private:
        const uint16 reserved;
        const uint32 xapicId;
        const uint32 flags;
        const uint32 acpiProcUid;
    public:
        ProcLxapicStruct() = delete;
        ProcLxapicStruct(const ProcLxapicStruct &) = delete;
};

class LocalXapicNmiStruct : public Ics
{
    private:
        const uint16 flags;
        const uint32 acpiProcUid;
        const uint8 localXapicLint;
        const uint8 reserved[3];
    public:
        LocalXapicNmiStruct() = delete;
        LocalXapicNmiStruct(const LocalXapicNmiStruct &) = delete;
};

class GiccStruct : public Ics
{
    private:
        const uint16 reservedBegin;
        const uint32 cpuInterfaceNumber;
        const uint32 acpiProcUid;
        const uint32 flags;
        const uint32 parkingProtocolVersion;
        const uint32 performanceIntGsiv;
        const uint64 parkedAddr;
        const uint64 physBaseAddr;
        const uint64 gicv;
        const uint64 gich;
        const uint32 vgicMaintenanceInt;
        const uint64 gicrBaseAddr;
        const uint64 mpidr;
        const uint8 ProcPowerEfficiencyClass;
        const uint8 reservedEnd;
        const uint16 speOverflowInterrupt;
    public:
        GiccStruct() = delete;
        GiccStruct(const GiccStruct &) = delete;
};

class GicdStruct : public Ics
{
    private:
        const uint16 reservedBegin;
        const uint32 gicId;
        const uint64 physBaseAddr;
        const uint32 sysVectorBase;
        const uint8 gicVersion;
        const uint8 reservedEnd[3];
    public:
        GicdStruct() = delete;
        GicdStruct(const GicdStruct &) = delete;
};

class GisMsiFrameStruct : public Ics
{
    private:
        const uint16 reserved;
        const uint32 gicMsiFrameId;
        const uint64 physBaseAddr;
        const uint32 flags;
        const uint16 spiCount;
        const uint16 spiBase;
    public:
        GisMsiFrameStruct() = delete;
        GisMsiFrameStruct(const GisMsiFrameStruct &) = delete;
};

class GicrStruct : public Ics
{
    private:
        const uint16 reserved;
        const uint64 discoveryRangeBaseAddr;
        const uint32 discoveryRangeLength;
    public:
        GicrStruct() = delete;
        GicrStruct(const GicrStruct &) = delete;
};

class ItsStruct : public Ics
{
    private:
        const uint16 reserved;
        const uint32 GicItsId;
        const uint64 physBaseAddr;
        const uint32 reservedEnd;
    public:
        ItsStruct() = delete;
        ItsStruct(const ItsStruct &) = delete;
};

class MprocWakeupStruct : Ics
{
    private:
        const uint16 mailBoxVersion;
        const uint32 reserved;
        const uint64 mailBoxAddr;
    public:
        MprocWakeupStruct() = delete;
        MprocWakeupStruct(const MprocWakeupStruct &) = delete;
};
