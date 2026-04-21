# Documentation

This library is for the game service. Please read the features section to learn more.

## Environment

Windows 10

Visual Studio 2022

C++ 23

## Prerequisites

[Microsoft Visual C++ Redistributable](https://aka.ms/vs/17/release/vc_redist.x86.exe)


# Features

All features are implemented based on client specifications. The intent is to keep everything as vanilla as possible.

## Stable Server Patch Groups

- Cross-faction whisper, trade, inspect, login, and world-enter support.
- Guild quality-of-life patches: officer limit, penalty removal, repeat GRB entry, and 2-player guild creation.
- Security sanitizers for command, character, and guild strings before DB-bound paths.
- Raid 150 component for expanded raid/party support.
- Rune cutting fixes for arena, auction house, capital, and guild map paths.
- Direct solo gold/item drops, optional gold bonus settings, and Fortune Bag inventory stacking.
- Boss death/spawn notices, instant mounts, revive with max HP/MP/SP, and death-skill trigger.
- Infinite consumables for explicitly listed unsellable item IDs.
- Spell cooldown fix for Ability Type 19 so the server uses the real value instead of the fixed 500s fallback.
- Stabilized Obelisk.ini spawn timing with the one-hour randomizer removed.
- Cloaks contributing real defense/resistance, off-hand support, stealth running, mantle drop fixes, and mantle packet hardening.
- Battleground movement support using the normal cast/update flow.

## Reward Item Event

Event progress is account-wide. The progress of the current item will reset if a character leaves the game world. Do not expect the progress bar to synchronize perfectly. The event will reset when the last item is received.

### Configuration

The client expects no more than 20 items. Use the following example to get started:

```ini
; PSM_Client\Bin\Data\RewardItem.ini

[RewardItem_1]
; minutes
Delay=5
Type=100
TypeID=1
Count=1

[RewardItem_2]
Delay=10
Type=100
TypeID=2
Count=1

[RewardItem_3]
Delay=15
Type=100
TypeID=3
Count=1

[RewardItem_4]
Delay=20
Type=100
TypeID=4
Count=1

[RewardItem_5]
Delay=25
Type=100
TypeID=5
Count=1
```

Add the following system messages:

```
### Recreation Runes

| ItemId | Effect |
|--------|--------|
| 100171 | 62     |

Custom deterministic recreation runes are currently disabled.

Status: **Future feature - broken right now**

The server currently preserves vanilla random recreation behavior for effect `62`. Do not publish custom deterministic rune data as a supported feature until the server-side item data contract is proven and retested.

Historical design notes are kept here for future work only. The intended selector was:

| ReqVg | Result |
|-------|--------|
| 1     | Max STR |
| 2     | Max DEX |
| 3     | Max INT |
| 4     | Max WIS |
| 5     | Max REC |
| 6     | Max LUC |
| 7     | Max HP |
| 8     | Max MP |
| 9     | Max SP |
| 10    | Remove all craft stats |

## NpcQuest

The episode 6 format has 6 quest results, each containing up to 3 items. The game service executable has been modified to read the file format.

## Skill Abilities

This library adds support for episode 6 skill abilities 70, 87, and 35 (exp stones). **It does not affect other skills.**

| SkillId | Ability | Supported          |
|---------|---------|--------------------|
| 222     | 35      | :white_check_mark: |
| 223     | 35      | :white_check_mark: |
| 272     | 35      | :white_check_mark: |
| 375     | 52      | :x:                |
| 376     | 53      | :x:                |
| 377     | 54      | :x:                |
| 378     | 55      | :x:                |
| 379     | 56      | :x:                |
| 380     | 57      | :x:                |
| 398     | 70      | :white_check_mark: |
| 399     | 70      | :white_check_mark: |
| 400     | 70      | :white_check_mark: |
| 401     | 70      | :white_check_mark: |
| 396     | 73      | :x:                |
| 397     | 74      | :x:                |
| 412     | 78      | :x:                |
| 426     | 35      | :white_check_mark: |
| 427     | 35      | :white_check_mark: |
| 432     | 87      | :white_check_mark: |
| 434     | 35      | :white_check_mark: |

### Skill Ability 35

The original code multiplies exp depending on the value of two `CUser` booleans:

```
// 00574080 (2.0)
bool32_t multiplyExp2;  //0x596C
// 00574090 (1.5)
bool32_t multiplyExp1;  //0x5970 (not used)
```

The library modifies `CUser::AddExpFromUser` to support 6.4 skill ability values.

| ItemId | SkillId | SkillLv | AbilityValue |
|--------|---------|---------|--------------|
| 100042 | 222     | 1       | 150          |
| 100043 | 223     | 1       | 150          |
| 101114 | 426     | 2       | 500          |
| 101115 | 426     | 3       | 1000         |
| 101117 | 427     | 2       | 500          |
| 101121 | 434     | 2       | 200          |
| 101122 | 434     | 3       | 200          |

The ability value is expected to be greater than 100. The library will divide the ability value by 100.

### Skill Ability 70

The effect(s) will be removed a few seconds after the skill has been stopped.

### Skill Ability 87

Use the following items to get started:

| ItemId | SkillId | SkillLv |
|--------|---------|---------|
| 101112 | 432     | 2       |
| 101113 | 432     | 3       |

The ability value is expected to be greater than 100. The library will divide the ability value by 100.

## Chaotic Squares

### ChaoticSquare.ini

Use the following example to get started:

```ini
; PSM_Client\Bin\Data\ChaoticSquare.ini

[ChaoticSquare_1]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
NewItemType=30
NewItemTypeID=6
NewItemCount=1

[ChaoticSquare_2]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=12,12,12,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
NewItemType=30
NewItemTypeID=13
NewItemCount=1

[ChaoticSquare_3]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=19,19,19,19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
NewItemType=30
NewItemTypeID=20
NewItemCount=1

[ChaoticSquare_4]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=26,26,26,26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
NewItemType=30
NewItemTypeID=27
NewItemCount=1

[ChaoticSquare_5]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=33,33,33,33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
NewItemType=30
NewItemTypeID=34
NewItemCount=1

[ChaoticSquare_6]
ItemID=102073
SuccessRate=80
MaterialType=30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialTypeID=40,40,40,40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
MaterialCount=1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
NewItemType=30
NewItemTypeID=41
NewItemCount=1
```

### Money

The gold per percentage value in the library is the same as the official server.

### Crafting Hammers

The **ReqVg** value is the success rate. The library will multiply the value by 100.

| ItemId | Effect | ReqVg |
|--------|--------|-------|
| 102074 | 102    | 5     |
| 102075 | 102    | 10    |

## Synergy

The library expects **SetItem.SData** to be in the **PSM_Client/Bin/Data** directory.

### Client Format

The client expects the file to be encrypted.

![Capture2](https://github.com/kurtekat/shaiya-episode-6/assets/142125482/01b93010-05a5-4323-b8d5-e890551ed4b7)

#### Bonus Description

`" Sinergia [5]\n- LUC +20\n- DEX +50\n- STR +70"`

### Server Format

The library expects the file to be decrypted. The bonus text is expected to be 12 comma-separated values.

![Capture](https://github.com/kurtekat/shaiya-episode-6/assets/142125482/a0a0c116-c5a0-4443-953e-35077e29f065)

The values are signed 32-bit integers, expected to be in the following order:

* strength
* dexterity
* intelligence
* wisdom
* reaction
* luck
* health
* mana
* stamina
* attackPower
* rangedAttackPower
* magicPower

## Item Ability Transfer

Use item `101150` directly from the inventory. This system does not use the client transfer window.

Inventory layout:

* slot 1 = source item
* slot 2 = transfer cube
* slot 3 = target item

When the cube is used, the transfer happens immediately. The source item loses its `CraftName` and `Gems`, and the target item receives them.

| ItemId | Effect |
|--------|--------|
| 101150 | 105    |

### Success Rate

The transfer is always successful. Catalysts are not used.

## Perfect Lapisian

The vanilla items define a perfect success rate (10000).

```cpp
auto materialCount = 10;
auto reqRec = 10000;

auto rate = reqRec * 100; // 1000000
rate += materialCount;    // 1000010
```

If `ReqRec` is zero, the game service will get the rate from `g_LapisianEnchantSuccessRate`.

### Configuration

| Column         | Value    | Description               |
|----------------|----------|---------------------------|
| Level          | 0:1      | Can use with Weapons      |
| Country        | 0:1      | Can use with Helmets      |
| AttackFighter  | 0:1      | Can use with Upper Armor  |
| DefenseFighter | 0:1      | Can use with Lower Armor  |
| PatrolRogue    | 0:1      | Can use with Shields      |
| ShootRogue     | 0:1      | Can use with Gloves       |
| AttackMage     | 0:1      | Can use with Boots        |
| ReqRec         | 0:10000  | Success rate              |
| ReqVg          | 0:1      | Needs item protection     |

## Item Effects

### Safety Charms

| ItemId | Effect |
|--------|--------|
| 101090 | 103    |
| 101132 | 103    |

### Town Move Scrolls

Town move scrolls now use the item `ReqVg` value as the `NpcTypeID` of the
gatekeeper in map `2`.

| Effect | ReqVg |
|--------|-------|
| 104    | GateKeeper NpcTypeID |
```
