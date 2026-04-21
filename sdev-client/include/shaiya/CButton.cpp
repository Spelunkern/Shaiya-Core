#include "CButton.h"
using namespace shaiya;

void CButton::Init(CButton* button)
{
    typedef void(__thiscall* LPFN)(CButton*);
    (*(LPFN)0x54FCE0)(button);
}

void CButton::Draw(CButton* button, int windowPosX, int windowPosY)
{
    typedef void(__thiscall* LPFN)(CButton*, int, int);
    (*(LPFN)0x551B40)(button, windowPosX, windowPosY);
}

BOOL CButton::IsActive(CButton* button, int unknown)
{
    typedef BOOL(__thiscall* LPFN)(CButton*, int);
    return (*(LPFN)0x54FEB0)(button, unknown);
}

BOOL CButton::IsPressed(CButton* button)
{
    typedef BOOL(__thiscall* LPFN)(CButton*);
    return (*(LPFN)0x550A10)(button);
}

void CButton::Create(
    CButton* button, int windowPosX, int windowPosY,
    int x, int y, int textureW, int textureH, int width, int height,
    int unknown, const char* fileName, int fileW, int fileH, int enabled,
    float a1, float b1, float c1, float d1,
    float a2, float b2, float c2, float d2,
    float a3, float b3, float c3, float d3,
    float a4, float b4, float c4, float d4,
    float a5, float b5, float c5, float d5,
    int unknown2)
{
    typedef void(__thiscall* LPFN)(
        CButton*, int, int, int, int, int, int, int, int, int,
        const char*, int, int, int,
        float, float, float, float,
        float, float, float, float,
        float, float, float, float,
        float, float, float, float,
        float, float, float, float,
        int);

    (*(LPFN)0x551860)(
        button, windowPosX, windowPosY, x, y, textureW, textureH, width, height,
        unknown, fileName, fileW, fileH, enabled,
        a1, b1, c1, d1,
        a2, b2, c2, d2,
        a3, b3, c3, d3,
        a4, b4, c4, d4,
        a5, b5, c5, d5,
        unknown2);
}

void CButton::Reset(CButton* button)
{
    typedef void(__thiscall* LPFN)(CButton*);
    (*(LPFN)0x54FE50)(button);
}
