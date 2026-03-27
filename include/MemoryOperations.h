#ifndef MEMORYOPERATIONS_H
#define MEMORYOPERATIONS_H

#include <Types.h>


class MemoryOperations
{
    public:
        // Return number of copy bytes
        static uint32 copy(void *source, void *destination, uint32 sizeBytes);
        // Return number of set bytes
        static uint32 set(void *field, uint8 value, uint32 sizeBytes);
};

#endif
