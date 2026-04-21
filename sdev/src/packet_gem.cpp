#include <random>
#include <ranges>
#include <string>
#include <util/util.h>
#include <shaiya/include/common/ItemEffect.h>
#include <shaiya/include/common/ItemSlot.h>
#include <shaiya/include/common/ItemType.h>
#include <shaiya/include/network/TP_MAIN.h>
#include <shaiya/include/network/dbAgent/incoming/0700.h>
#include <shaiya/include/network/game/incoming/0800.h>
#include <shaiya/include/network/game/outgoing/0200.h>
#include <shaiya/include/network/game/outgoing/0800.h>
#include <shaiya/include/network/gameLog/incoming/0400.h>
#include "include/main.h"
#include "include/shaiya/CGameData.h"
#include "include/shaiya/CItem.h"
#include "include/shaiya/CUser.h"
#include "include/shaiya/ItemFinder.h"
#include "include/shaiya/ItemHelper.h"
#include "include/shaiya/ItemInfo.h"
#include "include/shaiya/ItemPredicate.h"
#include "include/shaiya/ItemSynthesis.h"
#include "include/shaiya/NetworkHelper.h"
#include "include/shaiya/UserHelper.h"
using namespace shaiya;

namespace packet_gem
{
    void send_item_slot_update(CUser* user, CItem* item, int bag, int slot)
    {
        GameItemGetOutgoing outgoing{};
        outgoing.bag = bag;
        outgoing.slot = slot;
        outgoing.type = item->type;
        outgoing.typeId = item->typeId;
        outgoing.count = item->count;
        outgoing.quality = item->quality;
        outgoing.gems = item->gems;
        outgoing.craftName = item->craftName;
        NetworkHelper::Send(user, &outgoing, sizeof(GameItemGetOutgoing));
    }

    bool can_add_craft_stat(int currentValue, int numCraftExpansions, int maxCraftExpansions)
    {
        return currentValue || numCraftExpansions < maxCraftExpansions;
    }

    bool has_craft_ability(CItem* item)
    {
        return item->craftStrength
            || item->craftDexterity
            || item->craftReaction
            || item->craftIntelligence
            || item->craftWisdom
            || item->craftLuck
            || item->craftHealth
            || item->craftMana
            || item->craftStamina
            || item->craftAttackPower
            || item->craftAbsorption;
    }

    bool has_transferable_item_ability(CItem* item)
    {
        if (!item || !item->info)
            return false;

        return CItem::GetGemCount(item) > 0 || has_craft_ability(item);
    }

    bool is_valid_item_ability_transfer_item(CItem* item)
    {
        if (!item || !item->info)
            return false;

        // Reject malformed inventory entries whose CItem type pair does not
        // resolve back to the same ItemInfo row. Without this, invalid item
        // Type/TypeID combinations can pass the looser realType comparison and
        // consume the cube without a meaningful transfer.
        auto itemInfo = CGameData::GetItemInfo(item->type, item->typeId);
        if (!itemInfo || itemInfo != item->info)
            return false;

        return CItem::IsEquipment(item);
    }

    bool can_transfer_item_ability(CItem* source, CItem* target)
    {
        return
            is_valid_item_ability_transfer_item(source) &&
            is_valid_item_ability_transfer_item(target) &&
            has_transferable_item_ability(source) &&
            target->info->realType == source->info->realType &&
            target->info->level >= source->info->level &&
            target->info->slots >= source->info->slots &&
            target->info->craftExpansions >= source->info->craftExpansions &&
            target->info->reqWis >= source->info->reqWis;
    }

    int get_num_craft_expansions(CItem* item)
    {
        int count{};

        if (item->craftStrength)
            ++count;
        if (item->craftDexterity)
            ++count;
        if (item->craftReaction)
            ++count;
        if (item->craftIntelligence)
            ++count;
        if (item->craftWisdom)
            ++count;
        if (item->craftLuck)
            ++count;
        if (item->craftHealth)
            ++count;
        if (item->craftMana)
            ++count;
        if (item->craftStamina)
            ++count;

        return count;
    }

    /// <summary>
    /// Adds support for item effect 103.
    /// </summary>
    bool lapisian_add_protect(CUser* user, GameLapisianAddIncoming_EP6_4* incoming)
    {
        if (!incoming->itemProtect)
            return false;

        int bag{}, slot{};
        if (!ItemFinder(user, bag, slot, ItemEffectEqualF(ItemEffect::LapisianAddItemProtect)))
            return false;

        CUser::ItemUseNSend(user, bag, slot, false);
        return true;
    }

    /// <summary>
    /// Handles incoming 0x806 packets.
    /// Future feature - broken right now: custom deterministic runes based on
    /// effect 62 plus a selector are intentionally not active. Effect 62 keeps
    /// the stock random recreation behavior.
    /// </summary>
    void handler_0x806(CUser* user, GameItemComposeIncoming_EP6_4* incoming)
    {
        if (!incoming->runeBag || incoming->runeBag > user->bagsUnlocked)
            return;

        if (incoming->runeSlot >= ItemSlotCount)
            return;

        auto& rune = user->inventory[incoming->runeBag][incoming->runeSlot];
        if (!rune)
            return;

        if (incoming->itemBag > user->bagsUnlocked)
            return;

        if (incoming->itemSlot >= ItemSlotCount)
            return;

        auto& item = user->inventory[incoming->itemBag][incoming->itemSlot];
        if (!item)
            return;

        GameItemComposeOutgoing failure{};
        failure.result = GameItemComposeResult::Failure;

        if (!item->info->craftExpansions)
        {
            NetworkHelper::Send(user, &failure, sizeof(GameItemComposeOutgoing));
            return;
        }

        if (!item->info->reqWis || item->info->reqWis > ItemCraft_MAX)
        {
            NetworkHelper::Send(user, &failure, sizeof(GameItemComposeOutgoing));
            return;
        }

        //if (item->makeType == MakeType::QuestResult)
        //{
        //    NetworkHelper::Send(user, &failure, sizeof(GameItemComposeOutgoing));
        //    return;
        //}

        auto composeUniqueId = rune->uniqueId;
        auto composeItemId = rune->info->itemId;
        auto oldCraftName = item->craftName;
        // reqWis is reused as the maximum value for a single craft stat.
        auto maxCraftValue = item->info->reqWis;
        auto numCraftExpansions = get_num_craft_expansions(item);
        auto maxCraftExpansions = item->info->craftExpansions;

        std::random_device seed;
        std::mt19937 eng(seed());
        std::uniform_int_distribution<uint16_t> uni(1, maxCraftValue);
        auto craftValue = uni(eng);

        auto maxHealth = user->maxHealth;
        auto maxMana = user->maxHealth;
        auto maxStamina = user->maxHealth;

        switch (rune->info->effect)
        {
        case ItemEffect::RecreationRune:
            if (!incoming->itemBag)
            {
                CUser::ItemEquipmentOptionRem(user, item);
                CItem::ReGenerationCraftExpansion(item, true);
                CUser::ItemEquipmentOptionAdd(user, item);
                break;
            }

            CItem::ReGenerationCraftExpansion(item, true);
            break;
        case ItemEffect::StrRecreationRune:
            // Perfect runes may create the stat if the item still has free craft slots.
            if (!can_add_craft_stat(item->craftStrength, numCraftExpansions, maxCraftExpansions))
                return;

            if (!incoming->itemBag)
            {
                CUser::ItemEquipmentOptionRem(user, item);
                ItemHelper::SetCraftStrength(item, craftValue);
                CUser::ItemEquipmentOptionAdd(user, item);
                break;
            }

            ItemHelper::SetCraftStrength(item, craftValue);

            break;
        case ItemEffect::DexRecreationRune:
            if (!can_add_craft_stat(item->craftDexterity, numCraftExpansions, maxCraftExpansions))
                return;

            if (!incoming->itemBag)
            {
                CUser::ItemEquipmentOptionRem(user, item);
                ItemHelper::SetCraftDexterity(item, craftValue);
                CUser::ItemEquipmentOptionAdd(user, item);
                break;
            }

            ItemHelper::SetCraftDexterity(item, craftValue);

            break;
        case ItemEffect::IntRecreationRune:
            if (!can_add_craft_stat(item->craftIntelligence, numCraftExpansions, maxCraftExpansions))
                return;

            if (!incoming->itemBag)
            {
                CUser::ItemEquipmentOptionRem(user, item);
                ItemHelper::SetCraftIntelligence(item, craftValue);
                CUser::ItemEquipmentOptionAdd(user, item);
                break;
            }

            ItemHelper::SetCraftIntelligence(item, craftValue);

            break;
        case ItemEffect::WisRecreationRune:
            if (!can_add_craft_stat(item->craftWisdom, numCraftExpansions, maxCraftExpansions))
                return;

            if (!incoming->itemBag)
            {
                CUser::ItemEquipmentOptionRem(user, item);
                ItemHelper::SetCraftWisdom(item, craftValue);
                CUser::ItemEquipmentOptionAdd(user, item);
                break;
            }

            ItemHelper::SetCraftWisdom(item, craftValue);

            break;
        case ItemEffect::RecRecreationRune:
            if (!can_add_craft_stat(item->craftReaction, numCraftExpansions, maxCraftExpansions))
                return;

            if (!incoming->itemBag)
            {
                CUser::ItemEquipmentOptionRem(user, item);
                ItemHelper::SetCraftReaction(item, craftValue);
                CUser::ItemEquipmentOptionAdd(user, item);
                break;
            }

            ItemHelper::SetCraftReaction(item, craftValue);

            break;
        case ItemEffect::LucRecreationRune:
            if (!can_add_craft_stat(item->craftLuck, numCraftExpansions, maxCraftExpansions))
                return;

            if (!incoming->itemBag)
            {
                CUser::ItemEquipmentOptionRem(user, item);
                ItemHelper::SetCraftLuck(item, craftValue);
                CUser::ItemEquipmentOptionAdd(user, item);
                break;
            }

            ItemHelper::SetCraftLuck(item, craftValue);

            break;
        default:
            NetworkHelper::Send(user, &failure, sizeof(GameItemComposeOutgoing));
            return;
        }

        if (!incoming->itemBag)
        {
            if (!user->initStatusFlag)
            {
                if (maxHealth != user->maxHealth)
                    CUser::SendMaxHP(user);

                if (maxMana != user->maxMana)
                    CUser::SendMaxMP(user);

                if (maxStamina != user->maxStamina)
                    CUser::SendMaxSP(user);
            }

            CUser::SetAttack(user);
        }

        CUser::ItemUseNSend(user, incoming->runeBag, incoming->runeSlot, false);
        ItemHelper::SendDBAgentCraftName(user, item, incoming->itemBag, incoming->itemSlot);

        GameLogItemComposeIncoming gameLog{};
        CUser::SetGameLogMain(user, &gameLog.packet);
        gameLog.packet.uniqueId = item->uniqueId;
        gameLog.packet.itemId = item->info->itemId;
        gameLog.packet.itemName = item->info->itemName;
        gameLog.packet.gems = item->gems;
        gameLog.packet.makeTime = item->makeTime;
        gameLog.packet.craftName = item->craftName;
        gameLog.packet.composeUniqueId = composeUniqueId;
        gameLog.packet.composeItemId = composeItemId;
        gameLog.packet.oldCraftName = oldCraftName;
        NetworkHelper::SendGameLog(&gameLog.packet, sizeof(GameLogItemComposeIncoming));

        GameItemComposeOutgoing success{};
        success.result = GameItemComposeResult::Success;
        success.bag = incoming->itemBag;
        success.slot = incoming->itemSlot;
        success.craftName = item->craftName;
        NetworkHelper::Send(user, &success, sizeof(GameItemComposeOutgoing));
    }

    /// <summary>
    /// Handles incoming 0x830 packets.
    /// </summary>
    void handler_0x830(CUser* user, GameChaoticSquareListIncoming* incoming)
    {
        if (!incoming->chaoticSquareBag || incoming->chaoticSquareBag > user->bagsUnlocked)
            return;

        if (incoming->chaoticSquareSlot >= ItemSlotCount)
            return;

        auto& chaoticSquare = user->inventory[incoming->chaoticSquareBag][incoming->chaoticSquareSlot];
        if (!chaoticSquare)
            return;

        if (chaoticSquare->info->effect != ItemEffect::ChaoticSquare)
            return;

        user->savePosUseBag = incoming->chaoticSquareBag;
        user->savePosUseSlot = incoming->chaoticSquareSlot;

        CUser::CancelActionExc(user);
        MyShop::Ended(&user->myShop);

        auto itemId = chaoticSquare->info->itemId;
        for (const auto& chaoticSquare : g_chaoticSquares)
        {
            if (chaoticSquare.itemId == itemId)
            {
                GameChaoticSquareListOutgoing outgoing{};
                outgoing.newItemType = chaoticSquare.newItemType;
                outgoing.newItemTypeId = chaoticSquare.newItemTypeId;
                outgoing.goldPerPercentage = ItemSynthesisConstants::GoldPerPercentage;
                NetworkHelper::Send(user, &outgoing, sizeof(GameChaoticSquareListOutgoing));
            }
        }
    }

    /// <summary>
    /// Handles incoming 0x831 packets.
    /// </summary>
    void handler_0x831(CUser* user, GameItemSynthesisRecipeIncoming* incoming)
    {
        auto& chaoticSquare = user->inventory[user->savePosUseBag][user->savePosUseSlot];
        if (!chaoticSquare)
            return;

        if (chaoticSquare->info->effect != ItemEffect::ChaoticSquare)
            return;

        auto it = g_itemSyntheses.find(chaoticSquare->info->itemId);
        if (it == g_itemSyntheses.end())
            return;

        if (incoming->index >= it->second.size())
            return;

        auto& synthesis = it->second[incoming->index];
        if (incoming->newItemType != synthesis.newItemType || incoming->newItemTypeId != synthesis.newItemTypeId)
            return;

        GameItemSynthesisRecipeOutgoing outgoing{};
        outgoing.successRate = synthesis.successRate;
        outgoing.materialType = synthesis.materialType;
        outgoing.newItemType = synthesis.newItemType;
        outgoing.materialTypeId = synthesis.materialTypeId;
        outgoing.newItemTypeId = synthesis.newItemTypeId;
        outgoing.materialCount = synthesis.materialCount;
        outgoing.newItemCount = synthesis.newItemCount;
        NetworkHelper::Send(user, &outgoing, sizeof(GameItemSynthesisRecipeOutgoing));
    }

    /// <summary>
    /// Handles incoming 0x832 packets.
    /// </summary>
    void handler_0x832(CUser* user, GameItemSynthesisIncoming* incoming)
    {
        if (!incoming->chaoticSquareBag || incoming->chaoticSquareBag > user->bagsUnlocked)
            return;

        if (incoming->chaoticSquareSlot >= ItemSlotCount)
            return;

        auto& chaoticSquare = user->inventory[incoming->chaoticSquareBag][incoming->chaoticSquareSlot];
        if (!chaoticSquare)
            return;

        if (chaoticSquare->info->effect != ItemEffect::ChaoticSquare)
            return;

        auto it = g_itemSyntheses.find(chaoticSquare->info->itemId);
        if (it == g_itemSyntheses.end())
            return;

        if (incoming->index >= it->second.size())
            return;

        auto& synthesis = it->second[incoming->index];
        auto newItemInfo = CGameData::GetItemInfo(synthesis.newItemType, synthesis.newItemTypeId);
        if (!newItemInfo)
            return;

        if (incoming->money > user->money)
            return;

        auto successRate = synthesis.successRate;
        auto goldPerPercentage = ItemSynthesisConstants::GoldPerPercentage;

        if (goldPerPercentage)
        {
            auto money = incoming->money;
            if (money > ItemSynthesisConstants::MaxMoney)
                money = ItemSynthesisConstants::MaxMoney;

            if (money >= ItemSynthesisConstants::MinMoney)
            {
                auto bonus = ((double)money / goldPerPercentage) * 100.0;
                successRate += static_cast<int>(bonus);

                user->money -= money;
                CUser::SendDBMoney(user);
            }
        }

        if (incoming->hammerBag != 0)
        {
            if (incoming->hammerBag > user->bagsUnlocked)
                return;

            if (incoming->hammerSlot >= ItemSlotCount)
                return;

            auto& hammer = user->inventory[incoming->hammerBag][incoming->hammerSlot];
            if (!hammer)
                return;

            if (hammer->info->effect != ItemEffect::CraftingHammer)
                return;

            successRate += hammer->info->reqVg * 100;
            CUser::ItemUseNSend(user, incoming->hammerBag, incoming->hammerSlot, false);
        }

        CUser::ItemUseNSend(user, incoming->chaoticSquareBag, incoming->chaoticSquareSlot, false);

        for (const auto& [type, typeId, count] : std::views::zip(
            std::as_const(synthesis.materialType),
            std::as_const(synthesis.materialTypeId),
            std::as_const(synthesis.materialCount)
        ))
        {
            if (!type || !typeId || !count)
                continue;

            int bag{}, slot{};
            if (!ItemFinder(user, bag, slot, ItemCountGreaterEqualF(type, typeId, count)))
                return;

            if (!UserHelper::ItemRemove(user, bag, slot, count))
                return;
        }

        int randomValue = 0;
        if (successRate < ItemSynthesisConstants::MaxSuccessRate)
        {
            std::random_device seed;
            std::mt19937 eng(seed());

            std::uniform_int_distribution<int> uni(
                ItemSynthesisConstants::MinSuccessRate,
                ItemSynthesisConstants::MaxSuccessRate);

            randomValue = uni(eng);
        }

        if (randomValue <= successRate)
        {
            CUser::ItemCreate(user, newItemInfo, synthesis.newItemCount);

            GameItemSynthesisOutgoing success{};
            success.result = GameItemSynthesisResult::Success;
            NetworkHelper::Send(user, &success, sizeof(GameItemSynthesisOutgoing));
        }
        else
        {
            GameItemSynthesisOutgoing failure{};
            failure.result = GameItemSynthesisResult::Failure;
            NetworkHelper::Send(user, &failure, sizeof(GameItemSynthesisOutgoing));
        }
    }

    /// <summary>
    /// Handles incoming 0x833 packets. This feature will not be implemented.
    /// </summary>
    void handler_0x833(CUser* user, GameItemFreeSynthesisIncoming* incoming)
    {
    }

    int use_item_ability_transfer(CUser* user, int bag, int slot)
    {
        constexpr int sourceSlot = 0;
        constexpr int cubeSlot = 1;
        constexpr int targetSlot = 2;

        if (!bag || bag > user->bagsUnlocked)
            return 1;

        if (slot != cubeSlot)
            return 1;

        auto& cube = user->inventory[bag][cubeSlot];
        if (!cube)
            return 1;

        if (cube->info->effect != ItemEffect::ItemAbilityTransfer)
            return 1;

        GameItemAbilityTransferOutgoing outgoing{};
        outgoing.result = GameItemAbilityTransferResult::Failure;
        outgoing.srcBag = bag;
        outgoing.srcSlot = sourceSlot;
        outgoing.destBag = bag;
        outgoing.destSlot = targetSlot;

        auto& source = user->inventory[bag][sourceSlot];
        auto& target = user->inventory[bag][targetSlot];
        if (!source || !target)
        {
            NetworkHelper::Send(user, &outgoing, sizeof(GameItemAbilityTransferOutgoing));
            return 1;
        }

        if (!can_transfer_item_ability(source, target))
        {
            NetworkHelper::Send(user, &outgoing, sizeof(GameItemAbilityTransferOutgoing));
            return 1;
        }

        // The inventory layout is fixed:
        // slot 1 = source, slot 2 = cube, slot 3 = target.
        CUser::ItemUseNSend(user, bag, cubeSlot, false);

        target->gems = source->gems;
        ItemHelper::CopyCraftExpansion(source, target);

        source->gems.fill(0);
        ItemHelper::InitCraftExpansion(source);

        ItemHelper::SendDBAgentCraftName(user, target, bag, targetSlot);
        ItemHelper::SendDBAgentGems(user, target, bag, targetSlot);

        ItemHelper::SendDBAgentCraftName(user, source, bag, sourceSlot);
        ItemHelper::SendDBAgentGems(user, source, bag, sourceSlot);

        send_item_slot_update(user, source, bag, sourceSlot);
        send_item_slot_update(user, target, bag, targetSlot);

        outgoing.result = GameItemAbilityTransferResult::Success;
        NetworkHelper::Send(user, &outgoing, sizeof(GameItemAbilityTransferOutgoing));
        return 1;
    }

    /// <summary>
    /// Adds support for 6.4 packets.
    /// </summary>
    void handler(CUser* user, TP_MAIN* packet)
    {
        switch (packet->opcode)
        {
        case 0x830:
        {
            handler_0x830(user, reinterpret_cast<GameChaoticSquareListIncoming*>(packet));
            break;
        }
        case 0x831:
        {
            handler_0x831(user, reinterpret_cast<GameItemSynthesisRecipeIncoming*>(packet));
            break;
        }
        case 0x832:
        {
            handler_0x832(user, reinterpret_cast<GameItemSynthesisIncoming*>(packet));
            break;
        }
        case 0x833:
        {
            handler_0x833(user, reinterpret_cast<GameItemFreeSynthesisIncoming*>(packet));
            break;
        }
        default:
            SConnection::Close(user, 9, 0);
            break;
        }
    }
}

unsigned u0x47A04B = 0x47A04B;
void __declspec(naked) naked_0x47A040()
{
    __asm
    {
        pushad

        push esi // packet
        push edi // user
        call packet_gem::handler
        add esp,0x8

        popad

        jmp u0x47A04B
    }
}

unsigned u0x47A00C = 0x47A00C;
void __declspec(naked) naked_0x47A003()
{
    __asm
    {
        pushad

        push esi // packet
        push edi // user
        call packet_gem::handler_0x806
        add esp,0x8

        popad

        jmp u0x47A00C
    }
}

unsigned u0x46CD3F = 0x46CD3F;
void __declspec(naked) naked_0x46CD38()
{
    __asm
    {
        mov edi,[esp+0x14]
        mov edi,[edi+0x30]
        // ReqRec is reused as a custom lapisian success rate.
        // The game expects the final rate in the scaled 1,000,000 range.
        movzx edi,word ptr[edi+0x3A]
        test edi,edi
        je _original
        imul edi,edi,0x64
        jmp u0x46CD3F

        _original:
        // g_LapisianEnchantSuccessRate
        mov edi,[edx*0x4+0x581C88]
        jmp u0x46CD3F
    }
}

unsigned u0x4D2960 = 0x4D2960;
unsigned u0x46CDB5 = 0x46CDB5;
void __declspec(naked) naked_0x46CDB0()
{
    __asm
    {
        // original
        call u0x4D2960

        pushad

        push ebx // packet
        push ebp // user
        call packet_gem::lapisian_add_protect
        add esp,0x8

        popad

        jmp u0x46CDB5
    }
}

unsigned u0x46D598 = 0x46D598;
unsigned u0x46D3C4 = 0x46D3C4;
void __declspec(naked) naked_0x46D3BC()
{
    __asm
    {
        pushad

        push ebx // packet
        push ebp // user
        call packet_gem::lapisian_add_protect
        add esp,0x8
        test al,al

        popad

        jne _0x46D598

        // original
        movzx eax,byte ptr[ebx+0x7]
        movzx ebx,byte ptr[ebx+0x8]
        jmp u0x46D3C4

        _0x46D598:
        jmp u0x46D598
    }
}

void hook::packet_gem()
{
    // CUser::PacketGem case 0x806
    util::detour((void*)0x47A003, naked_0x47A003, 9);
    // CUser::PacketGem (default case)
    util::detour((void*)0x47A040, naked_0x47A040, 6);
    // CUser::ItemLapisianAdd (perfect lapisian)
    util::detour((void*)0x46CD38, naked_0x46CD38, 7);
    // CUser::ItemLapisianAdd (success)
    util::detour((void*)0x46CDB0, naked_0x46CDB0, 5);
    // CUser::ItemLapisianAdd (failure)
    util::detour((void*)0x46D3BC, naked_0x46D3BC, 8);

}
