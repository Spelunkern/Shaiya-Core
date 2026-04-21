#pragma once
#include "CButton.h"
#include "CMessage.h"
#include "CTexture.h"
#include "CWindow.h"

namespace shaiya
{
    #pragma pack(push, 1)
    struct CStatusMiniBar : CWindow
    {
        PAD(8);
        void* vftable;          //0x2C
        CTexture image1;        //0x30
        CTexture image2;        //0x40
        CTexture image3;        //0x50
        CTexture image4;        //0x60
        CTexture image5;        //0x70
        float hpBarLen;         //0x80
        float spBarLen;         //0x84
        float mpBarLen;         //0x88
        int v8C;                //0x8C
        float v90;              //0x90
        CButton pvpButton;      //0x94
        CTexture pvpImage;      //0x66C
        float pvpAlpha;         //0x67C
        DWORD lastMoveTime;     //0x680
        bool enabled;           //0x684
        PAD(3);
        CMessage* message;      //0x688
        void* v68C;             //0x68C
        void* v690;             //0x690
        void* v694;             //0x694
        D2D_SIZE_F size;        //0x69C
        D2D_SIZE_F size3;       //0x6A0
        D2D_SIZE_F size4;       //0x6A8
        D2D_POINT_2F namePos;   //0x6B0
        D2D_POINT_2F levelPos;  //0x6B8
        D2D_POINT_2F facePos;   //0x6C0
        D2D_POINT_2F hpBarPos;  //0x6C8
        D2D_POINT_2F hpPos;     //0x6D0
        D2D_POINT_2F mpBarPos;  //0x6D8
        D2D_POINT_2F mpPos;     //0x6E0
        D2D_POINT_2F spBarPos;  //0x6E8
        D2D_POINT_2F spPos;     //0x6F0
    };
    #pragma pack(pop)

    static_assert(sizeof(CStatusMiniBar) == 0x6F8);
}
