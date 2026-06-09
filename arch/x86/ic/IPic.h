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

#ifndef IPIC_H
#define IPIC_H

#include <Types.h>


// Defines
// ICW1

#define IC4 1
#define SNGL 2
#define ADI 4
#define LTIM 8

// ICW4

#define PM 1
#define AEOI 2
#define MSBUF 4
#define BUF 8
#define SFNM 16

// PIC

#define PICM_CTRL 0x20
#define PICM_DATA 0x21
#define PICS_CTRL 0xA0
#define PICS_DATA 0xA1

//-------------

class ICW
{
    public:
        virtual const uint8 toByteMasterPic() const = 0; 
        virtual const uint8 toByteSlavePic() const = 0;
        virtual const uint8 getNumber() const = 0;
};

#endif
