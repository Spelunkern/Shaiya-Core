#include <cstddef>
#include <cstdint>

#include <util/util.h>
#include "include/main.h"

namespace
{
    constexpr std::uint32_t kPartyUsers30 = 0x1E;
    constexpr std::uint32_t kPartyUsers150 = 0x96;

    struct OperandPatch
    {
        std::uint32_t from;
        std::uint32_t to;
    };

    constexpr OperandPatch kRaid150OperandPatches[]
    {
        { 0x00000118, 0x000004DC }, // party user array
        { 0x00000130, 0x000004F4 }, // party user array end
        { 0x00000104, 0x000004C8 }, // item division mode
        { 0x00000108, 0x000004CC }, // item division sequence
        { 0x0000010C, 0x000004D0 }, // party exp level cache
        { 0x00000110, 0x000004D4 }, // party user count
        { 0x00000114, 0x000004D8 }, // raid/party flag
        { 0x00000115, 0x000004D9 }, // auto join flag
        { 0x00000148, 0x0000050C },
        { 0x00000150, 0x00000514 }, // CParty allocation size
        { kPartyUsers30, kPartyUsers150 },
    };

    void patch_raid150_operand(std::uint32_t address)
    {
        // Raid 150 component:
        // Most of the CE table only moves CParty fields from the stock 30-player
        // layout to the expanded 150-player layout. Keep this as operand-only
        // surgery: scan the beginning of the original instruction and replace
        // known old constants with their expanded equivalents.
        std::uint8_t* instruction = reinterpret_cast<std::uint8_t*>(address);
        constexpr std::size_t kScanBytes = 6;

        for (const auto& patch : kRaid150OperandPatches)
        {
            const auto oldValue = patch.from;
            const auto newValue = patch.to;
            const auto* oldBytes = reinterpret_cast<const std::uint8_t*>(&oldValue);
            const auto* newBytes = reinterpret_cast<const std::uint8_t*>(&newValue);

            for (std::size_t i = 0; i + sizeof(std::uint32_t) <= kScanBytes; ++i)
            {
                if (instruction[i] == oldBytes[0] &&
                    instruction[i + 1] == oldBytes[1] &&
                    instruction[i + 2] == oldBytes[2] &&
                    instruction[i + 3] == oldBytes[3])
                {
                    util::write_memory(instruction + i, newBytes, sizeof(std::uint32_t));
                    return;
                }
            }

            // Some disassembly rows omit a trailing zero byte for 7-byte
            // instructions. Replacing the low 24 bits is still exact because the
            // high byte is already zero in both old and new offsets.
            for (std::size_t i = 0; i + 3 <= kScanBytes; ++i)
            {
                if (instruction[i] == oldBytes[0] &&
                    instruction[i + 1] == oldBytes[1] &&
                    instruction[i + 2] == oldBytes[2])
                {
                    util::write_memory(instruction + i, newBytes, 3);
                    return;
                }
            }
        }
    }

    unsigned u0x44E563 = 0x44E563;
    unsigned u0x44E575 = 0x44E575;
    void __declspec(naked) raid150_0x44E570()
    {
        __asm
        {
            cmp eax,0x96
            jl _below_limit
            jmp u0x44E575

        _below_limit:
            jmp u0x44E563
        }
    }

    unsigned u0x44E6F4 = 0x44E6F4;
    unsigned u0x44E706 = 0x44E706;
    void __declspec(naked) raid150_0x44E701()
    {
        __asm
        {
            cmp eax,0x96
            jl _below_limit
            jmp u0x44E706

        _below_limit:
            jmp u0x44E6F4
        }
    }

    unsigned u0x44ED13 = 0x44ED13;
    unsigned u0x44ED27 = 0x44ED27;
    void __declspec(naked) raid150_0x44ED22()
    {
        __asm
        {
            cmp ecx,0x96
            jl _below_limit
            jmp u0x44ED27

        _below_limit:
            jmp u0x44ED13
        }
    }

    unsigned u0x44ED58 = 0x44ED58;
    unsigned u0x44ED6B = 0x44ED6B;
    void __declspec(naked) raid150_0x44ED66()
    {
        __asm
        {
            cmp eax,0x96
            jl _below_limit
            jmp u0x44ED6B

        _below_limit:
            jmp u0x44ED58
        }
    }

    unsigned u0x44EDB5 = 0x44EDB5;
    void __declspec(naked) raid150_0x44EDB0()
    {
        __asm
        {
            cmp dword ptr [ebp+0x10],0x96
            push esi
            jmp u0x44EDB5
        }
    }

    unsigned u0x44F12F = 0x44F12F;
    void __declspec(naked) raid150_0x44F128()
    {
        __asm
        {
            cmp eax,0x96
            mov [esp+0x24],eax
            jmp u0x44F12F
        }
    }

    unsigned u0x44F587 = 0x44F587;
    unsigned u0x44F5AB = 0x44F5AB;
    void __declspec(naked) raid150_0x44F5A6()
    {
        __asm
        {
            cmp eax,0x96
            jl _below_limit
            jmp u0x44F5AB

        _below_limit:
            jmp u0x44F587
        }
    }

    unsigned u0x4568F2 = 0x4568F2;
    unsigned u0x4568FF = 0x4568FF;
    void __declspec(naked) raid150_0x4568FA()
    {
        __asm
        {
            cmp ecx,0x96
            jl _below_limit
            jmp u0x4568FF

        _below_limit:
            jmp u0x4568F2
        }
    }

    unsigned u0x4658A0 = 0x4658A0;
    unsigned u0x4658A4 = 0x4658A4;
    void __declspec(naked) raid150_0x46589B()
    {
        __asm
        {
            cmp esi,0x96
            jle _within_limit
            jmp u0x4658A0

        _within_limit:
            jmp u0x4658A4
        }
    }

    unsigned u0x4659C1 = 0x4659C1;
    unsigned u0x4659C5 = 0x4659C5;
    void __declspec(naked) raid150_0x4659BC()
    {
        __asm
        {
            cmp esi,0x96
            jle _within_limit
            jmp u0x4659C1

        _within_limit:
            jmp u0x4659C5
        }
    }

    unsigned u0x47580D = 0x47580D;
    unsigned u0x4757FD = 0x4757FD;
    void __declspec(naked) raid150_0x4757F7()
    {
        __asm
        {
            cmp dword ptr [eax+0x10],0x96
            jl _below_limit
            jmp u0x4757FD

        _below_limit:
            jmp u0x47580D
        }
    }

    unsigned u0x4A1E04 = 0x4A1E04;
    unsigned u0x4A1E08 = 0x4A1E08;
    void __declspec(naked) raid150_0x4A1DFF()
    {
        __asm
        {
            cmp esi,0x96
            jle _within_limit
            jmp u0x4A1E04

        _within_limit:
            jmp u0x4A1E08
        }
    }

    unsigned u0x4A1EDD = 0x4A1EDD;
    unsigned u0x4A1EE1 = 0x4A1EE1;
    void __declspec(naked) raid150_0x4A1ED8()
    {
        __asm
        {
            cmp esi,0x96
            jle _within_limit
            jmp u0x4A1EDD

        _within_limit:
            jmp u0x4A1EE1
        }
    }
}

void hook::raid_150()
{
    // Raid 150 component.
    // Direct operand patches generated from C:\Users\Franco\Desktop\Raid.CT.
    const std::uint32_t directPatchAddresses[]
    {
        0x0044DFD5, 0x0044DFE7, 0x0044E045, 0x0044E059, 0x0044E08B, 0x0044E091, 0x0044E097, 0x0044E0A6,
        0x0044E0C1, 0x0044E0C7, 0x0044E0CD, 0x0044E0E0, 0x0044E0F0, 0x0044E11C, 0x0044E133, 0x0044E156,
        0x0044E170, 0x0044E1CB, 0x0044E219, 0x0044E261, 0x0044E2A0, 0x0044E2AF, 0x0044E2C2, 0x0044E2F2,
        0x0044E322, 0x0044E331, 0x0044E34A, 0x0044E379, 0x0044E4C3, 0x0044E4C3, 0x0044E551, 0x0044E567,
        0x0044E597, 0x0044E5CE, 0x0044E5D4, 0x0044E611, 0x0044E62C, 0x0044E645, 0x0044E669, 0x0044E6E5,
        0x0044E708, 0x0044E70E, 0x0044E7C8, 0x0044E8B7, 0x0044E8C4, 0x0044E967, 0x0044E979, 0x0044EA14,
        0x0044EAA4, 0x0044EB85, 0x0044EC06, 0x0044EC75, 0x0044EC8D, 0x0044ECA8, 0x0044ECBF, 0x0044ECD0,
        0x0044ECE0, 0x0044ED43, 0x0044ED4D, 0x0044EDBE, 0x0044EF59, 0x0044EF6B, 0x0044EF78, 0x0044EF88,
        0x0044F17A, 0x0044F24C, 0x0044F4E7, 0x0044F5F6, 0x0044F641, 0x0044F6B3, 0x0044F71A, 0x0044F725,
        0x0044F734, 0x0044F73F, 0x0044F76E, 0x0044F792, 0x0044F7C2, 0x0044F969, 0x0044FA81, 0x0044FB22,
        0x0044FB2D, 0x0044FB61, 0x0044FDEB, 0x0044FE57, 0x0044FEC8, 0x0044FED3, 0x0044FEE2, 0x0044FEED,
        0x0044FF2F, 0x0044FF53, 0x0044FF83, 0x00450129, 0x00450238, 0x00450307, 0x004504F1, 0x004504FE,
        0x00450521, 0x004505A1, 0x00450622, 0x004506A1, 0x00450719, 0x004509DE, 0x00450B09, 0x00450B11,
        0x00450B17, 0x00450CBB, 0x00450CC2, 0x00452253, 0x00455C69, 0x004600EE, 0x00460272, 0x0046028E,
        0x00460494, 0x004604B4, 0x0046066D, 0x0046068D, 0x0046083E, 0x004609E0, 0x00460B87, 0x00465134,
        0x0046583C, 0x0046584D, 0x004658D8, 0x004659F9, 0x00465D80, 0x00465DB8, 0x00465E05, 0x00465F95,
        0x00465FFF, 0x00467419, 0x00467443, 0x00467758, 0x0046776F, 0x004677CA, 0x004677D2, 0x004677DC,
        0x00467804, 0x0046783B, 0x00467861, 0x00467869, 0x00467873, 0x0046789B, 0x004678C4, 0x0046A871,
        0x0046B401, 0x0046B675, 0x004734A2, 0x00475423, 0x00475658, 0x004757DE, 0x0047583A, 0x004758C7,
        0x004758D6, 0x004758E6, 0x004758FC, 0x00475907, 0x00475912, 0x00475927, 0x00475976, 0x00475986,
        0x004759E4, 0x004759F4, 0x00475A0A, 0x00475A4C, 0x00475A99, 0x00475B1B, 0x00475CF0, 0x00475D46,
        0x00475D5C, 0x00475DA2, 0x00475DF5, 0x00475E73, 0x00476926, 0x00476991, 0x00478862, 0x00478EBF,
        0x0047F92E, 0x0047F95F, 0x0047F971, 0x00480266, 0x00480297, 0x004802A9, 0x00482D6A, 0x00482E2E,
        0x00483C42, 0x00483C6F, 0x004851CF, 0x00485395, 0x004856D0, 0x004908D4, 0x00490905, 0x0049093C,
        0x004909E5, 0x00490A16, 0x00490A4D, 0x00490AF5, 0x00490B26, 0x00490B5D, 0x00490C8A, 0x00490CBF,
        0x00490D3E, 0x004911AE, 0x0049124A, 0x0049127B, 0x004912DA, 0x0049130E, 0x00491767, 0x004917A4,
        0x00491807, 0x00491876, 0x004918AB, 0x0049190D, 0x004919C1, 0x00491A12, 0x0049B131, 0x0049B15B,
        0x0049B184, 0x0049B1A6, 0x0049B1BF, 0x0049B1F3, 0x0049B21F, 0x0049B232, 0x0049BBEF, 0x0049BC2D,
        0x0049BC84, 0x0049E4C6, 0x0049E589, 0x004A1DAD, 0x004A1DBE, 0x004A1E3C, 0x004A1F15, 0x004BA98B,
        0x004BAFE2, 0x004BB1DD, 0x004BB1E6, 0x004BB25B, 0x004BB2F0, 0x004BB2F8, 0x004BB302, 0x004BB327,
        0x004BB370, 0x004BB397, 0x004BB39F, 0x004BB3A9, 0x004BB3D3, 0x004BB424, 0x004C6B30, 0x004C6B54,
    };

    for (auto address : directPatchAddresses)
        patch_raid150_operand(address);

    util::detour((void*)0x44E570, raid150_0x44E570, 5);
    util::detour((void*)0x44E701, raid150_0x44E701, 5);
    util::detour((void*)0x44ED22, raid150_0x44ED22, 5);
    util::detour((void*)0x44ED66, raid150_0x44ED66, 5);
    util::detour((void*)0x44EDB0, raid150_0x44EDB0, 5);
    util::detour((void*)0x44F128, raid150_0x44F128, 7);
    util::detour((void*)0x44F5A6, raid150_0x44F5A6, 5);
    util::detour((void*)0x4568FA, raid150_0x4568FA, 5);
    util::detour((void*)0x46589B, raid150_0x46589B, 5);
    util::detour((void*)0x4659BC, raid150_0x4659BC, 5);
    util::detour((void*)0x4757F7, raid150_0x4757F7, 6);
    util::detour((void*)0x4A1DFF, raid150_0x4A1DFF, 5);
    util::detour((void*)0x4A1ED8, raid150_0x4A1ED8, 5);
}
