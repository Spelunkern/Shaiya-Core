#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <map>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <util/util.h>
#include <util/ini/ini.h>
#include <shaiya/include/network/game/RewardItemUnit.h>
#include "include/extensions/filesystem.hpp"
#include "BattlefieldMoveInfo.h"
#include "Configuration.h"
#include "ItemInfo.h"
#include "ItemSynthesis.h"
#include "RewardItem.h"
#include "SBinaryReader.h"
#include "Synergy.h"
using namespace shaiya;

namespace
{
    constexpr int DefaultEnchantCap = 20;
    constexpr int DefaultLevelCap = 70;
    constexpr int MinEnchantCap = 1;
    // Hard safety limit for the INI-driven enchant cap. The patched byte can
    // technically hold higher values, but production configs should not allow
    // enchant caps above 49 because higher values can destabilize balance and
    // related item calculations.
    constexpr int MaxEnchantCap = 49;
    constexpr int MinLevelCap = 1;
    constexpr int MaxLevelCap = 254;
    constexpr uintptr_t EnchantCapAddress = 0x46CCC1;
    constexpr uintptr_t LevelCapAddresses[]
    {
        0x460C57,
        0x460C87,
        0x460CB7,
        0x4612CE,
        0x4612F9,
        0x461324,
        0x46135D,
        0x4613AE,
        0x4613FB,
        0x461440,
        0x464FF7,
        0x465080,
        0x465161,
        0x4651DC,
        0x4651EC,
        0x465241,
        0x467BFE,
        0x467C20,
        0x467C99,
        0x480E0E,
        0x480FC6,
        0x49241D,
        0x49243D,
        0x49B4A4,
        0x49B4D5,
        0x49B506,
        0x49B537,
        0x49B564,
        0x49B591,
        0x49B5E0,
        0x49B63C,
        0x49B690,
        0x49B6DC,
        0x49B71E,
        0x49B760,
        0x49B7C4,
        0x49B820,
        0x49B874,
        0x49B8C0,
        0x49B902,
        0x49B944,
        0x49BB77,
        0x49BB9E,
        0x49BDA0,
        0x49BDD1,
        0x49BE02,
        0x49BE37,
        0x49BE64,
        0x49BE91,
        0x49BEDD,
        0x49BF36,
        0x49BF83,
        0x49BFD3,
        0x49C015,
        0x49C054,
        0x49C0B6,
        0x49C10F,
        0x49C15C,
        0x49C1AC,
        0x49C1EE,
        0x49C22D
    };
}

void Configuration::Init()
{
    std::wstring buffer(INT16_MAX, 0);
    auto count = GetModuleFileNameW(nullptr, buffer.data(), INT16_MAX);
    if (!count)
        return;

    auto first = buffer.begin();
    auto last = first + count;
    m_root.assign(first, last).remove_filename();
}

void Configuration::LoadServerConfig()
{
    try
    {
        std::filesystem::path path(m_root);
        ext::filesystem::combine(path, "Data", "ServerConfig.ini");

        if (!std::filesystem::exists(path))
        {
            util::ini::write_pair(L"Server", L"EnchantCap", L"20", path);
            util::ini::write_pair(L"Server", L"LevelCap", L"70", path);
        }

        auto enchantCap = static_cast<int>(util::ini::get_value(L"Server", L"EnchantCap", DefaultEnchantCap, path));
        enchantCap = std::clamp(enchantCap, MinEnchantCap, MaxEnchantCap);

        auto enchantCapValue = static_cast<uint8_t>(enchantCap);
        util::write_memory(reinterpret_cast<void*>(EnchantCapAddress), &enchantCapValue, sizeof(enchantCapValue));

        auto levelCap = static_cast<int>(util::ini::get_value(L"Server", L"LevelCap", DefaultLevelCap, path));
        levelCap = std::clamp(levelCap, MinLevelCap, MaxLevelCap);

        auto levelCapValue = static_cast<uint8_t>(levelCap);
        for (auto address : LevelCapAddresses)
            util::write_memory(reinterpret_cast<void*>(address), &levelCapValue, sizeof(levelCapValue));
    }
    catch (...)
    {
        auto enchantCapValue = static_cast<uint8_t>(DefaultEnchantCap);
        util::write_memory(reinterpret_cast<void*>(EnchantCapAddress), &enchantCapValue, sizeof(enchantCapValue));

        auto levelCapValue = static_cast<uint8_t>(DefaultLevelCap);
        for (auto address : LevelCapAddresses)
            util::write_memory(reinterpret_cast<void*>(address), &levelCapValue, sizeof(levelCapValue));
    }
}

void Configuration::LoadBattlefieldMoveData()
{
    try
    {
        std::filesystem::path path(m_root);
        ext::filesystem::combine(path, "Data", "BattleFieldMoveInfo.ini");

        if (!std::filesystem::exists(path))
            return;

        auto count = static_cast<int>(util::ini::get_value(L"BATTLEFIELD_INFO", L"BATTLEFIELD_COUNT", 0, path));
        if (count <= 0)
            return;

        g_battlefieldMoveData.clear();
        g_battlefieldMoveData.reserve(count);

        for (int num = 1; num <= count; ++num)
        {
            auto section = std::format(L"BATTLEFIELD_{}", num);
            auto mapId = static_cast<int>(util::ini::get_value(section.c_str(), L"MAP_NO", -1, path));
            if (mapId < 0)
                continue;

            BattlefieldMoveInfo info{};
            info.levelMin = static_cast<int>(util::ini::get_value(section.c_str(), L"LEVEL_MIN", 0, path));
            info.levelMax = static_cast<int>(util::ini::get_value(section.c_str(), L"LEVEL_MAX", 0, path));

            auto readPos = [&](int index, const wchar_t* prefix)
            {
                auto keyX = std::format(L"{}_POSX", prefix);
                auto keyY = std::format(L"{}_POSY", prefix);
                auto keyZ = std::format(L"{}_POSZ", prefix);

                auto x = util::ini::get_value(section.c_str(), keyX.c_str(), L"", path);
                auto y = util::ini::get_value(section.c_str(), keyY.c_str(), L"", path);
                auto z = util::ini::get_value(section.c_str(), keyZ.c_str(), L"", path);

                info.mapPos[index].mapId = mapId;
                info.mapPos[index].x = std::stof(x);
                info.mapPos[index].y = std::stof(y);
                info.mapPos[index].z = std::stof(z);
            };

            readPos(0, L"HU");
            readPos(1, L"EL");
            readPos(2, L"DE");
            readPos(3, L"VI");

            g_battlefieldMoveData.push_back(info);
        }
    }
    catch (...)
    {
        g_battlefieldMoveData.clear();
    }
}

void Configuration::LoadItemSetData()
{
    try
    {
        std::filesystem::path path(m_root);
        ext::filesystem::combine(path, "Data", "SetItem.SData");

        if (!std::filesystem::exists(path))
            return;

        SBinaryReader reader(path);

        auto itemSetCount = reader.readUInt32();
        for (size_t i = 0; i < itemSetCount; ++i)
        {
            ItemSet itemSet{};
            itemSet.id = reader.readUInt16();

            // Discard the name
            auto length = reader.readUInt32();
            reader.ignore(length);

            for (auto&& itemId : itemSet.items)
            {
                auto type = reader.readUInt16();
                auto typeId = reader.readUInt16();

                auto value = (type * 1000) + typeId;
                if (value >= ItemId_MIN && value <= ItemId_MAX)
                    itemId = value;
            }

            for (auto&& synergy : itemSet.synergies)
            {
                // e.g., 70,50,0,0,0,20,0,0,0,0,0,0
                auto effects = reader.readString();
                auto rng = std::views::split(effects, ',');
                auto vec = std::ranges::to<std::vector<std::string>>(rng);
                if (vec.size() != synergy.effects.size())
                    continue;

                for (size_t i = 0; i < synergy.effects.size(); ++i)
                    synergy.effects[i] = std::stoi(vec[i]);
            }

            g_itemSets.push_back(itemSet);
        }

        reader.close();
    }
    catch (...)
    {
        g_itemSets.clear();
    }
}

void Configuration::LoadItemSynthesis()
{
    try
    {
        std::filesystem::path path(m_root);
        ext::filesystem::combine(path, "Data", "ChaoticSquare.ini");

        if (!std::filesystem::exists(path))
            return;

        auto sections = util::ini::get_sections(path);
        for (const auto& section : sections)
        {
            auto pairs = util::ini::get_pairs(section.c_str(), path);
            if (pairs.size() != 8)
                continue;

            auto itemId = std::stoi(pairs[0].second);
            if (itemId < ItemId_MIN || itemId > ItemId_MAX)
                continue;

            auto successRate = std::stoi(pairs[1].second);
            successRate = std::clamp(successRate, 1, 100);

            ItemSynthesis synthesis{};
            synthesis.successRate = successRate * 100;

            auto view = std::views::zip(
                synthesis.materialType,
                synthesis.materialTypeId,
                synthesis.materialCount
            );

            auto rng1 = std::views::split(pairs[2].second, L',');
            auto vec1 = std::ranges::to<std::vector<std::wstring>>(rng1);
            if (vec1.size() != view.size())
                continue;

            auto rng2 = std::views::split(pairs[3].second, L',');
            auto vec2 = std::ranges::to<std::vector<std::wstring>>(rng2);
            if (vec2.size() != view.size())
                continue;

            auto rng3 = std::views::split(pairs[4].second, L',');
            auto vec3 = std::ranges::to<std::vector<std::wstring>>(rng3);
            if (vec3.size() != view.size())
                continue;

            for (size_t i = 0; i < view.size(); ++i)
            {
                auto type = std::stoi(vec1[i]);
                if (!std::in_range<uint8_t>(type))
                    continue;

                auto typeId = std::stoi(vec2[i]);
                if (!std::in_range<uint8_t>(typeId))
                    continue;

                auto count = std::stoi(vec3[i]);
                if (!std::in_range<uint8_t>(count))
                    continue;

                std::get<0>(view[i]) = type;
                std::get<1>(view[i]) = typeId;
                std::get<2>(view[i]) = count;
            }

            auto type = std::stoi(pairs[5].second);
            if (!std::in_range<uint8_t>(type))
                continue;

            auto typeId = std::stoi(pairs[6].second);
            if (!std::in_range<uint8_t>(typeId))
                continue;

            auto count = std::stoi(pairs[7].second);
            if (!std::in_range<uint8_t>(count))
                continue;

            synthesis.newItemType = type;
            synthesis.newItemTypeId = typeId;
            synthesis.newItemCount = count;

            if (auto it = g_itemSyntheses.find(itemId); it != g_itemSyntheses.end())
                it->second.push_back(synthesis);
            else
                g_itemSyntheses.insert({ itemId, { synthesis } });
        }

        for (const auto& [itemId, syntheses] : g_itemSyntheses)
        {
            ChaoticSquare chaoticSquare{};
            chaoticSquare.itemId = itemId;

            auto view = std::views::zip(
                chaoticSquare.newItemType,
                chaoticSquare.newItemTypeId
            );

            size_t index = 0;
            for (const auto& synthesis : syntheses)
            {
                std::get<0>(view[index]) = synthesis.newItemType;
                std::get<1>(view[index]) = synthesis.newItemTypeId;

                ++index;

                if (index < view.size())
                    continue;
                else
                {
                    g_chaoticSquares.push_back(chaoticSquare);
                    std::fill(view.begin(), view.end(), std::tuple(0, 0));
                    index = 0;
                }
            }

            if (!index)
                continue;

            g_chaoticSquares.push_back(chaoticSquare);
        }
    }
    catch (...)
    {
        g_itemSyntheses.clear();
        g_chaoticSquares.clear();
    }
}

void Configuration::LoadRewardItemEvent()
{
    try
    {
        std::filesystem::path path(m_root);
        ext::filesystem::combine(path, "Data", "RewardItem.ini");

        if (!std::filesystem::exists(path))
            return;

        auto sections = util::ini::get_sections(path);
        auto dest = g_rewardItemList.begin();

        for (const auto& section : sections)
        {
            auto pairs = util::ini::get_pairs(section.c_str(), path);
            if (pairs.size() != 4)
                continue;

            auto minutes = std::stoi(pairs[0].second);
            if (minutes <= 0)
                continue;

            auto type = std::stoi(pairs[1].second);
            if (!std::in_range<uint8_t>(type))
                continue;

            auto typeId = std::stoi(pairs[2].second);
            if (!std::in_range<uint8_t>(typeId))
                continue;

            auto count = std::stoi(pairs[3].second);
            if (!std::in_range<uint8_t>(count))
                continue;

            dest->minutes = minutes;
            dest->type = type;
            dest->typeId = typeId;
            dest->count = count;

            ++dest;
            ++g_rewardItemCount;

            if (dest == g_rewardItemList.end())
                break;
        }
    }
    catch (...)
    {
        g_rewardItemCount = 0;
        g_rewardItemList.fill({});
    }
}
