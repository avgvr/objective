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
