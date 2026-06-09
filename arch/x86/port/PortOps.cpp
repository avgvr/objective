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

#include "PortOps.h"


void PortOps::outb(portadr port, regb value)
{
    asm("outb %b0, %w1"
        :
        : "a"(value), "Nd"(port)
        : "memory");
};

void PortOps::outw(portadr port, regw value)
{
    asm("out %w0, %w1"
        :
        : "a"(value), "Nd"(port)
        : "memory");
};

void PortOps::outl(portadr port, regl value)
{
    asm("out %k0, %w1"
        :
        : "a"(value), "Nd"(port)
        : "memory");
};

regb PortOps::inb(portadr port)
{
    regb res;

    asm("in %w1, %b0"
        : "=a"(res)
        : "Nd"(port)
        : "memory");

    return res;
};

regw PortOps::inw(portadr port)
{
    regw res;

    asm("in %w1, %w0"
        : "=a"(res)
        : "Nd"(port)
        : "memory");

    return res;
};

regl PortOps::inl(portadr port)
{
    regl res;

    asm("in %w1, %k0"
        : "=a"(res)
        : "Nd"(port)
        : "memory");

    return res;
};
