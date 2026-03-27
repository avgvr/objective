
#include "../arch/SecondaryInit.h"
#include "../arch/x86/ic/Pic.h"


void empty() {};

void main()
{
    SecondaryInitialization init;

    init.processorInitialize();
    init.devicesInitialize();

    out::display << "Hello, World!\n";

    asm("sti");
    while(true);
};
