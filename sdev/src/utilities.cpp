#include <cstddef>
#include <cstdint>
#include <cstring>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <util/util.h>
#include "include/main.h"

namespace
{
    // Cross-faction GM chat:
    // GM/admin users (admin level >= 0x0B) keep the original faction check.
    // Everyone below that level skips the original compare and can talk cross-faction.
    unsigned u0x42793E = 0x42793E;
    unsigned u0x427944 = 0x427944;
    void __declspec(naked) naked_0x427938()
    {
        __asm
        {
            cmp word ptr [ecx+0x5808],0x0B
            jl _cross_faction

            // original
            cmp dl,[ecx+0x12D]
            jmp u0x42793E

            _cross_faction:
            jmp u0x427944
        }
    }

    // Stack Fortune Bag drops on Inventory.
    // The original CE script offsets are valid for this ps_game.exe build.
    // We hook the same epilogue site right after the item-open routine.
    unsigned u0x46C284 = 0x46C284;
    unsigned u0x4685A0 = 0x4685A0;
    void __declspec(naked) naked_0x46C27E()
    {
        __asm
        {
            cmp dword ptr [esp+0x15C],0x473BD4
            jne _originalcode
            cmp eax,1
            jne _originalcode
            push eax
            push ecx
            push edx
            push ebx
            push esi
            push edi

            mov ebx,dword ptr [esp+0x1C]
            imul ebx,ebx,24
            add ebx,dword ptr [esp+0x2C]
            imul ebx,ebx,4
            add ebx,0x1C0
            mov edi,dword ptr [esp+0x20]
            add ebx,edi
            mov esi,dword ptr [ebx]

            push esi
            push edi
            call _func_get_bag_slot_next_identical_item

            test eax,eax
            je _exit

            mov edx,dword ptr [esp+0x2C]
            mov ebx,dword ptr [esp+0x1C]

            movzx ecx,ah
            push ecx
            movzx ecx,al
            push ecx

            push edx
            push ebx

            mov ecx,edi
            call u0x4685A0

        _exit:
            pop edi
            pop esi
            pop ebx
            pop edx
            pop ecx
            pop eax

        _originalcode:
            add esp,0x15C
            jmp u0x46C284

        _func_get_bag_slot_next_identical_item:
            push ecx
            push edx
            push ebx
            push esi
            push edi

            mov esi,dword ptr [esp+0x1C]
            mov edi,dword ptr [esp+0x18]

            cmp esi,0x400000
            jl _not_found
            cmp dword ptr [esi+0x30],0x400000
            jl _not_found
            cmp dword ptr [edi],0x571AA8
            jne _not_found
            mov eax,dword ptr [esi+0x30]
            cmp dword ptr [eax],1001
            jl _not_found
            cmp dword ptr [eax],150255
            jg _not_found

            mov al,1
            mov ah,0
            add edi,0x220

        _next_item:
            mov ecx,dword ptr [edi]
            test ecx,ecx
            je _check_slot
            cmp ecx,esi
            je _check_slot
            mov edx,dword ptr [ecx+0x30]
            cmp edx,dword ptr [esi+0x30]
            jne _check_slot
            movzx ebx,byte ptr [edx+0x4B]
            cmp bl,byte ptr [ecx+0x42]
            ja _success

        _check_slot:
            add edi,4
            inc ah
            cmp ah,24
            jb _next_item

            xor ah,ah
            inc al
            cmp al,5
            jbe _next_item

        _not_found:
            xor eax,eax

        _success:
            pop edi
            pop esi
            pop ebx
            pop edx
            pop ecx
            ret 8
        }
    }

    // Union leader summons the whole raid.
    // This build matches the original CE script offsets, so we mirror that
    // logic directly and only bypass the original comparison when the user is
    // the current union/raid leader.
    unsigned u0x44ECF0 = 0x44ECF0;
    unsigned u0x49E4E6 = 0x49E4E6;
    unsigned u0x49E4EA = 0x49E4EA;
    void __declspec(naked) naked_0x49E4E1()
    {
        __asm
        {
            call u0x44ECF0
            push eax
            push ecx

            mov eax,[edi+0x17F4]
            test eax,eax
            je _exit

            mov ecx,[eax+0x0C]
            cmp ecx,0
            jl _exit
            imul ecx,ecx,8
            add ecx,0x18
            mov ecx,[eax+ecx]
            test ecx,ecx
            je _exit
            cmp edi,ecx
            je _leader_success

        _exit:
            pop ecx
            pop eax
            jmp u0x49E4E6

        _leader_success:
            pop ecx
            pop eax
            jmp u0x49E4EA
        }
    }

    // Rune cutting fixes:
    // These mirror the tested CE scripts. When the user state at +0x1270 is 2,
    // skip the map/zone rejection path and continue through the success branch.
    unsigned u0x473DB7 = 0x473DB7;
    unsigned u0x473EDE = 0x473EDE;
    unsigned u0x474004 = 0x474004;
    unsigned u0x47419D = 0x47419D;
    unsigned u0x4742B6 = 0x4742B6;
    unsigned u0x47468A = 0x47468A;

    void __declspec(naked) naked_0x473DB0()
    {
        __asm
        {
            cmp dword ptr [ebp+0x1270],2
            je _allow_rune_cutting

            // original
            cmp dword ptr [eax+0x1A4],0
            jmp u0x473DB7

        _allow_rune_cutting:
            jmp u0x47468A
        }
    }

    void __declspec(naked) naked_0x473ED7()
    {
        __asm
        {
            cmp dword ptr [ebp+0x1270],2
            je _allow_rune_cutting

            // original
            cmp dword ptr [eax+0x1A4],0
            jmp u0x473EDE

        _allow_rune_cutting:
            jmp u0x47468A
        }
    }

    void __declspec(naked) naked_0x473FFD()
    {
        __asm
        {
            cmp dword ptr [ebp+0x1270],2
            je _allow_rune_cutting

            // original
            cmp dword ptr [eax+0x1A4],0
            jmp u0x474004

        _allow_rune_cutting:
            jmp u0x47468A
        }
    }

    void __declspec(naked) naked_0x474196()
    {
        __asm
        {
            cmp dword ptr [ebp+0x1270],2
            je _allow_rune_cutting

            // original
            cmp dword ptr [eax+0x1A4],6
            jmp u0x47419D

        _allow_rune_cutting:
            jmp u0x4742B6
        }
    }

    // Security: sanitize vulnerable strings before they are copied into SQL-bound packets.
    unsigned u0x431CBE = 0x431CBE;
    unsigned u0x431CE8 = 0x431CE8;
    unsigned u0x431E2D = 0x431E2D;
    unsigned u0x47A518 = 0x47A518;
    unsigned u0x480A68 = 0x480A68;
    unsigned u0x4905A9 = 0x4905A9;
    unsigned u0x49AA3F = 0x49AA3F;

    void __declspec(naked) naked_0x480A62()
    {
        __asm
        {
            // Commands: replace apostrophes to avoid SQL injection.
            cmp cl,0x27
            jne _copy_character
            mov cl,0x20

        _copy_character:
            mov byte ptr [edx+eax],cl
            inc eax
            test cl,cl
            jmp u0x480A68
        }
    }

    void __declspec(naked) naked_0x4905A3()
    {
        __asm
        {
            // Character create: replace SQL/control-sensitive name characters.
            cmp dl,0x27
            je _replace_character
            cmp dl,0x22
            je _replace_character
            cmp dl,0x5B
            je _replace_character
            cmp dl,0x5D
            jne _copy_character

        _replace_character:
            mov dl,0x20

        _copy_character:
            mov byte ptr [ecx+esi],dl
            inc esi
            test dl,dl
            jmp u0x4905A9
        }
    }

    void __declspec(naked) naked_0x47A512()
    {
        __asm
        {
            // Character rename: replace SQL/control-sensitive name characters.
            cmp cl,0x27
            je _replace_character
            cmp cl,0x22
            je _replace_character
            cmp cl,0x5B
            je _replace_character
            cmp cl,0x5D
            jne _copy_character

        _replace_character:
            mov cl,0x20

        _copy_character:
            mov byte ptr [edx+eax],cl
            inc eax
            test cl,cl
            jmp u0x47A518
        }
    }

    void __declspec(naked) naked_0x431CB8()
    {
        __asm
        {
            // Guild name: replace quote characters before DB packet creation.
            cmp cl,0x27
            je _replace_character
            cmp cl,0x22
            jne _copy_character

        _replace_character:
            mov cl,0x20

        _copy_character:
            mov byte ptr [edx+eax],cl
            inc eax
            test cl,cl
            jmp u0x431CBE
        }
    }

    void __declspec(naked) naked_0x431CE2()
    {
        __asm
        {
            // Guild description: replace quote characters before DB packet creation.
            cmp cl,0x27
            je _replace_character
            cmp cl,0x22
            jne _copy_character

        _replace_character:
            mov cl,0x20

        _copy_character:
            mov byte ptr [edx+eax],cl
            inc eax
            test cl,cl
            jmp u0x431CE8
        }
    }

    void __declspec(naked) naked_0x431E27()
    {
        __asm
        {
            // Guild description update: replace quote characters before DB packet creation.
            cmp cl,0x27
            je _replace_character
            cmp cl,0x22
            jne _copy_character

        _replace_character:
            mov cl,0x20

        _copy_character:
            mov byte ptr [edx+eax],cl
            inc eax
            test cl,cl
            jmp u0x431E2D
        }
    }

    void __declspec(naked) naked_0x49AA35()
    {
        __asm
        {
            // Do not clear potential passive skills while stat reset is disabled.
            cmp byte ptr [esi+0x58EA],1
            je _skip_clear

            // original
            mov dword ptr [esi+0x1380],0

        _skip_clear:
            jmp u0x49AA3F
        }
    }

    // Jump cut solution.
    // If the packet/state byte at [ebp+0x02] is 2, jump to the original
    // success/skip branch used by the tested CE script; otherwise preserve the
    // original stack write and continue normally.
    unsigned u0x478954 = 0x478954;
    unsigned u0x479155 = 0x479155;
    void __declspec(naked) naked_0x47894D()
    {
        __asm
        {
            // original
            mov cl,[ebp+0x02]

            cmp cl,0x02
            je _jump_cut_solution

            // original
            mov [esp+0x22],edx
            jmp u0x478954

        _jump_cut_solution:
            jmp u0x479155
        }
    }

    // Drop and Gold directly to inventory + Gold bonus settings.
    // First hook: when no raid pointer exists, award solo gold directly to the
    // player. White Tiger Charm and Red Phoenix Charm apply fixed gold bonuses.
    unsigned u0x46BBA0 = 0x46BBA0;
    unsigned u0x4BAD58 = 0x4BAD58;
    unsigned u0x4BAE0D = 0x4BAE0D;
    unsigned u0x4BAEB8 = 0x4BAEB8;
    void __declspec(naked) naked_0x4BAD4C()
    {
        __asm
        {
            // original
            mov eax,dword ptr [esp+0x44]
            test eax,eax
            je _solo_gold
            jmp u0x4BAD58

        _solo_gold:
            mov edx,dword ptr [esp+0x40]
            test edx,edx
            je _original_solo_ground_gold

            cmp dword ptr [edx+0x594C],2
            je _solo_bonus_wtc
            cmp dword ptr [edx+0x594C],3
            je _solo_bonus_rpc

        _solo_bonus_ok:
            mov ecx,dword ptr [esp+0x18]
            call u0x46BBA0
            jmp u0x4BAEB8

        _solo_bonus:
            sub esp,4
            fild dword ptr [esp+0x1C]
            fld dword ptr [esp]
            fmul st(1),st(0)
            fstp dword ptr [esp]
            fistp dword ptr [esp+0x1C]
            add esp,4
            jmp _solo_bonus_ok

        _solo_bonus_wtc:
            mov dword ptr [esp-0x4],0x3F99999A // White Tiger Charm = +20%
            jmp _solo_bonus

        _solo_bonus_rpc:
            mov dword ptr [esp-0x4],0x3FC00000 // Red Phoenix Charm = +50%
            jmp _solo_bonus

        _original_solo_ground_gold:
            jmp u0x4BAE0D
        }
    }

    // Drop and Gold directly to inventory + Gold bonus settings.
    // Second hook: when no party/raid recipient exists, send the item directly
    // to the solo player's inventory instead of creating a world drop.
    unsigned u0x46AE60 = 0x46AE60;
    unsigned u0x4BB1DD = 0x4BB1DD;
    unsigned u0x4BB438 = 0x4BB438;
    unsigned u0x4BB47A = 0x4BB47A;
    void __declspec(naked) naked_0x4BB1D5()
    {
        __asm
        {
            // original
            test ebp,ebp
            je _solo_item
            jmp u0x4BB1DD

        _solo_item:
            mov ecx,dword ptr [esp+0x18]
            test ecx,ecx
            je _original_no_item_recipient

            push ebx
            call u0x46AE60
            jmp u0x4BB47A

        _original_no_item_recipient:
            jmp u0x4BB438
        }
    }

    // Revive with max HP/MP/SP.
    // The stock revive flow starts by restoring only part of the character's
    // resources. This hook mirrors the tested CE script and copies the max
    // HP/SP/MP values into the current HP/SP/MP fields before continuing after
    // the original partial-restore block.
    unsigned u0x466908 = 0x466908;
    void __declspec(naked) naked_0x4668D5()
    {
        __asm
        {
            mov eax,[edi+0x178]
            mov [edi+0x1234],eax

            mov eax,[edi+0x180]
            mov [edi+0x123C],eax

            mov eax,[edi+0x17C]
            mov [edi+0x1238],eax

            jmp u0x466908
        }
    }

    // Infinite consumables.
    // Hooked at the stock "dec al / mov [ebx+0x42],al" stack-consumption site.
    // For configured item IDs, refresh the stack to 255 and skip the DB
    // decrement write. Unlike the original CE experiment, this also refreshes
    // stacks at 1 so the consumable is truly infinite from the user's point of
    // view. The original DB function returns with ret 8, so the infinite branch
    // manually balances those two pending arguments before continuing at 00472AEB.
    unsigned u0x472AD2 = 0x472AD2;
    unsigned u0x472AEB = 0x472AEB;
    void __declspec(naked) naked_0x472ACD()
    {
        __asm
        {
            push ecx
            push edx
            push eax

            mov edx,[ebx+0x30]
            test edx,edx
            je _not_infinite

            // Safety guard: configured infinite consumables must be unsellable.
            // If SellPrice is greater than zero, consume the item normally so
            // a mistaken Item.SData edit cannot create an infinite gold exploit.
            cmp dword ptr [edx+0x84],0
            jne _not_infinite

            // Item IDs allowed to behave as infinite consumables.
            // Add/remove IDs here using the same "cmp dword ptr [edx],ID"
            // pattern. This intentionally mirrors the CE script exactly.
            cmp dword ptr [edx],25006 // Echobloom
            je _refresh_stack
            cmp dword ptr [edx],25012 // Hubbin Fruit
            je _refresh_stack
            cmp dword ptr [edx],25018 // Magic Herb
            je _refresh_stack
            cmp dword ptr [edx],25019 // Mini Healing Potion
            je _refresh_stack
            cmp dword ptr [edx],25020 // Healing Potion
            je _refresh_stack
            cmp dword ptr [edx],25021 // Great Healing Potion
            je _refresh_stack
            cmp dword ptr [edx],25022 // Mini Stamina Potion
            je _refresh_stack
            cmp dword ptr [edx],25023 // Stamina Potion
            je _refresh_stack
            cmp dword ptr [edx],25024 // Great Stamina Potion
            je _refresh_stack
            cmp dword ptr [edx],25025 // Mini Mana Potion
            je _refresh_stack
            cmp dword ptr [edx],25026 // Mana Potion
            je _refresh_stack
            cmp dword ptr [edx],25027 // Great Mana Potion
            je _refresh_stack
            cmp dword ptr [edx],25037 // Mini Super Potion
            je _refresh_stack
            cmp dword ptr [edx],25038 // Super Potion
            je _refresh_stack
            cmp dword ptr [edx],25039 // Great Super Potion
            je _refresh_stack
            cmp dword ptr [edx],25068 // Cure Potion
            je _refresh_stack
            cmp dword ptr [edx],25069 // Dispel Potion
            je _refresh_stack
            cmp dword ptr [edx],25070 // Abolishing Potion
            je _refresh_stack
            cmp dword ptr [edx],25071 // Blessing Potion
            je _refresh_stack
            cmp dword ptr [edx],25072 // Holy Potion
            je _refresh_stack

        _not_infinite:
            pop eax
            pop edx
            pop ecx

            // original
            dec al
            mov [ebx+0x42],al
            jmp u0x472AD2

        _refresh_stack:
            pop eax
            pop edx
            pop ecx

            mov al,0xFF
            mov [ebx+0x42],al
            mov eax,0x702
            mov [esp+0x20],ax
            mov eax,[edi+0x582C]
            mov [esp+0x22],eax
            add esp,8
            jmp u0x472AEB

        }
    }

    // Use an skill at death, default Unto LV1.
    // Edit these two constants to change the skill that is automatically used
    // when the player reaches the LifeSkill/Rebirth death paths.
    constexpr int kDeathSkillId = 276;
    constexpr int kDeathSkillLevel = 1;

    // Shared wrapper: use_skill_player(player, skill_id, skill_level).
    // It mirrors the CE helper exactly: resolve SkillInfo through
    // CGameData::GetSkillInfo(id, level), then invoke CUser::UseItemSkill.
    unsigned u0x41BB30 = 0x41BB30;
    unsigned u0x4725B0 = 0x4725B0;
    void __declspec(naked) use_skill_player()
    {
        __asm
        {
            push ebp
            mov ebp,esp
            mov edx,[ebp+0x08]
            mov eax,[ebp+0x0C]
            mov edi,[ebp+0x10]
            call u0x41BB30
            call u0x4725B0
            pop ebp
            ret 0x0C
        }
    }

    unsigned u0x466D87 = 0x466D87;
    void __declspec(naked) naked_0x466D80()
    {
        __asm
        {
            pushad

            push ebp
            push kDeathSkillId
            push kDeathSkillLevel
            call use_skill_player

            popad

            // original
            mov ecx,[esp+0xB0]
            jmp u0x466D87
        }
    }

    unsigned u0x46703D = 0x46703D;
    void __declspec(naked) naked_0x467036()
    {
        __asm
        {
            pushad

            push edi
            push kDeathSkillId
            push kDeathSkillLevel
            call use_skill_player

            popad

            // original
            xor ebx,ebx
            mov eax,1
            jmp u0x46703D
        }
    }

    // Boss death/spawn server wide notices.
    // Both hooks build the same 0xF90B notice packet as the CE scripts, but the
    // string assembly lives in C++ so the bounds are explicit and easy to audit.
    constexpr std::size_t kBossNoticeBufferSize = 100;
    unsigned u0x419120 = 0x419120;
    unsigned u0x4A2089 = 0x4A2089;
    unsigned u0x422F13 = 0x422F13;

    std::size_t bounded_strlen(const char* text, std::size_t maxLength)
    {
        if (!text)
            return 0;

        std::size_t length = 0;
        while (length < maxLength && text[length])
            ++length;

        return length;
    }

    void append_notice_text(char* buffer, const char* text)
    {
        auto length = bounded_strlen(buffer, kBossNoticeBufferSize);
        if (length >= kBossNoticeBufferSize)
            return;

        while (text && *text && length + 1 < kBossNoticeBufferSize)
            buffer[length++] = *text++;

        buffer[length] = '\0';
    }

    void send_server_notice_packet(char* buffer)
    {
        auto length = static_cast<unsigned char>(std::strlen(buffer));
        buffer[2] = length;

        auto packetSize = static_cast<unsigned>(length) + 3;
        auto objectMgr = *reinterpret_cast<unsigned*>(0x587960);

        __asm
        {
            mov esi,objectMgr
            mov eax,packetSize
            mov ecx,buffer
            call u0x419120
        }
    }

    void __stdcall send_boss_death_notice(void* mob, void* player)
    {
        if (!mob || !player)
            return;

        auto mobInfo = *reinterpret_cast<std::uintptr_t*>(
            static_cast<char*>(mob) + 0xD4);
        if (!mobInfo)
            return;

        char buffer[kBossNoticeBufferSize]{};
        buffer[0] = 0x0B;
        buffer[1] = static_cast<char>(0xF9);
        buffer[2] = static_cast<char>(0xFF);

        append_notice_text(buffer, reinterpret_cast<const char*>(mobInfo + 2));
        append_notice_text(buffer, " was just killed by ");
        append_notice_text(buffer, static_cast<const char*>(player) + 0x184);
        send_server_notice_packet(buffer);
    }

    void __stdcall send_boss_spawn_notice(void* mobName)
    {
        if (!mobName)
            return;

        char buffer[kBossNoticeBufferSize]{};
        buffer[0] = 0x0B;
        buffer[1] = static_cast<char>(0xF9);
        buffer[2] = static_cast<char>(0xFF);

        append_notice_text(buffer, static_cast<const char*>(mobName) + 2);
        append_notice_text(buffer, " just spawned!");
        send_server_notice_packet(buffer);
    }

    void __declspec(naked) naked_0x4A2083()
    {
        __asm
        {
            pushad
            push eax
            push ebx
            call send_boss_death_notice
            popad

            // original
            lea ecx,[ebx+0xDD4]
            jmp u0x4A2089
        }
    }

    void __declspec(naked) naked_0x422F0D()
    {
        __asm
        {
            pushad
            push ebx
            call send_boss_spawn_notice
            popad

            // original
            lea edx,[edi+0x98]
            jmp u0x422F13
        }
    }

    // Lapisia operation flux fix.
    // The stock code seeds this operation with _time64(0), which is too coarse
    // for repeated lapisia attempts. GetTickCount64 gives a moving millisecond
    // seed, then the original user-specific xor is preserved.
    unsigned u0x46CD1B = 0x46CD1B;
    unsigned u0x4BD500 = 0x4BD500;
    void __declspec(naked) naked_0x46CCFE()
    {
        __asm
        {
            call GetTickCount64

            // original, adjusted for no _time64 stack argument
            mov edi,[ebp+0x57F8]
            xor edi,eax
            mov eax,esi
            mov [esp+0x40],edx
            call u0x4BD500

            jmp u0x46CD1B
        }
    }

    // Instant Mounts.
    // Skips the mount cast delay and immediately sends the success packet/state
    // refresh that the delayed path would normally emit after completing.
    unsigned u0x477999 = 0x477999;
    unsigned u0x4ED0E0 = 0x4ED0E0;
    unsigned u0x4913E0 = 0x4913E0;
    unsigned packetSuccess = 0;
    void __declspec(naked) naked_0x477164()
    {
        __asm
        {
            mov [esp+0x24],dx
            push ecx
            lea ecx,[ebp+0xD0]
            mov edx,6
            mov [ebp+0x1480],esi
            mov [esp+0x2A],eax
            call _remove_delay

            pushad

            mov eax,[ebp+0x1F4]
            mov dword ptr [ebp+0x1484],0x0E
            mov ecx,[eax+0x30]
            cmp byte ptr [ecx+0x30],2
            jb _mount_type_ok

            mov dword ptr [ebp+0x1484],0x0F

        _mount_type_ok:
            mov dword ptr [packetSuccess],0x01010216
            movzx ecx,word ptr [ecx+0x42]
            cmp cx,2
            movzx ecx,cx
            jb _low_mount_model

            add ecx,7
            jmp _store_mount_model

        _low_mount_model:
            add ecx,ecx

        _store_mount_model:
            mov [ebp+0x1488],ecx
            lea eax,[packetSuccess]
            push 4
            push eax
            mov ecx,ebp
            call u0x4ED0E0
            mov dword ptr [ebp+0x147C],2
            mov ecx,ebp
            call u0x4913E0

            popad
            jmp u0x477999

        _remove_delay:
            ret 4
        }
    }

    // Support for off-hand feature.
    // Expands CItem::IsOneHandWeapon so the off-hand validation accepts the
    // configured realType range used by the custom off-hand system.
    unsigned u0x468129 = 0x468129;
    unsigned u0x468140 = 0x468140;
    void __declspec(naked) naked_0x468120()
    {
        __asm
        {
            // original
            mov ecx,[eax+0x30]
            mov eax,[ecx+0xA0]

            cmp eax,9
            je _one_hand_weapon
            cmp eax,10
            je _one_hand_weapon
            cmp eax,11
            je _one_hand_weapon
            cmp eax,12
            je _one_hand_weapon
            cmp eax,13
            je _one_hand_weapon
            cmp eax,14
            je _one_hand_weapon
            cmp eax,15
            je _one_hand_weapon

            jmp u0x468129

        _one_hand_weapon:
            jmp u0x468140
        }
    }
}

void hook::utilities()
{
    // Cross-faction whisper.
    util::write_memory((void*)0x47F629, 0x90, 6);
    util::write_memory((void*)0x47FF69, 0x90, 6);

    // Cross-faction trade.
    util::write_memory((void*)0x47D9B6, 0x90, 6);

    // Cross-faction inspect.
    util::write_memory((void*)0x477D49, 0x90, 6);

    // Both factions GM chat.
    util::detour((void*)0x427938, naked_0x427938, 6);

    // Enable summon/move on Exiel room.
    // This build matches the original CE script offsets directly.
    util::write_memory((void*)0x4733E9, 0x90, 10);
    util::write_memory((void*)0x473567, 0x90, 6);

    // Stack Fortune Bag drops on Inventory.
    util::detour((void*)0x46C27E, naked_0x46C27E, 6);

    // Union leader summons the whole raid.
    util::detour((void*)0x49E4E1, naked_0x49E4E1, 5);

    // Rune cutting fixes for Capital, Auction House, Arena, and Guild maps.
    util::detour((void*)0x473DB0, naked_0x473DB0, 7);
    util::detour((void*)0x473ED7, naked_0x473ED7, 7);
    util::detour((void*)0x473FFD, naked_0x473FFD, 7);
    util::detour((void*)0x474196, naked_0x474196, 7);

    // Enable helmets and mantles to drop.
    // The original server rejects item type 16 (helmet) and 24 (mantle) here.
    util::write_memory((void*)0x4BAFC8, 0x90, 9);
    util::write_memory((void*)0x4BAFD1, 0x90, 9);
    util::write_memory((void*)0x473BBD, 0x90, 13);

    // Fix mantles not dropping on bag items.
    // These two checks are a separate bag-item drop path; without them, mantle
    // drops can still be rejected even after enabling normal mantle drops.
    util::write_memory((void*)0x473B87, 0x90, 2);
    util::write_memory((void*)0x473BC4, 0x90, 6);

    // Enable cross-faction login and world enter.
    // The original code conditionally rejects the opposite faction; these
    // patches turn the tested CE conditional jumps into unconditional jumps.
    unsigned char crossFactionLoginLight[] = { 0xE9, 0x68, 0x01, 0x00, 0x00, 0x90 };
    unsigned char crossFactionLoginFury[] = { 0xE9, 0xAE, 0x00, 0x00, 0x00, 0x90 };
    unsigned char crossFactionWorldEnter = 0xEB;
    util::write_memory((void*)0x47B54E, crossFactionLoginLight, sizeof(crossFactionLoginLight));
    util::write_memory((void*)0x47B608, crossFactionLoginFury, sizeof(crossFactionLoginFury));
    util::write_memory((void*)0x47C134, &crossFactionWorldEnter, sizeof(crossFactionWorldEnter));

    // Guilds.
    // Allow more than the original 7 guild officers.
    util::write_memory((void*)0x43491B, 0x90, 10);

    // Remove the guild creation/join penalty timer check.
    unsigned char removeGuildPenalty[] = { 0xE9, 0x34, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    util::write_memory((void*)0x48598F, removeGuildPenalty, sizeof(removeGuildPenalty));

    // Let players enter GRB more than once by not storing the one-entry marker.
    util::write_memory((void*)0x4569CB, 0x90, 6);

    // Create guild with 2 players: change the exact-party-size check from 7 to 2.
    // The following original "jl fail" remains in place and uses these flags.
    unsigned char createGuildCheck[] = { 0x83, 0xF8, 0x02, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    util::write_memory((void*)0x4850A8, createGuildCheck, sizeof(createGuildCheck));

    // Create guild request: require at least 2 players instead of 7.
    unsigned char createGuildRequest[] = { 0x83, 0x7E, 0x24, 0x02, 0x7C, 0x24 };
    util::write_memory((void*)0x48542C, createGuildRequest, sizeof(createGuildRequest));

    // Character growth.
    // Use the Ultimate Mode status/skill-point reset and level-up paths for every mode.
    unsigned char statResetStatusUltimate[] = { 0xEB, 0x1C, 0x90, 0x90, 0x90, 0x90, 0x90 };
    unsigned char statResetSkillUltimate[] = { 0xEB, 0x22, 0x90, 0x90, 0x90, 0x90, 0x90 };
    unsigned char levelUpStatusUltimate[] = { 0xEB, 0x18, 0x90, 0x90, 0x90, 0x90, 0x90 };
    util::write_memory((void*)0x48F95B, statResetStatusUltimate, sizeof(statResetStatusUltimate));
    util::write_memory((void*)0x48FCA4, statResetSkillUltimate, sizeof(statResetSkillUltimate));
    util::write_memory((void*)0x49B45B, levelUpStatusUltimate, sizeof(levelUpStatusUltimate));

    // Give 5 skill points per level to all modes/growth branches.
    unsigned char skillPointsPerLevel = 5;
    util::write_memory((void*)0x49B496, &skillPointsPerLevel, sizeof(skillPointsPerLevel));
    util::write_memory((void*)0x49B5D2, &skillPointsPerLevel, sizeof(skillPointsPerLevel));

    // Security.
    // Sanitize command, character, and guild strings before they reach DB-bound code.
    util::detour((void*)0x480A62, naked_0x480A62, 6);
    util::detour((void*)0x4905A3, naked_0x4905A3, 6);
    util::detour((void*)0x47A512, naked_0x47A512, 6);
    util::detour((void*)0x431CB8, naked_0x431CB8, 6);
    util::detour((void*)0x431CE2, naked_0x431CE2, 6);
    util::detour((void*)0x431E27, naked_0x431E27, 6);

    // Prevent stat/skill reset items from clearing potential skills when reset is disabled.
    util::detour((void*)0x49AA35, naked_0x49AA35, 10);

    // Jump cut solution.
    util::detour((void*)0x47894D, naked_0x47894D, 7);

    // Drop and Gold directly to inventory + Gold bonus settings.
    util::detour((void*)0x4BAD4C, naked_0x4BAD4C, 12);
    util::detour((void*)0x4BB1D5, naked_0x4BB1D5, 8);

    // Revive with max HP/MP/SP.
    util::detour((void*)0x4668D5, naked_0x4668D5, 6);

    // Infinite consumables.
    util::detour((void*)0x472ACD, naked_0x472ACD, 5);

    // Use an skill at death, default Unto LV1.
    util::detour((void*)0x466D80, naked_0x466D80, 7);
    util::detour((void*)0x467036, naked_0x467036, 7);

    // Boss death/spawn server wide notices.
    util::detour((void*)0x4A2083, naked_0x4A2083, 6);
    util::detour((void*)0x422F0D, naked_0x422F0D, 6);

    // Lapisia operation flux fix.
    util::detour((void*)0x46CCFE, naked_0x46CCFE, 7);

    // Fix mantle enchancements hack via packet.
    // Redirect the mantle rejection branch to the regular failure return path.
    unsigned char mantleEnhancementPacketFix[] = { 0x0F, 0x84, 0xB9, 0x00, 0x00, 0x00 };
    util::write_memory((void*)0x46C9F8, mantleEnhancementPacketFix, sizeof(mantleEnhancementPacketFix));

    // Fix mantle merchants spawn at server start.
    // jne 0x42FBD7 -> jmp 0x42FBD7
    unsigned char mantleMerchantSpawnFix = 0xEB;
    util::write_memory((void*)0x42FBBF, &mantleMerchantSpawnFix, sizeof(mantleMerchantSpawnFix));

    // Instant Mounts.
    util::detour((void*)0x477164, naked_0x477164, 5);

    // Support for off-hand feature.
    util::detour((void*)0x468120, naked_0x468120, 9);

    // Characters can run on Stealth.
    util::write_memory((void*)0x49429F, 0x90, 8);

    // Cloaks provide real defense and resistance.
    // Equip/desequip cloak stat handling: include cloak slot/type 6 in the
    // same defense/resistance recalculation paths that originally started at 7.
    unsigned char cloakDefenseResistanceStart = 6;
    util::write_memory((void*)0x4616B4, &cloakDefenseResistanceStart, sizeof(cloakDefenseResistanceStart));
    util::write_memory((void*)0x461D89, &cloakDefenseResistanceStart, sizeof(cloakDefenseResistanceStart));

    // Change ress leader timer from 30s to 5s.
    // add eax,30000 -> add eax,5000
    int ressLeaderTimerMs = 5000;
    util::write_memory((void*)0x478EA3, &ressLeaderTimerMs, sizeof(ressLeaderTimerMs));

    // Change logout time .ms.
    // CUser+0x587C stores the logout timestamp; these four paths used
    // add eax,10000. A raw 2000ms patch lands at about 3s in practice because
    // the stock logout flow adds an extra visible tick, so use 1000ms here to
    // make the real player-facing logout delay land close to 2s.
    int logoutTimeMs = 1000;
    util::write_memory((void*)0x4140FE, &logoutTimeMs, sizeof(logoutTimeMs));
    util::write_memory((void*)0x414294, &logoutTimeMs, sizeof(logoutTimeMs));
    util::write_memory((void*)0x474CF0, &logoutTimeMs, sizeof(logoutTimeMs));
    util::write_memory((void*)0x474D4F, &logoutTimeMs, sizeof(logoutTimeMs));

    // Stabilize obelisk spawn times, no longer uses 1 hour more or less - Just
    // have +20-30s spawn than the start value configured.
    // Removes the stock ((random % 120) - 60) * 60000 randomizer from the
    // Obelisk.ini respawn path; the scheduler/tick delay can still add a small
    // practical delay over the configured start value.
    unsigned char stabilizeObeliskSpawnTimes[] = { 0x31, 0xD2, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    util::write_memory((void*)0x4287BD, stabilizeObeliskSpawnTimes, sizeof(stabilizeObeliskSpawnTimes));
}
