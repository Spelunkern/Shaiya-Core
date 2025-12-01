#include <util/util.h>
#include "include/main.h"

/// <summary>
/// The episode 6.4 PT client expects a packet that contains the weapon step values. 
/// This code makes the function that gets the step from the array return the exact 
/// values.
/// </summary>

unsigned u0x4E753B = 0x4E753B;
void __declspec(naked) naked_0x4E7506()
{
    __asm
    {
        // CPlayerData->weaponStep[i]
        movzx ecx,word ptr[ecx+edx*0x2+0x6268]
        mov eax,ecx
        jmp u0x4E753B
    }
}

void hook::weapon_step()
{
    // Return the exact weapon step value
    util::detour((void*)0x4E7506, naked_0x4E7506, 8);
}