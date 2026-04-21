#include <windows.h>
#include <util/util.h>
#include "include/main.h"
#include "include/shaiya/CCharacter.h"
#include "include/shaiya/CDataFile.h"
#include "include/shaiya/HexColor.h"
#include "include/shaiya/ItemInfo.h"
#include "include/shaiya/Static.h"
using namespace shaiya;

namespace name_color
{
    constexpr uint16_t kRainbowNameRange = 11;

    D3DCOLOR make_rgb_color(uint8_t red, uint8_t green, uint8_t blue)
    {
        return 0xFF000000
            | (static_cast<D3DCOLOR>(red) << 16)
            | (static_cast<D3DCOLOR>(green) << 8)
            | static_cast<D3DCOLOR>(blue);
    }

    D3DCOLOR get_rainbow_name_color()
    {
        auto phase = (GetTickCount() / 450) % 6;
        auto step = static_cast<uint8_t>((GetTickCount() % 450) * 255 / 450);
        auto inverse = static_cast<uint8_t>(255 - step);

        switch (phase)
        {
        case 0:
            return make_rgb_color(255, step, 0);
        case 1:
            return make_rgb_color(inverse, 255, 0);
        case 2:
            return make_rgb_color(0, 255, step);
        case 3:
            return make_rgb_color(0, inverse, 255);
        case 4:
            return make_rgb_color(step, 0, 255);
        default:
            return make_rgb_color(255, 0, inverse);
        }
    }

    D3DCOLOR get_custom_color(uint16_t range)
    {
        switch (range)
        {
        case 1:
            return std::to_underlying(HexColor::Red);
        case 2:
            return std::to_underlying(HexColor::DodgerBlue);
        case 3:
            return std::to_underlying(HexColor::Green);
        case 4:
            return std::to_underlying(HexColor::Yellow);
        case 5:
            return std::to_underlying(HexColor::Orange);
        case 6:
            return std::to_underlying(HexColor::Purple);
        case 7:
            return std::to_underlying(HexColor::Pink);
        case 8:
            return std::to_underlying(HexColor::Cyan);
        case 9:
            return std::to_underlying(HexColor::Gold);
        case 10:
            return std::to_underlying(HexColor::Silver);
        case kRainbowNameRange:
            return get_rainbow_name_color();
        default:
            return 0;
        }
    }

    bool is_name_color_helmet(const ItemInfo* itemInfo)
    {
        if (!itemInfo)
            return false;

        switch (itemInfo->type)
        {
        case 16:
        case 31:
        case 72:
        case 87:
            return true;
        default:
            return false;
        }
    }

    HexColor get_mob_name_color(int mobLevel)
    {
        int gap = mobLevel - g_pPlayerData->level;
        if (gap >= 10)
            return HexColor::Gray;

        switch (gap)
        {
        case 9: case 8:
            return HexColor::Pink;
        case 7: case 6:
            return HexColor::Red;
        case 5: case 4:
            return HexColor::Orange;
        case 3: case 2:
            return HexColor::Yellow;
        case 1: case 0: case -1:
            return HexColor::Green;
        case -2: case -3:
            return HexColor::Blue;
        case -4: case -5:
            return HexColor::LightBlue;
        default:
            break;
        }

        return HexColor::White;
    }

    D3DCOLOR get_helmet_name_color(CCharacter* user)
    {
        if (!g_showNameColors)
            return 0;

        auto helmetType = user->equipment.type[ItemSlot::Helmet];
        auto helmetTypeId = user->equipment.typeId[ItemSlot::Helmet];

        auto itemInfo = CDataFile::GetItemInfo(helmetType, helmetTypeId);
        if (!itemInfo)
            return 0;

        if (!is_name_color_helmet(itemInfo) || !itemInfo->range)
            return 0;

        return get_custom_color(itemInfo->range);
    }
}

void __declspec(naked) naked_0x4E50D0()
{
    __asm
    {
        push ebx
        push edi
        push esi

        movzx eax,word ptr[esp+0x10]
        push eax
        call name_color::get_mob_name_color
        add esp,0x4

        pop esi
        pop edi
        pop ebx

        retn 0x4
    }
}

unsigned u0x453CE1 = 0x453CE1;
void __declspec(naked) naked_0x453CDB()
{
    __asm
    {
        push ebx
        push edi
        push esi

        push esi // user
        call name_color::get_helmet_name_color
        add esp,0x4
        test eax,eax

        pop esi
        pop edi
        pop ebx

        je original

        mov ebp,eax

        original:
        mov edi,dword ptr [esi+0x314]
        jmp u0x453CE1
    }
}

void hook::name_color()
{
    // mobs
    util::detour((void*)0x4E50D0, naked_0x4E50D0, 5);
    // users: apply helmet-driven name color immediately before char name draw,
    // after the stock admin/party/faction/PvP color decisions are finished.
    util::detour((void*)0x453CDB, naked_0x453CDB, 6);
}

