#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <Types.h>


class MemArea
{
    private:
        uintptr start, final;
    public:
        MemArea();
        MemArea(const uintptr start, const uintptr final);
        uintptr begin();
        uintptr end();
        bool valid();

        typedef uintptr iterator;
};

class StringOperations
{
    public:
        // Return the starting address where is sequence is allocated
        // else NULL
        static void *findInMemory(const char *sequence, MemArea memory);
        // Return size of ASCIZ sequence without 0-symbol, NULL on failure
        static uint32 sequenceSize(const char *chars);
};

#endif
