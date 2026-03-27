#ifndef PORT_H
#define PORT_H

#include <Types.h>


using regb = uint8;
using regw = uint16;
using regl = uint32;
using portadr = uint16;

class PortOps
{
    public:
        static void outb(portadr port, regb value);
        static void outw(portadr port, regw value);
        static void outl(portadr port, regl value);
        // Get byte from port
        static regb inb(portadr port);
        // Get word from port
        static regw inw(portadr port);
        // Get double word from port
        static regl inl(portadr port);
};

#endif
