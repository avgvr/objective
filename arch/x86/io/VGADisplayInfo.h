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

#ifndef VGAINFOCLASS_H
#define VGAINFOCLASS_H

#include <Types.h>

#define TOBG(attr) attr << 4

class VGADisplayInfo
{
    public:
        const static uint32 videoMemAddr = 0xB8000;
        const static uint16 width = 80;
        const static uint16 height = 25;
        const static uint16 tabSize = 8;
        enum class colorAttr {
            black = 0,
            blue = 1,
            green = 2,
            cyan = 3,
            red = 4,
            magenta = 5,
            brown = 6,
            gray = 7
        };
};

#endif
