#include <Types.h>
#include <StringOperations.h>
#include "Ics.h"


typedef decltype(nullptr) nulltype;

class Gas
{
    private:
        const uint8 addressSpaceId;
        const uint8 registerBitWidth;
        const uint8 registerBitOffset;
        const uint8 accessSize;
        const uint64 address;
    public:
        Gas();
        Gas(const Gas &) = default;
} __attribute__((packed));

class DescHeader
{
    private:
        const char signature[4];
        const uint32 length;
        const uint8 revision;
        const uint8 checksum;
        const char oemId[6];
        const uint64 oemTableId;
        const uint32 oemRevision;
        const uint32 creatorId;
        const uint32 creatorRevision;
    public:
        DescHeader() = delete;
        DescHeader(const DescHeader &) = delete;
        const uint32 getLength() const;
        uint8 checksumCheck() const;
} __attribute__((packed));


class Entry
{
    public:
        // In bytes
        static const uint8 size;
    protected:
        const DescHeader header;
    public:
        Entry() = delete;
        Entry(const Entry &) = delete;
} __attribute__((packed));

class Fadt : public Entry
{
    private:
        const uint32 firmwareCtrl;
        const uint32 dsdt;
        const uint8 reserved;
        const uint8 preferredPmProfile;
        const uint16 sciInt;
        const uint32 smiCmd;
        const uint8 acpiEnable;
        const uint8 acpiDisable;
        const uint8 s4biosReq;
        const uint8 pstateCnt;
        const uint32 pm1AEvtBlk;
        const uint32 pm1BEvtBlk;
        const uint32 pm1ACntBlk;
        const uint32 pm1BCntBlk;
        const uint32 pm2CntBlk;
        const uint32 pmTmrBlk;
        const uint32 gpe0Blk;
        const uint32 gpe1Blk;
        const uint8 pm1EvtLen;
        const uint8 pm1CntLen;
        const uint8 pm2CntLen;
        const uint8 pmTmrLen;
        const uint8 gpe0BlkLen;
        const uint8 gpe1BlkLen;
        const uint8 gpe1Base;
        const uint8 cstCnt;
        const uint16 pLvl2Lat;
        const uint16 pLvl3Lat;
        const uint16 flushSize;
        const uint16 flushStride;
        const uint8 dutyOffset;
        const uint8 dutyWidth;
        const uint8 dayAlrm;
        const uint8 monAlrm;
        const uint8 century;
        const uint16 iapcBootArch;
        const uint8 reservedTwo;
        const uint32 flags;
        const Gas resetReg;
        const uint8 resetValue;
        const uint16 armBootArch;
        const uint8 minorRevision;
        const uint64 xFirmwareCtrl;
        const uint64 xDsdt;
        const Gas xPm1AEvtBlk;
        const Gas xPm1BEvtBlk;
        const Gas xPm1ACntBlk;
        const Gas xPm1BCntBlk;
        const Gas xPm2CntBlk;
        const Gas xPmTmrBlk;
        const Gas xGpe0Blk;
        const Gas xGpe1Blk;
        const Gas sleepControlReg;
        const Gas sleepStatusReg;
        const uint64 hypervisorVendorIdentity;
    public:
        Fadt() = delete;
        Fadt(const Fadt &) = delete;
} __attribute__((packed));

class Madt : Entry
{
    public:
        const static uint8 sizeWithoutIcs;
    private:
        const uint32 localIntControlerAddr;
        const uint32 flags;
        const uint8 icsBegin;
    public:
        Madt() = delete;
        Madt(const Madt &) = delete;
        const Ics *getIcs(uint8 index) const;
} __attribute__((packed));

class Rsdt
{
    private:
        const DescHeader header;
        const uint8 entries;
    public:
        Rsdt() = delete;
        Rsdt(const Rsdt &) = delete;
        const Entry *getEntry(uint8 index) const;
} __attribute__((packed, aligned(sizeof(uint32))));

class Xsdt
{
    private:
        const DescHeader header;
        const uint64 firstEntryAddr;
    public:
        Xsdt() = delete;
        Xsdt(const Xsdt &) = delete;
} __attribute__((packed));

class Rsdp
{
    public:
        const static uint8 validFieldRevision;
        // RSDP memory address areas in IA-PC systems
        const static char *signatureString;
        const static uintptr romAreaStart;
        const static uintptr romAreaEnd;
        const static uintptr ebdaAreaStart;
        const static uintptr ebdaAreaEnd;
   private:
        const static uint8 checksunFieldSize;
        const char signature[8];
        const uint8 checksum;
        const char oemId[6];
        const uint8 revision;
        const uint32 rsdtAddress;
        const uint32 length;
        const uint32 xsdtAddress;
        const uint8 extendedChecksum;
        const char reserved[3];
    public:
        Rsdp() = delete;
        Rsdp(const Rsdp &) = delete;
        Rsdt *getRsdt() const;
        Xsdt *getXsdt() const;
        uint8 checksumCheck() const;
};
