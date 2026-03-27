#include "../kernel/mem/Segments.h"
#include "../kernel/mem/Kheap.h"
#include <Types.h>

#if defined(__IAPC)
#include "x86/PrimaryInit.h"
#endif

class SecondaryInitialization : protected PrimaryInitialization
{
    public:
        void processorInitialize() override final;
        void devicesInitialize() override final;
};

