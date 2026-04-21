#include <util/util.h>
#include <shaiya/include/common/ItemEffect.h>
#include <shaiya/include/network/game/outgoing/0200.h>
#include <shaiya/include/network/game/outgoing/0400.h>
#include "include/main.h"
#include "include/shaiya/CItem.h"
#include "include/shaiya/CNpcData.h"
#include "include/shaiya/CObject.h"
#include "include/shaiya/CUser.h"
#include "include/shaiya/ItemInfo.h"
#include "include/shaiya/NpcGateKeeper.h"
#include "include/shaiya/UserHelper.h"
using namespace shaiya;

namespace item_effect
{
    /// <summary>
    /// Adds support for additional item effects. Add new values to the enum and 
    /// then add more cases to this function.
    /// </summary>
    int hook(CUser* user, CItem* item, ItemEffect itemEffect, int bag, int slot)
    {
        switch (itemEffect)
        {
        case ItemEffect::ItemAbilityTransfer:
            return packet_gem::use_item_ability_transfer(user, bag, slot);
        case ItemEffect::TownMoveScroll:
        {
            // ReqVg stores the NpcTypeID of the gatekeeper used by this scroll.
            auto gateKeeperId = item->info->reqVg;
            if (!gateKeeperId)
                return 0;

            auto gateKeeper = CNpcData<NpcGateKeeper*>::GetNpc(2, gateKeeperId);
            if (!gateKeeper)
                return 0;

            auto index = user->savePosUseIndex;
            if (index < 0 || index > 2)
                return 0;

            auto mapId = gateKeeper->gates[index].mapId;
            auto x = gateKeeper->gates[index].x;
            auto y = gateKeeper->gates[index].y;
            auto z = gateKeeper->gates[index].z;
            UserHelper::SetMovePosition(user, mapId, x, y, z, UserMovePosType::TownMoveScroll, 5000);

            GameUserItemCastOutgoing outgoing{};
            outgoing.objectId = user->id;
            CObject::PSendViewCombat(user, &outgoing, sizeof(GameUserItemCastOutgoing));
            return 1;
        }
        default:
            return 0;
        }
    }

}

unsigned u0x47469F = 0x47469F;
unsigned u0x474690 = 0x474690;
void __declspec(naked) naked_0x47468A()
{
    __asm
    {
        pushad

        mov edx,[esp+0xB7C]
        mov edi,[esp+0xB78]

        push edx // slot
        push edi // bag
        push ecx // effect
        push ebx // item
        push ebp // user
        call item_effect::hook
        add esp,0x14
        test eax,eax

        popad

        jne _0x47469F

        // original
        mov eax,[esp+0x40]
        test eax,eax
        jmp u0x474690

        _0x47469F:
        jmp u0x47469F
    }
}

void hook::item_effect()
{
    // CUser::ItemUse
    util::detour((void*)0x47468A, naked_0x47468A, 6);
}
