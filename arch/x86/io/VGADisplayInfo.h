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
