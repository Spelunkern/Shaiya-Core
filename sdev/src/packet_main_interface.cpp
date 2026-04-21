#include <algorithm>
#include <ranges>
#include <util/util.h>
#include <shaiya/include/network/TP_MAIN.h>
#include <shaiya/include/network/game/incoming/0200.h>
#include <shaiya/include/network/game/outgoing/0200.h>
#include "include/main.h"
#include "include/shaiya/BattlefieldMoveInfo.h"
#include "include/shaiya/CObject.h"
#include "include/shaiya/CUser.h"
#include "include/shaiya/CWorld.h"
#include "include/shaiya/MyShop.h"
#include "include/shaiya/SConnection.h"
#include "include/shaiya/UserHelper.h"
using namespace shaiya;

namespace packet_main_interface
{
    void handler_0x233(CUser* user)
    {
        // Battleground buttons.
        // The client only requests the move; the server decides the valid
        // battlefield by level and family, then uses the same 5s cast movement
        // path as town move scrolls. Debuffs intentionally do not block this.
        if (user->status == UserStatus::Death)
            return;

        if (user->logoutTime)
            return;

        auto it = std::ranges::find_if(g_battlefieldMoveData, BattlefieldLvInRange(user->level));
        if (it == g_battlefieldMoveData.end())
            return;

        auto family = std::clamp(static_cast<int>(user->family), 0, 3);
        auto& pos = it->mapPos[family];
        if (user->mapId == pos.mapId)
            return;

        if (!CWorld::GetZone(pos.mapId))
            return;

        CUser::CancelActionExc(user);
        MyShop::Ended(&user->myShop);

        user->savePosUseBag = 0;
        user->savePosUseSlot = 0;
        user->savePosUseIndex = 0;
        UserHelper::SetMovePosition(user, pos.mapId, pos.x, pos.y, pos.z, UserMovePosType::TownMoveScroll, 5000);

        GameUserItemCastOutgoing outgoing{};
        outgoing.objectId = user->id;
        CObject::PSendViewCombat(user, &outgoing, sizeof(GameUserItemCastOutgoing));
    }

    void handler(CUser* user, TP_MAIN* packet)
    {
        switch (packet->opcode)
        {
        case 0x233:
            handler_0x233(user);
            break;
        default:
            SConnection::Close(user, 9, 0);
            break;
        }
    }
}

unsigned u0x477999 = 0x477999;
void __declspec(naked) naked_0x47798E()
{
    __asm
    {
        pushad

        push ebx // packet
        push ebp // user
        call packet_main_interface::handler
        add esp,0x8

        popad

        jmp u0x477999
    }
}

void hook::packet_main_interface()
{
    // CUser::PacketMainInterface default case.
    util::detour((void*)0x47798E, naked_0x47798E, 6);
}
