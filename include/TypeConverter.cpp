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

#include "TypeConverter.h"

IntConverter::IntConverter()
{
    resetProps();
};

void IntConverter::resetProps()
{
    for(int i = 0; i < 21; i++) buffer[i] = '\0';
    index = bufferSize - 2;
};

char *IntConverter::intToChar(int n)
{
    resetProps();

    int number = n >> 31;   // n < 0 - n = -1, else n = 0
    number = (n ^ number) - number; // Clear sign bit

    bool isNegative = n < 0;

    writeNumberToBuffer(number);
    
    if(isNegative) buffer[index] = '-';

    return &buffer[index];
};

char *IntConverter::uintToChar(const uint32 n)
{
    resetProps();
    writeNumberToBuffer(n);
    
    return &buffer[++index];
};

char *IntConverter::uintToChar(const uint64 n)
{
    resetProps(); 
    writeNumberToBuffer(n);

    return &buffer[++index];
};

void IntConverter::writeNumberToBuffer(uint64 n)
{
    do {
        buffer[index--] = '0' + (n % 10);
        n /= 10;
    }while(n != 0);
};
