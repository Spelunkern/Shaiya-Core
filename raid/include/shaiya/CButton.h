#pragma once
#include <shaiya/include/common.h>
#include "common.h"

namespace shaiya
{
    #pragma pack(push, 1)
    struct CButton
    {
        void* vftable;      //0x00
        bool enabled;       //0x04
        PAD(1);
        bool mouseEnter;    //0x06
        PAD(9);
        D2D_POINT_2U pos;   //0x10
        D2D_POINT_2U size;  //0x18
        PAD(131);
        bool checked;       //0xA3
        PAD(1332);
        // 0x5D8
    };
    #pragma pack(pop)

    static_assert(sizeof(CButton) == 0x5D8);
}
