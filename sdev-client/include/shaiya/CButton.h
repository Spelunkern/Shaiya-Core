#pragma once
#include <shaiya/include/common.h>
#include "common.h"

namespace shaiya
{
#pragma pack(push, 1)
    struct CButton
    {
        void* vftable;      // 0x00
        bool enabled;       // 0x04
        PAD(1);
        bool mouseEnter;    // 0x06
        PAD(9);
        D2D_POINT_2U pos;   // 0x10
        D2D_SIZE_U size;    // 0x18
        PAD(131);
        bool checked;       // 0xA3
        PAD(1332);

        static void Init(CButton* button);
        static void Draw(CButton* button, int windowPosX, int windowPosY);
        static BOOL IsPressed(CButton* button);
        static BOOL IsActive(CButton* button, int unknown);
        static void Create(
            CButton* button, int windowPosX, int windowPosY,
            int x, int y, int textureW, int textureH, int width, int height,
            int unknown, const char* fileName, int fileW, int fileH, int enabled,
            float a1, float b1, float c1, float d1,
            float a2, float b2, float c2, float d2,
            float a3, float b3, float c3, float d3,
            float a4, float b4, float c4, float d4,
            float a5, float b5, float c5, float d5,
            int unknown2);
        static void Reset(CButton* button);
    };
#pragma pack(pop)

    static_assert(sizeof(CButton) == 0x5D8);
}
