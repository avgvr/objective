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

#ifndef VGAGRAPHICS_H
#define VGAGRAPHICS_H

#include "VGADisplayInfo.h"
#include <TypeConverter.h>

typedef unsigned short dchar;

class VGADisplay
{
    private:
        uint16 xCurr;
        uint16 yCurr;
        dchar *videoMem;
        bool shift;
        char color;
        bool isBlink;
        bool isBright;

        void doShift();
        // 0 if passed char isn't escape sequence, else 1
        bool printChar(const char &c);
        void goNewLine();
        void goHorizontalTab();

    public:
        VGADisplay();
        VGADisplay(uint16 x, uint16 y);
        VGADisplay& operator<<(const char &c);
        VGADisplay& operator<<(const char *c);
        VGADisplay& operator<<(const uint32 number);
        VGADisplay& operator<<(const uint64 number);
        VGADisplay& operator++(int);
        void setShift(bool status);
        void setColor(char colorAttr);
        void setBlink(bool blinkAttr);
        void setBright(bool brigthAttr);
        void clearScreen();
};

// Global variables
namespace out
{
    extern VGADisplay display;
};

#endif
