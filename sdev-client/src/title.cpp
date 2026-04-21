#include <string>
#include <unordered_map>
#include <windows.h>
#include <util/util.h>
#include "include/main.h"
#include "include/shaiya/CCharacter.h"
#include "include/shaiya/CDataFile.h"
#include "include/shaiya/CMonster.h"
#include "include/shaiya/CStaticText.h"
#include "include/shaiya/ItemInfo.h"
#include "include/shaiya/HexColor.h"
#include "include/shaiya/Static.h"
using namespace shaiya;

namespace title
{
    using ItemId = uint32_t;

    constexpr float chat_y_add = 1.75F;
    constexpr uint16_t kRainbowTitleRange = 11;
    std::unordered_map<CCharacter*, ItemId> userTitleItemIds;

    D3DCOLOR make_rgb_color(uint8_t red, uint8_t green, uint8_t blue)
    {
        return 0xFF000000
            | (static_cast<D3DCOLOR>(red) << 16)
            | (static_cast<D3DCOLOR>(green) << 8)
            | static_cast<D3DCOLOR>(blue);
    }

    D3DCOLOR get_rainbow_title_color()
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

    bool is_title_cloak(const ItemInfo* itemInfo)
    {
        if (!itemInfo)
            return false;

        if (itemInfo->reqIg != 1)
            return false;

        return itemInfo->type == std::to_underlying(RealType::Cloak)
            || itemInfo->type == std::to_underlying(RealType::FuryCloak);
    }

    D3DCOLOR get_title_color(uint16_t range)
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
        case kRainbowTitleRange:
            return get_rainbow_title_color();
        default:
            return std::to_underlying(HexColor::White);
        }
    }

    void reset(CCharacter* user)
    {
        userTitleItemIds.erase(user);

        if (user->title.text)
        {
            if (user->title.text->texture)
            {
                user->title.text->texture->Release();
                user->title.text->texture = nullptr;
            }

            Static::operator_delete(user->title.text);
            user->title.text = nullptr;
        }
    }

    bool ensure_text(CCharacter* user, ItemId itemId, const std::string& text)
    {
        auto cachedItemId = userTitleItemIds.find(user);
        if (user->title.text && cachedItemId != userTitleItemIds.end() && cachedItemId->second == itemId)
            return true;

        reset(user);

        user->title.text = CStaticText::Create(text.c_str());
        if (!user->title.text)
            return false;

        auto w = CStaticText::GetTextWidth(text.c_str());
        user->title.pointX = static_cast<int>(w * 0.5);
        userTitleItemIds[user] = itemId;
        return true;
    }

    void hook(CCharacter* user, float x, float y, float extrusion)
    {
        if (!g_showTitles)
        {
            reset(user);
            return;
        }

        auto cloakType = user->equipment.type[ItemSlot::Cloak];
        auto cloakTypeId = user->equipment.typeId[ItemSlot::Cloak];

        if (!cloakType)
        {
            reset(user);
            return;
        }

        auto itemInfo = CDataFile::GetItemInfo(cloakType, cloakTypeId);
        if (!itemInfo)
        {
            reset(user);
            return;
        }

        if (!is_title_cloak(itemInfo) || !itemInfo->name || !itemInfo->name[0])
        {
            reset(user);
            return;
        }

        auto itemId = (itemInfo->type * 1000) + itemInfo->typeId;
        std::string text(itemInfo->name);
        auto color = get_title_color(itemInfo->range);

        if (!ensure_text(user, itemId, text))
            return;

        auto posY = static_cast<int>(y - 30.0);
        auto posX = static_cast<int>(x - user->title.pointX);

        CStaticText::Draw(user->title.text, posX, posY, extrusion, color);
    }
}

unsigned u0x453E81 = 0x453E81;
void __declspec(naked) naked_0x453E7C()
{
    __asm
    {
        pushad
        pushfd

        sub esp,0xC
        fld dword ptr[esp+0x4C]
        fstp dword ptr[esp+0x8]

        fld dword ptr[esp+0x48]
        fstp dword ptr[esp+0x4]

        fld dword ptr[esp+0x44]
        fstp dword ptr[esp]

        push esi // user
        call title::hook
        add esp,0x10

        popfd
        popad

        // original
        mov eax,dword ptr ds:[0x22B69A8]
        jmp u0x453E81
    }
}

unsigned n0x4184CF = 0x4184CF;
unsigned u0x418312 = 0x418312;
void __declspec(naked) naked_0x41830D()
{
    __asm 
    {
        // monster->model
        cmp dword ptr[eax+0x74],0x0
        je _0x4184CF
        
        // original
        cmp dword ptr[esp+0x38],0x0
        jmp u0x418312

        _0x4184CF:
        jmp n0x4184CF
    }
}

unsigned u0x412765 = 0x412765;
void __declspec(naked) naked_0x41275F()
{
    __asm
    {
        fld dword ptr[title::chat_y_add]
        jmp u0x412765
    }
}

unsigned u0x59F0C8 = 0x59F0C8;
void __declspec(naked) naked_0x59F0C3()
{
    __asm
    {
        pushad

        push esi // user
        call title::reset
        add esp,0x4

        popad

        // original 
        cmp byte ptr[esp+0x14],0x0
        jmp u0x59F0C8
    }
}

void hook::title()
{
    util::detour((void*)0x453E7C, naked_0x453E7C, 5);
    // hide pets without a model
    util::detour((void*)0x41830D, naked_0x41830D, 5);
    // increase chat balloon height (1.5 to 1.75)
    util::detour((void*)0x41275F, naked_0x41275F, 6);
    // 0x507 packet method
    util::detour((void*)0x59F0C3, naked_0x59F0C3, 5);
}
