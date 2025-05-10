#include "SecondaryInit.h"

void SecondaryInitialization::processorInitialize()
{
    PrimaryInitialization::processorInitialize();

    Segments();
};

void SecondaryInitialization::devicesInitialize()
{
    PrimaryInitialization::devicesInitialize();
};
