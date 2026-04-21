#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <array>
#include <cstring>
#include <util/util.h>
#include "include/main.h"

namespace
{
#define RES_IDX_1366_768  14
#define RES_IDX_1400_900  15
#define RES_IDX_2560_1080 16
#define RES_IDX_2560_1440 17
#define RES_IDX_3840_1080 18
#define RES_IDX_3840_2160 19
#define RES_IDX_3440_1440 20

#define RESOLUTION_COUNT_ADDR 0x007ADEB4
#define SAVED_RESOLUTION_INDEX_ADDR 0x007C0DFC
#define RUNTIME_RESOLUTION_INDEX_ADDR 0x022B0224
#define RUNTIME_WIDTH_ADDR 0x007AB0E8
#define RUNTIME_HEIGHT_ADDR 0x007AB0EC
#define RESOLUTION_TABLE_INDEX_14_ADDR 0x022B03D8
#define INI_FILE_ADDR 0x007C0720
#define INI_VIDEO_ADDR 0x00746E38
#define INI_SIZE_X_ADDR 0x00746E30
#define INI_SIZE_Y_ADDR 0x00746E28

    struct ResolutionEntry
    {
        uint32_t id;
        char text[20];
    };

    static_assert(sizeof(ResolutionEntry) == 0x18);

    const char k1366[] = "1366";
    const char k1400[] = "1400";
    const char k2560[] = "2560";
    const char k3440[] = "3440";
    const char k3840[] = "3840";
    const char k768[] = "768";
    const char k900[] = "900";
    const char k1080[] = "1080";
    const char k1440[] = "1440";
    const char k2160[] = "2160";

    const std::array<ResolutionEntry, 7> kExtraResolutions
    {{
        { RES_IDX_1366_768,  "1366x768"  },
        { RES_IDX_1400_900,  "1400x900"  },
        { RES_IDX_2560_1080, "2560x1080" },
        { RES_IDX_2560_1440, "2560x1440" },
        { RES_IDX_3840_1080, "3840x1080" },
        { RES_IDX_3840_2160, "3840x2160" },
        { RES_IDX_3440_1440, "3440x1440" },
    }};

    void install_resolution_table()
    {
        const uint32_t count = 21; // stock 0..13 plus new 14..20
        util::write_memory(reinterpret_cast<void*>(RESOLUTION_COUNT_ADDR), &count, sizeof(count));
        util::write_memory(reinterpret_cast<void*>(RESOLUTION_TABLE_INDEX_14_ADDR),
            kExtraResolutions.data(), sizeof(ResolutionEntry) * kExtraResolutions.size());
    }

    uint32_t extra_resolution_index_from_size(uint32_t width, uint32_t height)
    {
        if (width == 1366 && height == 768)
            return RES_IDX_1366_768;

        if (width == 1400 && height == 900)
            return RES_IDX_1400_900;

        if (width == 2560 && height == 1080)
            return RES_IDX_2560_1080;

        if (width == 2560 && height == 1440)
            return RES_IDX_2560_1440;

        if (width == 3840 && height == 1080)
            return RES_IDX_3840_1080;

        if (width == 3840 && height == 2160)
            return RES_IDX_3840_2160;

        if (width == 3440 && height == 1440)
            return RES_IDX_3440_1440;

        return 0;
    }

    DWORD renderNewResReturn = 0x51E869;
    DWORD renderNewResContinue = 0x51E872;

    __declspec(naked) void render_new_res()
    {
        __asm
        {
            pushad
            call install_resolution_table
            popad

            cmp dword ptr [edx],edi
            je _continue
            inc eax
            jmp renderNewResReturn

        _continue:
            jmp renderNewResContinue
        }
    }

    DWORD alterLimitResReturn = 0x51E84F;

    __declspec(naked) void alter_limit_res()
    {
        __asm
        {
            mov dword ptr ds:[RESOLUTION_COUNT_ADDR],21
            mov ecx,dword ptr ds:[RESOLUTION_COUNT_ADDR]
            jmp alterLimitResReturn
        }
    }

    DWORD alterLimitOkReturn = 0x51D742;

    __declspec(naked) void alter_limit_ok()
    {
        __asm
        {
            mov dword ptr ds:[RESOLUTION_COUNT_ADDR],21
            jmp alterLimitOkReturn
        }
    }

    DWORD renderOptionRessLoginReturn = 0x51AF01;

    __declspec(naked) void render_option_res_login()
    {
        __asm
        {
            push eax
            push ecx
            push edx
            mov ecx,dword ptr ds:[RUNTIME_WIDTH_ADDR]
            mov edx,dword ptr ds:[RUNTIME_HEIGHT_ADDR]
            push edx
            push ecx
            call extra_resolution_index_from_size
            add esp,8
            test eax,eax
            je _stock
            mov dword ptr ds:[RUNTIME_RESOLUTION_INDEX_ADDR],eax
            pop edx
            pop ecx
            pop eax
            jmp renderOptionRessLoginReturn

        _stock:
            pop edx
            pop ecx
            pop eax
            mov dword ptr ds:[RESOLUTION_TABLE_INDEX_14_ADDR],eax
            jmp renderOptionRessLoginReturn
        }
    }

    DWORD renderOptionRessLoginReturn2 = 0x51AEDB;

    __declspec(naked) void render_option_res_login2()
    {
        __asm
        {
            push eax
            push ecx
            push edx
            mov ecx,dword ptr ds:[RUNTIME_WIDTH_ADDR]
            mov edx,dword ptr ds:[RUNTIME_HEIGHT_ADDR]
            push edx
            push ecx
            call extra_resolution_index_from_size
            add esp,8
            test eax,eax
            je _stock
            mov dword ptr ds:[RUNTIME_RESOLUTION_INDEX_ADDR],eax
            pop edx
            pop ecx
            pop eax
            jmp renderOptionRessLoginReturn2

        _stock:
            pop edx
            pop ecx
            pop eax
            mov dword ptr ds:[RUNTIME_RESOLUTION_INDEX_ADDR],eax
            jmp renderOptionRessLoginReturn2
        }
    }

    DWORD setNewResReturn = 0x0051B314;
    DWORD interfaceContinue = 0x0051B351;
    DWORD setNewResOutOfRange = 0x0051B696;

    __declspec(naked) void set_new_res()
    {
        __asm
        {
            cmp eax,RES_IDX_1366_768
            je _1366x768
            cmp eax,RES_IDX_1400_900
            je _1400x900
            cmp eax,RES_IDX_2560_1080
            je _2560x1080
            cmp eax,RES_IDX_2560_1440
            je _2560x1440
            cmp eax,RES_IDX_3840_1080
            je _3840x1080
            cmp eax,RES_IDX_3840_2160
            je _3840x2160
            cmp eax,RES_IDX_3440_1440
            je _3440x1440
            cmp eax,0x0D
            jbe _stock
            jmp setNewResOutOfRange

        _stock:
            jmp setNewResReturn

        _1366x768:
            push INI_FILE_ADDR
            push offset k1366
            push INI_SIZE_X_ADDR
            push INI_VIDEO_ADDR
            call esi
            push INI_FILE_ADDR
            push offset k768
            push INI_SIZE_Y_ADDR
            push INI_VIDEO_ADDR
            call esi
            mov ecx,1366
            mov eax,768
            jmp interfaceContinue

        _1400x900:
            push INI_FILE_ADDR
            push offset k1400
            push INI_SIZE_X_ADDR
            push INI_VIDEO_ADDR
            call esi
            push INI_FILE_ADDR
            push offset k900
            push INI_SIZE_Y_ADDR
            push INI_VIDEO_ADDR
            call esi
            mov ecx,1400
            mov eax,900
            jmp interfaceContinue

        _2560x1080:
            push INI_FILE_ADDR
            push offset k2560
            push INI_SIZE_X_ADDR
            push INI_VIDEO_ADDR
            call esi
            push INI_FILE_ADDR
            push offset k1080
            push INI_SIZE_Y_ADDR
            push INI_VIDEO_ADDR
            call esi
            mov ecx,2560
            mov eax,1080
            jmp interfaceContinue

        _2560x1440:
            push INI_FILE_ADDR
            push offset k2560
            push INI_SIZE_X_ADDR
            push INI_VIDEO_ADDR
            call esi
            push INI_FILE_ADDR
            push offset k1440
            push INI_SIZE_Y_ADDR
            push INI_VIDEO_ADDR
            call esi
            mov ecx,2560
            mov eax,1440
            jmp interfaceContinue

        _3840x1080:
            push INI_FILE_ADDR
            push offset k3840
            push INI_SIZE_X_ADDR
            push INI_VIDEO_ADDR
            call esi
            push INI_FILE_ADDR
            push offset k1080
            push INI_SIZE_Y_ADDR
            push INI_VIDEO_ADDR
            call esi
            mov ecx,3840
            mov eax,1080
            jmp interfaceContinue

        _3840x2160:
            push INI_FILE_ADDR
            push offset k3840
            push INI_SIZE_X_ADDR
            push INI_VIDEO_ADDR
            call esi
            push INI_FILE_ADDR
            push offset k2160
            push INI_SIZE_Y_ADDR
            push INI_VIDEO_ADDR
            call esi
            mov ecx,3840
            mov eax,2160
            jmp interfaceContinue

        _3440x1440:
            push INI_FILE_ADDR
            push offset k3440
            push INI_SIZE_X_ADDR
            push INI_VIDEO_ADDR
            call esi
            push INI_FILE_ADDR
            push offset k1440
            push INI_SIZE_Y_ADDR
            push INI_VIDEO_ADDR
            call esi
            mov ecx,3440
            mov eax,1440
            jmp interfaceContinue
        }
    }

    DWORD isSaveResReturn = 0x40659B;
    DWORD isSaveContinue = 0x406774;

    __declspec(naked) void save_res_index()
    {
        __asm
        {
            push eax
            push ecx
            push edx
            mov edx,eax
            push edx
            push ecx
            call extra_resolution_index_from_size
            add esp,8
            test eax,eax
            je _stock
            mov dword ptr ds:[SAVED_RESOLUTION_INDEX_ADDR],eax
            pop edx
            pop ecx
            pop eax
            jmp isSaveContinue

        _stock:
            pop edx
            pop ecx
            pop eax
            mov dword ptr ds:[SAVED_RESOLUTION_INDEX_ADDR],1
            jmp isSaveResReturn
        }
    }

    DWORD is1400addr = 0x54F80C;
    DWORD is1366addr = 0x54F849;
    DWORD is3840addr = 0x54F894;
    DWORD isOriginaladdr = 0x54F849;
    DWORD setAdjustInterfaceReturn = 0x54F779;

    __declspec(naked) void adjust_interface1()
    {
        __asm
        {
            cmp eax,0x0E
            je _1366
            cmp eax,0x0F
            je _1400
            cmp eax,0x10
            jae _wide
            cmp eax,0x0D
            ja _original
            jmp setAdjustInterfaceReturn

        _1400:
            jmp is1400addr
        _1366:
            jmp is1366addr
        _wide:
            jmp is3840addr
        _original:
            jmp isOriginaladdr
        }
    }

    DWORD is1366_addr = 0x54F2AA;
    DWORD is1400_addr = 0x54F3BC;
    DWORD is3840_addr = 0x54F2F5;
    DWORD isOriginal_addr = 0x54F59B;
    DWORD setAdjustInterface2Return = 0x54F1DA;

    __declspec(naked) void adjust_interface2()
    {
        __asm
        {
            cmp eax,0x0E
            je _1366
            cmp eax,0x0F
            je _1400
            cmp eax,0x10
            jae _wide
            cmp eax,0x0D
            ja _original
            jmp setAdjustInterface2Return

        _1366:
            jmp is1366_addr
        _1400:
            jmp is1400_addr
        _wide:
            jmp is3840_addr
        _original:
            jmp isOriginal_addr
        }
    }

    DWORD is1366_addr2 = 0x4959B6;
    DWORD is1400_addr2 = 0x495EC0;
    DWORD isOriginal_addr2 = 0x49650D;
    DWORD setAdjustInterface3Return = 0x4954EA;

    __declspec(naked) void adjust_interface3()
    {
        __asm
        {
            cmp eax,0x0E
            je _1366
            cmp eax,0x0F
            jae _1400_or_wide
            cmp eax,0x0D
            ja _original
            jmp setAdjustInterface3Return

        _1366:
            push ebx
            push ebp
            push edi
            jmp is1366_addr2
        _1400_or_wide:
            push ebx
            push ebp
            push edi
            jmp is1400_addr2
        _original:
            jmp isOriginal_addr2
        }
    }

    DWORD is1366_addr3 = 0x494D6E;
    DWORD is1400_addr3 = 0x494D80;
    DWORD isOriginal_addr3 = 0x494D90;
    DWORD setAdjustInterface4Return = 0x494D52;

    __declspec(naked) void adjust_interface4()
    {
        __asm
        {
            cmp eax,0x0E
            je _1366
            cmp eax,0x0F
            jae _1400_or_wide
            cmp eax,0x0D
            ja _original
            jmp setAdjustInterface4Return

        _1366:
            jmp is1366_addr3
        _1400_or_wide:
            jmp is1400_addr3
        _original:
            jmp isOriginal_addr3
        }
    }

    DWORD is1366_addr4 = 0x493B95;
    DWORD is1400_addr4 = 0x4942FE;
    DWORD isOriginal_addr4 = 0x494A82;
    DWORD setAdjustInterface5Return = 0x493450;

    __declspec(naked) void adjust_interface5()
    {
        __asm
        {
            cmp eax,0x0E
            je _1366
            cmp eax,0x0F
            jae _1400_or_wide
            cmp eax,0x0D
            ja _original
            jmp setAdjustInterface5Return

        _1366:
            jmp is1366_addr4
        _1400_or_wide:
            jmp is1400_addr4
        _original:
            jmp isOriginal_addr4
        }
    }

    DWORD gammaCache = 0;
    DWORD gammaReturn1 = 0x5215A4;
    DWORD gammaReturn2 = 0x5215BE;
    DWORD gammaReturn3 = 0x51B282;

    __declspec(naked) void cache_gamma_compare()
    {
        __asm
        {
            cmp eax,dword ptr [gammaCache]
            jmp gammaReturn1
        }
    }

    __declspec(naked) void cache_gamma_store()
    {
        __asm
        {
            mov dword ptr [gammaCache],eax
            jmp gammaReturn2
        }
    }

    __declspec(naked) void cache_gamma_load()
    {
        __asm
        {
            mov ecx,dword ptr [gammaCache]
            jmp gammaReturn3
        }
    }

    DWORD distanceCache = 0;
    DWORD distanceReturn1 = 0x52156C;
    DWORD distanceReturn2 = 0x521586;
    DWORD distanceReturn3 = 0x51B28E;

    __declspec(naked) void cache_distance_compare()
    {
        __asm
        {
            cmp eax,dword ptr [distanceCache]
            jmp distanceReturn1
        }
    }

    __declspec(naked) void cache_distance_store()
    {
        __asm
        {
            mov dword ptr [distanceCache],eax
            jmp distanceReturn2
        }
    }

    __declspec(naked) void cache_distance_load()
    {
        __asm
        {
            mov edx,dword ptr [distanceCache]
            jmp distanceReturn3
        }
    }

    float floatCache = 0.0f;
    DWORD floatReturn1 = 0x5223BF;
    DWORD floatReturn2 = 0x5224D3;
    DWORD floatReturn3 = 0x5224EE;
    DWORD floatReturn4 = 0x5224F6;
    DWORD floatReturn5 = 0x52250D;

    __declspec(naked) void cache_float_store1()
    {
        __asm
        {
            fstp dword ptr [floatCache]
            jmp floatReturn1
        }
    }

    __declspec(naked) void cache_float_load1()
    {
        __asm
        {
            fld dword ptr [floatCache]
            jmp floatReturn2
        }
    }

    __declspec(naked) void cache_float_store2()
    {
        __asm
        {
            fstp dword ptr [floatCache]
            jmp floatReturn3
        }
    }

    __declspec(naked) void cache_float_compare()
    {
        __asm
        {
            fcom dword ptr [floatCache]
            jmp floatReturn4
        }
    }

    __declspec(naked) void cache_float_load2()
    {
        __asm
        {
            fld dword ptr [floatCache]
            jmp floatReturn5
        }
    }
}

void hook::resolutions()
{
    // Adds new resolutions.
    // Extends the stock 0..13 resolution table with:
    // 1366x768, 1400x900, 2560x1080, 2560x1440, 3840x1080,
    // 3840x2160, and 3440x1440.
    install_resolution_table();

    util::detour((void*)0x406591, save_res_index, 10);
    util::detour((void*)0x51AEFC, render_option_res_login, 5);
    util::detour((void*)0x51AED6, render_option_res_login2, 5);
    util::detour((void*)0x51E864, render_new_res, 5);
    util::detour((void*)0x51E849, alter_limit_res, 6);
    util::detour((void*)0x51D738, alter_limit_ok, 10);
    util::detour((void*)0x51B30B, set_new_res, 9);

    // UI position branches reused by the original client for larger layouts.
    util::detour((void*)0x54F770, adjust_interface1, 9);
    util::detour((void*)0x54F1D1, adjust_interface2, 9);
    util::detour((void*)0x4954E1, adjust_interface3, 9);
    util::detour((void*)0x494D4D, adjust_interface4, 5);
    util::detour((void*)0x493447, adjust_interface5, 9);

    // Keep the stock gamma/distance option sliders stable after expanding the
    // resolution table. These are isolated caches used by the original options
    // screen code paths.
    util::detour((void*)0x52159E, cache_gamma_compare, 6);
    util::detour((void*)0x5215B9, cache_gamma_store, 5);
    util::detour((void*)0x51B27C, cache_gamma_load, 6);
    util::detour((void*)0x521566, cache_distance_compare, 6);
    util::detour((void*)0x521581, cache_distance_store, 5);
    util::detour((void*)0x51B288, cache_distance_load, 6);
    util::detour((void*)0x5223B9, cache_float_store1, 6);
    util::detour((void*)0x5224CD, cache_float_load1, 6);
    util::detour((void*)0x5224E8, cache_float_store2, 6);
    util::detour((void*)0x5224F0, cache_float_compare, 6);
    util::detour((void*)0x522507, cache_float_load2, 6);
}
