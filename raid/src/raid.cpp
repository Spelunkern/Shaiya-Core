#pragma warning(disable: 4733)
#include <util/util.h>
#include "include/main.h"
#include "include/shaiya/CButton.h"
using namespace shaiya;

uint8_t xz[] = "\x00\xCA\x9A\x3B\x00\xE1\xF5\x05\x80\x96\x98\x00\x40\x42\x0F\x00\xA0\x86\x01\x00\x10\x27\x00\x00\xE8\x03\x00\x00\x64\x00\x00\x00\x0A\x00\x00\x00\x01\x00\x00\x00";
unsigned* xzadr = (unsigned*)&xz;

float Float_Y = -21.0f;
float Float_X_Raid_1 = 380.0f;
float Float_X_Raid_2 = 402.0f;
float Float_X_Raid_3 = 424.0f;
float Float_X_Raid_4 = 446.0f;
float Float_X_Raid_5 = 468.0f;
const char* Raid_Button_1_Targa = "RaidButton1.tga";
const char* Raid_Button_2_Targa = "RaidButton2.tga";
const char* Raid_Button_3_Targa = "RaidButton3.tga";
const char* Raid_Button_4_Targa = "RaidButton4.tga";
const char* Raid_Button_5_Targa = "RaidButton5.tga";
unsigned Button_Call = 0x429FD0;
unsigned Button_Call_2 = 0x54FCE0;
unsigned Button_Call_3 = 0x54F100;
unsigned Button_Call_4 = 0x631BE0;
unsigned Button_Call_5 = 0x551860;

CButton raid_btn_1{};
CButton raid_btn_2{};
CButton raid_btn_3{};
CButton raid_btn_4{};
CButton raid_btn_5{};
int raid_number = 0;
int raid_pointer_lead = 5;

__declspec(naked) void Raid_Button_1()
{
    __asm
    {
        push - 0x01
        push 0x007356FE
        mov eax, fs: [0x00000000]
        push eax
        push ecx
        push ebx
        push ebp
        push esi
        push edi
        mov eax, dword ptr ds : [0x007B4DD0]
        xor eax, esp
        push eax
        lea eax, [esp + 0x18]
        mov fs : [0x00000000] , eax
        mov esi, ecx
        mov[esp + 0x14], esi
        xor ebx, ebx
        mov[esi + 0x04], ebx
        mov[esi + 0x08], ebx
        mov[esi + 0x0C], ebx
        mov[esi + 0x10], ebx
        mov[esi + 0x14], ebx
        mov[esi + 0x18], ebx
        mov[esi + 0x1C], ebx
        mov[esi + 0x20], 0x00000001
        mov[esi + 0x24], ebx
        mov[esi + 0x28], ebx
        lea edi, [esi + 0x2C]
        mov[esp + 0x20], ebx
        mov dword ptr ds : [edi] , 0x00748120
        lea ecx, [esi + 0x30]
        mov[esp + 0x20], 01
        mov dword ptr ds : [esi] , 0x00751F78
        mov dword ptr ds : [edi] , 0x00751F60
        call Button_Call
        fldz
        fst dword ptr ds : [esi + 0x00002130]
        mov[esi + 0x00002110], ebx
        fst dword ptr ds : [esi + 0x00002134]
        mov[esi + 0x0000210C], ebx
        fst dword ptr ds : [esi + 0x00002140]
        fst dword ptr ds : [esi + 0x00002144]
        mov[esi + 0x00002120], ebx //
        mov[esi + 0x0000211C], ebx //
        fst dword ptr ds : [esi + 0x00002150] //
        fst dword ptr ds : [esi + 0x00002154] //
        mov[esi + 0x00002130], ebx //
        mov[esi + 0x0000212C], ebx //
        fst dword ptr ds : [esi + 0x00002160] //
        fstp dword ptr ds : [esi + 0x00002164] //
        mov[esi + 0x0000214C], ebx //
        mov[esi + 0x00002148], ebx //
        lea ebp, [raid_btn_1]
        mov ecx, ebp
        mov byte ptr ds : [esp + 0x20] , 0x06
        call Button_Call_2
        fldz
        fst dword ptr ds : [esi + 0x00000724]
        mov[esi + 0x00000720], ebx
        fst dword ptr ds : [esi + 0x00000728]
        mov[esi + 0x0000071C], ebx
        fst dword ptr ds : [esi + 0x00000734]
        fst dword ptr ds : [esi + 0x00000738]
        mov[esi + 0x00000730], ebx
        mov[esi + 0x0000072C], ebx
        fst dword ptr ds : [esi + 0x00000744]
        fstp dword ptr ds : [esi + 0x00000748]
        mov[esi + 0x00000740], ebx
        mov[esi + 0x0000073C], ebx
        mov eax, [edi]
        mov edx, [eax + 0x10]
        mov ecx, edi
        mov byte ptr ds : [esp + 0x20] , 0x0A
        call edx
        mov[esi + 0x20], ebx
        mov eax, dword ptr ds : [0x007AB0D8]
        add eax, 0xFFFFFEEC
        mov[esi + 0x04], eax
        mov al, -0x01
        mov[esi + 0x08], ebx
        mov[esi + 0x0C], 0x00000100
        mov[esi + 0x10], 0x000001E6
        mov byte ptr ds : [esi + 0x00002115] , 0x01
        mov[esi + 0x00002115], al
        mov[esi + 0x00002110], ebx
        mov byte ptr ds : [esi + 0x00002116] , 0x01
        mov edx, [esi]
        mov[esi + 0x0000211], al
        mov eax, [edx + 0x0C]
        mov ecx, esi
        mov[esi + 0x00000744], ebx
        mov[esi + 0x00002158], bl
        call eax
        mov ecx, esi
        call Button_Call_3
        fldz
        push ebx
        sub esp, 0x50
        fst dword ptr ds : [esp + 0x4C]
        fst dword ptr ds : [esp + 0x48]
        fst dword ptr ds : [esp + 0x44]
        fst dword ptr ds : [esp + 0x40]
        fst dword ptr ds : [esp + 0x3C]
        fst dword ptr ds : [esp + 0x38]
        fst dword ptr ds : [esp + 0x34]
        fst dword ptr ds : [esp + 0x30]
        fld1
        fst dword ptr ds : [esp + 0x2C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x28]
        fld dword ptr ds : [0x00748164]
        fstp dword ptr ds : [esp + 0x24]
        fld dword ptr ds : [0x00748160]
        fst dword ptr ds : [esp + 0x20]
        fxch st(2)
        fst dword ptr ds : [esp + 0x1C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x18]
        fxch st(2)
        fstp dword ptr ds : [esp + 0x14]
        fld dword ptr ds : [0x0074815C]
        fst dword ptr ds : [esp + 0x10]
        fxch st(1)
        fstp dword ptr ds : [esp + 0x0C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x08]
        fxch st(1)
        fstp dword ptr ds : [esp + 0x04]
        fstp dword ptr ds : [esp]
        push 0x01
        fld dword ptr ds : [Float_Y]
        push 0x20
        push 0x00000080
        push Raid_Button_1_Targa
        push ebx
        push 0x17
        push 0x18
        push 0x17
        push 0x18
        call Button_Call_4
        fld dword ptr ds : [Float_X_Raid_1]
        push eax
        call Button_Call_4
        mov ecx, [esi + 0x08]
        mov edx, [esi + 0x04]
        push eax
        push ecx
        push edx
        mov ecx, ebp
        call Button_Call_5
        mov[esi + 0x00000750], 00000001
        mov[esi + 0x00000758], ebx
        mov eax, esi
        mov ecx, [esp + 0x18]
        mov fs : [0x00000000] , ecx
        pop ecx
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 0x10
        ret
    }
}

__declspec(naked) void Raid_Button_2()
{
    __asm
    {
        push - 0x01
        push 0x007356FE
        mov eax, fs: [0x00000000]
        push eax
        push ecx
        push ebx
        push ebp
        push esi
        push edi
        mov eax, dword ptr ds : [0x007B4DD0]
        xor eax, esp
        push eax
        lea eax, [esp + 0x18]
        mov fs : [0x00000000] , eax
        mov esi, ecx
        mov[esp + 0x14], esi
        xor ebx, ebx
        mov[esi + 0x04], ebx
        mov[esi + 0x08], ebx
        mov[esi + 0x0C], ebx
        mov[esi + 0x10], ebx
        mov[esi + 0x14], ebx
        mov[esi + 0x18], ebx
        mov[esi + 0x1C], ebx
        mov[esi + 0x20], 0x00000001
        mov[esi + 0x24], ebx
        mov[esi + 0x28], ebx
        lea edi, [esi + 0x2C]
        mov[esp + 0x20], ebx
        mov dword ptr ds : [edi] , 0x00748120
        lea ecx, [esi + 0x30]
        mov[esp + 0x20], 01
        mov dword ptr ds : [esi] , 0x00751F78
        mov dword ptr ds : [edi] , 0x00751F60
        call Button_Call
        fldz
        fst dword ptr ds : [esi + 0x00002130]
        mov ds : [esi + 0x00002110] , ebx
        fst dword ptr ds : [esi + 0x00002134]
        mov[esi + 0x0000210C], ebx
        fst dword ptr ds : [esi + 0x00002140]
        fst dword ptr ds : [esi + 0x00002144]
        mov[esi + 0x00002120], ebx //
        mov[esi + 0x0000211C], ebx //
        fst dword ptr ds : [esi + 0x00002150] //
        fst dword ptr ds : [esi + 0x00002154] //
        mov[esi + 0x00002130], ebx //
        mov[esi + 0x0000212C], ebx //
        fst dword ptr ds : [esi + 0x00002160] //
        fstp dword ptr ds : [esi + 0x00002164] //
        mov[esi + 0x0000214C], ebx //
        mov[esi + 0x00002148], ebx //
        lea ebp, [raid_btn_2]
        mov ecx, ebp
        mov byte ptr ds : [esp + 0x20] , 0x06
        call Button_Call_2
        fldz
        fst dword ptr ds : [esi + 0x00000724]
        mov[esi + 0x00000720], ebx
        fst dword ptr ds : [esi + 0x00000728]
        mov[esi + 0x0000071C], ebx
        fst dword ptr ds : [esi + 0x00000734]
        fst dword ptr ds : [esi + 0x00000738]
        mov[esi + 0x00000730], ebx
        mov[esi + 0x0000072C], ebx
        fst dword ptr ds : [esi + 0x00000744]
        fstp dword ptr ds : [esi + 0x00000748]
        mov[esi + 0x00000740], ebx
        mov[esi + 0x0000073C], ebx
        mov eax, [edi]
        mov edx, [eax + 0x10]
        mov ecx, edi
        mov byte ptr ds : [esp + 0x20] , 0x0A
        call edx
        mov[esi + 0x20], ebx
        mov eax, dword ptr ds : [0x007AB0D8]
        add eax, 0xFFFFFEEC
        mov[esi + 0x04], eax
        mov al, -0x01
        mov[esi + 0x08], ebx
        mov[esi + 0x0C], 0x00000100
        mov[esi + 0x10], 0x000001E6
        mov byte ptr ds : [esi + 0x00002115] , 0x01
        mov[esi + 0x00002115], al
        mov[esi + 0x00002110], ebx
        mov byte ptr ds : [esi + 0x00002116] , 0x01
        mov edx, [esi]
        mov[esi + 0x0000211], al
        mov eax, [edx + 0x0C]
        mov ecx, esi
        mov[esi + 0x00000744], ebx
        mov[esi + 0x00002158], bl
        call eax
        mov ecx, esi
        call Button_Call_3
        fldz
        push ebx
        sub esp, 0x50
        fst dword ptr ds : [esp + 0x4C]
        fst dword ptr ds : [esp + 0x48]
        fst dword ptr ds : [esp + 0x44]
        fst dword ptr ds : [esp + 0x40]
        fst dword ptr ds : [esp + 0x3C]
        fst dword ptr ds : [esp + 0x38]
        fst dword ptr ds : [esp + 0x34]
        fst dword ptr ds : [esp + 0x30]
        fld1
        fst dword ptr ds : [esp + 0x2C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x28]
        fld dword ptr ds : [0x00748164]
        fstp dword ptr ds : [esp + 0x24]
        fld dword ptr ds : [0x00748160]
        fst dword ptr ds : [esp + 0x20]
        fxch st(2)
        fst dword ptr ds : [esp + 0x1C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x18]
        fxch st(2)
        fstp dword ptr ds : [esp + 0x14]
        fld dword ptr ds : [0x0074815C]
        fst dword ptr ds : [esp + 0x10]
        fxch st(1)
        fstp dword ptr ds : [esp + 0x0C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x08]
        fxch st(1)
        fstp dword ptr ds : [esp + 0x04]
        fstp dword ptr ds : [esp]
        push 0x01
        fld dword ptr ds : [Float_Y]
        push 0x20
        push 0x00000080
        push Raid_Button_2_Targa
        push ebx
        push 0x17
        push 0x18
        push 0x17
        push 0x18
        call Button_Call_4
        fld dword ptr ds : [Float_X_Raid_2]
        push eax
        call Button_Call_4
        mov ecx, [esi + 0x08]
        mov edx, [esi + 0x04]
        push eax
        push ecx
        push edx
        mov ecx, ebp
        call Button_Call_5
        mov[esi + 0x00000750], 00000001
        mov[esi + 0x00000758], ebx
        mov eax, esi
        mov ecx, [esp + 0x18]
        mov fs : [0x00000000] , ecx
        pop ecx
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 0x10
        ret
    }
}

__declspec(naked) void Raid_Button_3()
{
    __asm
    {
        push - 0x01
        push 0x007356FE
        mov eax, fs: [0x00000000]
        push eax
        push ecx
        push ebx
        push ebp
        push esi
        push edi
        mov eax, dword ptr ds : [0x007B4DD0]
        xor eax, esp
        push eax
        lea eax, [esp + 0x18]
        mov fs : [0x00000000] , eax
        mov esi, ecx
        mov[esp + 0x14], esi
        xor ebx, ebx
        mov[esi + 0x04], ebx
        mov[esi + 0x08], ebx
        mov[esi + 0x0C], ebx
        mov[esi + 0x10], ebx
        mov[esi + 0x14], ebx
        mov[esi + 0x18], ebx
        mov[esi + 0x1C], ebx
        mov[esi + 0x20], 0x00000001
        mov[esi + 0x24], ebx
        mov[esi + 0x28], ebx
        lea edi, [esi + 0x2C]
        mov[esp + 0x20], ebx
        mov dword ptr ds : [edi] , 0x00748120
        lea ecx, [esi + 0x30]
        mov[esp + 0x20], 01
        mov dword ptr ds : [esi] , 0x00751F78
        mov dword ptr ds : [edi] , 0x00751F60
        call Button_Call
        fldz
        fst dword ptr ds : [esi + 0x00002130]
        mov ds : [esi + 0x00002110] , ebx
        fst dword ptr ds : [esi + 0x00002134]
        mov[esi + 0x0000210C], ebx
        fst dword ptr ds : [esi + 0x00002140]
        fst dword ptr ds : [esi + 0x00002144]
        mov[esi + 0x00002120], ebx //
        mov[esi + 0x0000211C], ebx //
        fst dword ptr ds : [esi + 0x00002150] //
        fst dword ptr ds : [esi + 0x00002154] //
        mov[esi + 0x00002130], ebx //
        mov[esi + 0x0000212C], ebx //
        fst dword ptr ds : [esi + 0x00002160] //
        fstp dword ptr ds : [esi + 0x00002164] //
        mov[esi + 0x0000214C], ebx //
        mov[esi + 0x00002148], ebx //
        lea ebp, [raid_btn_3]
        mov ecx, ebp
        mov byte ptr ds : [esp + 0x20] , 0x06
        call Button_Call_2
        fldz
        fst dword ptr ds : [esi + 0x00000724]
        mov[esi + 0x00000720], ebx
        fst dword ptr ds : [esi + 0x00000728]
        mov[esi + 0x0000071C], ebx
        fst dword ptr ds : [esi + 0x00000734]
        fst dword ptr ds : [esi + 0x00000738]
        mov[esi + 0x00000730], ebx
        mov[esi + 0x0000072C], ebx
        fst dword ptr ds : [esi + 0x00000744]
        fstp dword ptr ds : [esi + 0x00000748]
        mov[esi + 0x00000740], ebx
        mov[esi + 0x0000073C], ebx
        mov eax, [edi]
        mov edx, [eax + 0x10]
        mov ecx, edi
        mov byte ptr ds : [esp + 0x20] , 0x0A
        call edx
        mov[esi + 0x20], ebx
        mov eax, dword ptr ds : [0x007AB0D8]
        add eax, 0xFFFFFEEC
        mov[esi + 0x04], eax
        mov al, -0x01
        mov[esi + 0x08], ebx
        mov[esi + 0x0C], 0x00000100
        mov[esi + 0x10], 0x000001E6
        mov byte ptr ds : [esi + 0x00002115] , 0x01
        mov[esi + 0x00002115], al
        mov[esi + 0x00002110], ebx
        mov byte ptr ds : [esi + 0x00002116] , 0x01
        mov edx, [esi]
        mov[esi + 0x0000211], al
        mov eax, [edx + 0x0C]
        mov ecx, esi
        mov[esi + 0x00000744], ebx
        mov[esi + 0x00002158], bl
        call eax
        mov ecx, esi
        call Button_Call_3
        fldz
        push ebx
        sub esp, 0x50
        fst dword ptr ds : [esp + 0x4C]
        fst dword ptr ds : [esp + 0x48]
        fst dword ptr ds : [esp + 0x44]
        fst dword ptr ds : [esp + 0x40]
        fst dword ptr ds : [esp + 0x3C]
        fst dword ptr ds : [esp + 0x38]
        fst dword ptr ds : [esp + 0x34]
        fst dword ptr ds : [esp + 0x30]
        fld1
        fst dword ptr ds : [esp + 0x2C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x28]
        fld dword ptr ds : [0x00748164]
        fstp dword ptr ds : [esp + 0x24]
        fld dword ptr ds : [0x00748160]
        fst dword ptr ds : [esp + 0x20]
        fxch st(2)
        fst dword ptr ds : [esp + 0x1C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x18]
        fxch st(2)
        fstp dword ptr ds : [esp + 0x14]
        fld dword ptr ds : [0x0074815C]
        fst dword ptr ds : [esp + 0x10]
        fxch st(1)
        fstp dword ptr ds : [esp + 0x0C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x08]
        fxch st(1)
        fstp dword ptr ds : [esp + 0x04]
        fstp dword ptr ds : [esp]
        push 0x01
        fld dword ptr ds : [Float_Y]
        push 0x20
        push 0x00000080
        push Raid_Button_3_Targa
        push ebx
        push 0x17
        push 0x18
        push 0x17
        push 0x18
        call Button_Call_4
        fld dword ptr ds : [Float_X_Raid_3]
        push eax
        call Button_Call_4
        mov ecx, [esi + 0x08]
        mov edx, [esi + 0x04]
        push eax
        push ecx
        push edx
        mov ecx, ebp
        call Button_Call_5
        mov[esi + 0x00000750], 00000001
        mov[esi + 0x00000758], ebx
        mov eax, esi
        mov ecx, [esp + 0x18]
        mov fs : [0x00000000] , ecx
        pop ecx
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 0x10
        ret
    }
}

__declspec(naked) void Raid_Button_4()
{
    __asm
    {
        push - 0x01
        push 0x007356FE
        mov eax, fs: [0x00000000]
        push eax
        push ecx
        push ebx
        push ebp
        push esi
        push edi
        mov eax, dword ptr ds : [0x007B4DD0]
        xor eax, esp
        push eax
        lea eax, [esp + 0x18]
        mov fs : [0x00000000] , eax
        mov esi, ecx
        mov[esp + 0x14], esi
        xor ebx, ebx
        mov[esi + 0x04], ebx
        mov[esi + 0x08], ebx
        mov[esi + 0x0C], ebx
        mov[esi + 0x10], ebx
        mov[esi + 0x14], ebx
        mov[esi + 0x18], ebx
        mov[esi + 0x1C], ebx
        mov[esi + 0x20], 0x00000001
        mov[esi + 0x24], ebx
        mov[esi + 0x28], ebx
        lea edi, [esi + 0x2C]
        mov[esp + 0x20], ebx
        mov dword ptr ds : [edi] , 0x00748120
        lea ecx, [esi + 0x30]
        mov[esp + 0x20], 01
        mov dword ptr ds : [esi] , 0x00751F78
        mov dword ptr ds : [edi] , 0x00751F60
        call Button_Call
        fldz
        fst dword ptr ds : [esi + 0x00002130]
        mov ds : [esi + 0x00002110] , ebx
        fst dword ptr ds : [esi + 0x00002134]
        mov[esi + 0x0000210C], ebx
        fst dword ptr ds : [esi + 0x00002140]
        fst dword ptr ds : [esi + 0x00002144]
        mov[esi + 0x00002120], ebx //
        mov[esi + 0x0000211C], ebx //
        fst dword ptr ds : [esi + 0x00002150] //
        fst dword ptr ds : [esi + 0x00002154] //
        mov[esi + 0x00002130], ebx //
        mov[esi + 0x0000212C], ebx //
        fst dword ptr ds : [esi + 0x00002160] //
        fstp dword ptr ds : [esi + 0x00002164] //
        mov[esi + 0x0000214C], ebx //
        mov[esi + 0x00002148], ebx //
        lea ebp, [raid_btn_4]
        mov ecx, ebp
        mov byte ptr ds : [esp + 0x20] , 0x06
        call Button_Call_2
        fldz
        fst dword ptr ds : [esi + 0x00000724]
        mov[esi + 0x00000720], ebx
        fst dword ptr ds : [esi + 0x00000728]
        mov[esi + 0x0000071C], ebx
        fst dword ptr ds : [esi + 0x00000734]
        fst dword ptr ds : [esi + 0x00000738]
        mov[esi + 0x00000730], ebx
        mov[esi + 0x0000072C], ebx
        fst dword ptr ds : [esi + 0x00000744]
        fstp dword ptr ds : [esi + 0x00000748]
        mov[esi + 0x00000740], ebx
        mov[esi + 0x0000073C], ebx
        mov eax, [edi]
        mov edx, [eax + 0x10]
        mov ecx, edi
        mov byte ptr ds : [esp + 0x20] , 0x0A
        call edx
        mov[esi + 0x20], ebx
        mov eax, dword ptr ds : [0x007AB0D8]
        add eax, 0xFFFFFEEC
        mov[esi + 0x04], eax
        mov al, -0x01
        mov[esi + 0x08], ebx
        mov[esi + 0x0C], 0x00000100
        mov[esi + 0x10], 0x000001E6
        mov byte ptr ds : [esi + 0x00002115] , 0x01
        mov[esi + 0x00002115], al
        mov[esi + 0x00002110], ebx
        mov byte ptr ds : [esi + 0x00002116] , 0x01
        mov edx, [esi]
        mov[esi + 0x0000211], al
        mov eax, [edx + 0x0C]
        mov ecx, esi
        mov[esi + 0x00000744], ebx
        mov[esi + 0x00002158], bl
        call eax
        mov ecx, esi
        call Button_Call_3
        fldz
        push ebx
        sub esp, 0x50
        fst dword ptr ds : [esp + 0x4C]
        fst dword ptr ds : [esp + 0x48]
        fst dword ptr ds : [esp + 0x44]
        fst dword ptr ds : [esp + 0x40]
        fst dword ptr ds : [esp + 0x3C]
        fst dword ptr ds : [esp + 0x38]
        fst dword ptr ds : [esp + 0x34]
        fst dword ptr ds : [esp + 0x30]
        fld1
        fst dword ptr ds : [esp + 0x2C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x28]
        fld dword ptr ds : [0x00748164]
        fstp dword ptr ds : [esp + 0x24]
        fld dword ptr ds : [0x00748160]
        fst dword ptr ds : [esp + 0x20]
        fxch st(2)
        fst dword ptr ds : [esp + 0x1C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x18]
        fxch st(2)
        fstp dword ptr ds : [esp + 0x14]
        fld dword ptr ds : [0x0074815C]
        fst dword ptr ds : [esp + 0x10]
        fxch st(1)
        fstp dword ptr ds : [esp + 0x0C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x08]
        fxch st(1)
        fstp dword ptr ds : [esp + 0x04]
        fstp dword ptr ds : [esp]
        push 0x01
        fld dword ptr ds : [Float_Y]
        push 0x20
        push 0x00000080
        push Raid_Button_4_Targa
        push ebx
        push 0x17
        push 0x18
        push 0x17
        push 0x18
        call Button_Call_4
        fld dword ptr ds : [Float_X_Raid_4]
        push eax
        call Button_Call_4
        mov ecx, [esi + 0x08]
        mov edx, [esi + 0x04]
        push eax
        push ecx
        push edx
        mov ecx, ebp
        call Button_Call_5
        mov[esi + 0x00000750], 00000001
        mov[esi + 0x00000758], ebx
        mov eax, esi
        mov ecx, [esp + 0x18]
        mov fs : [0x00000000] , ecx
        pop ecx
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 0x10
        ret
    }
}

__declspec(naked) void Raid_Button_5()
{
    __asm
    {
        push - 0x01
        push 0x007356FE
        mov eax, fs: [0x00000000]
        push eax
        push ecx
        push ebx
        push ebp
        push esi
        push edi
        mov eax, dword ptr ds : [0x007B4DD0]
        xor eax, esp
        push eax
        lea eax, [esp + 0x18]
        mov fs : [0x00000000] , eax
        mov esi, ecx
        mov[esp + 0x14], esi
        xor ebx, ebx
        mov[esi + 0x04], ebx
        mov[esi + 0x08], ebx
        mov[esi + 0x0C], ebx
        mov[esi + 0x10], ebx
        mov[esi + 0x14], ebx
        mov[esi + 0x18], ebx
        mov[esi + 0x1C], ebx
        mov[esi + 0x20], 0x00000001
        mov[esi + 0x24], ebx
        mov[esi + 0x28], ebx
        lea edi, [esi + 0x2C]
        mov[esp + 0x20], ebx
        mov dword ptr ds : [edi] , 0x00748120
        lea ecx, [esi + 0x30]
        mov[esp + 0x20], 01
        mov dword ptr ds : [esi] , 0x00751F78
        mov dword ptr ds : [edi] , 0x00751F60
        call Button_Call
        fldz
        fst dword ptr ds : [esi + 0x00002130]
        mov[esi + 0x00002110], ebx
        fst dword ptr ds : [esi + 0x00002134]
        mov[esi + 0x0000210C], ebx
        fst dword ptr ds : [esi + 0x00002140]
        fst dword ptr ds : [esi + 0x00002144]
        mov[esi + 0x00002120], ebx //
        mov[esi + 0x0000211C], ebx //
        fst dword ptr ds : [esi + 0x00002150] //
        fst dword ptr ds : [esi + 0x00002154] //
        mov[esi + 0x00002130], ebx //
        mov[esi + 0x0000212C], ebx //
        fst dword ptr ds : [esi + 0x00002160] //
        fstp dword ptr ds : [esi + 0x00002164] //
        mov[esi + 0x0000214C], ebx //
        mov[esi + 0x00002148], ebx //
        lea ebp, [raid_btn_5]
        mov ecx, ebp
        mov byte ptr ds : [esp + 0x20] , 0x06
        call Button_Call_2
        fldz
        fst dword ptr ds : [esi + 0x00000724]
        mov[esi + 0x00000720], ebx
        fst dword ptr ds : [esi + 0x00000728]
        mov[esi + 0x0000071C], ebx
        fst dword ptr ds : [esi + 0x00000734]
        fst dword ptr ds : [esi + 0x00000738]
        mov[esi + 0x00000730], ebx
        mov[esi + 0x0000072C], ebx
        fst dword ptr ds : [esi + 0x00000744]
        fstp dword ptr ds : [esi + 0x00000748]
        mov[esi + 0x00000740], ebx
        mov[esi + 0x0000073C], ebx
        mov eax, [edi]
        mov edx, [eax + 0x10]
        mov ecx, edi
        mov byte ptr ds : [esp + 0x20] , 0x0A
        call edx
        mov[esi + 0x20], ebx
        mov eax, dword ptr ds : [0x007AB0D8]
        add eax, 0xFFFFFEEC
        mov[esi + 0x04], eax
        mov al, -0x01
        mov[esi + 0x08], ebx
        mov[esi + 0x0C], 0x00000100
        mov[esi + 0x10], 0x000001E6
        mov byte ptr ds : [esi + 0x00002115] , 0x01
        mov[esi + 0x00002115], al
        mov[esi + 0x00002110], ebx
        mov byte ptr ds : [esi + 0x00002116] , 0x01
        mov edx, [esi]
        mov[esi + 0x0000211], al
        mov eax, [edx + 0x0C]
        mov ecx, esi
        mov[esi + 0x00000744], ebx
        mov[esi + 0x00002158], bl
        call eax
        mov ecx, esi
        call Button_Call_3
        fldz
        push ebx
        sub esp, 0x50
        fst dword ptr ds : [esp + 0x4C]
        fst dword ptr ds : [esp + 0x48]
        fst dword ptr ds : [esp + 0x44]
        fst dword ptr ds : [esp + 0x40]
        fst dword ptr ds : [esp + 0x3C]
        fst dword ptr ds : [esp + 0x38]
        fst dword ptr ds : [esp + 0x34]
        fst dword ptr ds : [esp + 0x30]
        fld1
        fst dword ptr ds : [esp + 0x2C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x28]
        fld dword ptr ds : [0x00748164]
        fstp dword ptr ds : [esp + 0x24]
        fld dword ptr ds : [0x00748160]
        fst dword ptr ds : [esp + 0x20]
        fxch st(2)
        fst dword ptr ds : [esp + 0x1C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x18]
        fxch st(2)
        fstp dword ptr ds : [esp + 0x14]
        fld dword ptr ds : [0x0074815C]
        fst dword ptr ds : [esp + 0x10]
        fxch st(1)
        fstp dword ptr ds : [esp + 0x0C]
        fxch st(1)
        fst dword ptr ds : [esp + 0x08]
        fxch st(1)
        fstp dword ptr ds : [esp + 0x04]
        fstp dword ptr ds : [esp]
        push 0x01
        fld dword ptr ds : [Float_Y]
        push 0x20
        push 0x00000080
        push Raid_Button_5_Targa
        push ebx
        push 0x17
        push 0x18
        push 0x17
        push 0x18
        call Button_Call_4
        fld dword ptr ds : [Float_X_Raid_5]
        push eax
        call Button_Call_4
        mov ecx, [esi + 0x08]
        mov edx, [esi + 0x04]
        push eax
        push ecx
        push edx
        mov ecx, ebp
        call Button_Call_5
        mov[esi + 0x00000750], 00000001
        mov[esi + 0x00000758], ebx
        mov eax, esi
        mov ecx, [esp + 0x18]
        mov fs : [0x00000000] , ecx
        pop ecx
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 0x10
        ret
    }
}

unsigned Send_Buttons_To_Memory_Return = 0x42B11B;
unsigned Button_Original = 0x5187B0;
unsigned Pointer_Test = 0;
__declspec(naked) void Send_Buttons_To_Memory()
{
    __asm
    {
        mov dword ptr ds : [Pointer_Test] , eax
        //mov ecx,eax
        call Raid_Button_3
        mov ecx, Pointer_Test
        call Button_Original
        jmp Send_Buttons_To_Memory_Return
    }
}

unsigned Send_Buttons_Call = 0x006307F3;
unsigned u0x42B6D2 = 0x42B6D2;
void __declspec(naked) New_Send_BUttons_To_Memory()
{
    __asm
    {
        pushad

        push 0x1AC3C
        mov[esp + 0x48], bl
        call Send_Buttons_Call
        add esp, 0x04

        mov[esp + 0x38], eax
        mov byte ptr[esp + 0x44], 0x22

        test eax, eax
        je label

        mov ecx, eax
        Call Raid_Button_1
        mov ecx, eax
        Call Raid_Button_2
        mov ecx, eax
        call Raid_Button_3
        mov ecx, eax
        Call Raid_Button_4
        mov ecx, eax
        Call Raid_Button_5

        label :

        popad

            // original
            xor eax, eax
            mov ecx, 0x34
            jmp u0x42B6D2
    }
}

unsigned Render_Call = 0x550120;
unsigned Render_Call_2 = 0x550A20;
unsigned Render_Raid_Button_Return = 0x53F8D4;
__declspec(naked) void Render_Raid_Buttons()
{
    __asm
    {
        pushad
        mov eax, [edi + 0x08]
        mov ecx, [edi + 0x04]
        push eax
        lea edi, [raid_btn_1]
        push ecx
        mov ecx, edi
        call Render_Call
        mov ecx, edi
        call Render_Call_2
        popad

        // 022FE180
        // 022FE220
        // 022FE2E0
        pushad
        mov eax, [edi + 0x08]
        mov ecx, [edi + 0x04]
        push eax
        lea edi, [raid_btn_2]
        push ecx
        mov ecx, edi
        call Render_Call
        mov ecx, edi
        call Render_Call_2
        popad

        pushad
        mov eax, [edi + 0x08]
        mov ecx, [edi + 0x04]
        push eax
        lea edi, [raid_btn_3]
        push ecx
        mov ecx, edi
        call Render_Call
        mov ecx, edi
        call Render_Call_2
        popad

        pushad
        mov eax, [edi + 0x08]
        mov ecx, [edi + 0x04]
        push eax
        lea edi, [raid_btn_4]
        push ecx
        mov ecx, edi
        call Render_Call
        mov ecx, edi
        call Render_Call_2
        popad

        pushad
        mov eax, [edi + 0x08]
        mov ecx, [edi + 0x04]
        push eax
        lea edi, [raid_btn_5]
        push ecx
        mov ecx, edi
        call Render_Call
        mov ecx, edi
        call Render_Call_2
        popad

        mov ebx, [esp + 0x28]
        test ebx, ebx
        jmp Render_Raid_Button_Return
    }
}

unsigned Get_Click = 0x550A10;
unsigned Raid_Click_Return = 0x53F46A;
__declspec(naked) void Click_Raid_Button()
{
    __asm
    {
        pushad
        lea ecx, [raid_btn_1]
        call Get_Click
        test eax, eax
        jne Raid_1
        popad

        pushad
        lea ecx, [raid_btn_2]
        call Get_Click
        test eax, eax
        jne Raid_2
        popad

        pushad
        lea ecx, [raid_btn_3]
        call Get_Click
        test eax, eax
        jne Raid_3
        popad

        pushad
        lea ecx, [raid_btn_4]
        call Get_Click
        test eax, eax
        jne Raid_4
        popad

        pushad
        lea ecx, [raid_btn_5]
        call Get_Click
        test eax, eax
        jne Raid_5
        popad

        originalcode :
        cmp dword ptr ds : [0x022AA800] , ebp
            jmp Raid_Click_Return

            Raid_1 :
        popad
            mov byte ptr ds : [raid_number] , 0x00
            jmp originalcode

            Raid_2 :
        popad
            mov byte ptr ds : [raid_number] , 0x01
            jmp originalcode

            Raid_3 :
        popad
            mov byte ptr ds : [raid_number] , 0x02
            jmp originalcode

            Raid_4 :
        popad
            mov byte ptr ds : [raid_number] , 0x03
            jmp originalcode

            Raid_5 :
        popad
            mov byte ptr ds : [raid_number] , 0x04
            jmp originalcode
    }
}

unsigned original_code_addr = 0x551072;
unsigned Send_Holding_Return = 0x550F5D;
__declspec(naked) void Send_Holding()
{
    __asm
    {
        cmp byte ptr ds : [raid_number] , 00
        je Send_Holding_Raid_1
        cmp byte ptr ds : [raid_number] , 01
        je Send_Holding_Raid_2
        cmp byte ptr ds : [raid_number] , 02
        je Send_Holding_Raid_3
        cmp byte ptr ds : [raid_number] , 03
        je Send_Holding_Raid_4
        cmp byte ptr ds : [raid_number] , 04
        je Send_Holding_Raid_5

    originalcode:
        cmp byte ptr ds : [esi + 0x06] , 00
            je original_code_conditional
            jmp Send_Holding_Return

            Send_Holding_Raid_1 :
        mov byte ptr ds : [raid_btn_1 + 0x06] , 01
            jmp originalcode

            Send_Holding_Raid_2 :
        mov byte ptr ds : [raid_btn_2 + 0x06] , 01
            jmp originalcode

            Send_Holding_Raid_3 :
        mov byte ptr ds : [raid_btn_3 + 0x06] , 01
            jmp originalcode

            Send_Holding_Raid_4 :
        mov byte ptr ds : [raid_btn_4 + 0x06] , 01
            jmp originalcode

            Send_Holding_Raid_5 :
        mov byte ptr ds : [raid_btn_5 + 0x06] , 01
            jmp originalcode

        original_code_conditional:
        jmp original_code_addr
    }
}

void __declspec(naked) Members()
{
    __asm
    {
        push eax
        push ebx
        push edx
        push edi
        xor edi, edi
        mov ebx, xzadr
        m1 :
        xor edx, edx
            div dword ptr[ebx]
            cmp eax, 0x00
            jne m2
            cmp edi, 0x01
            jne m3
            m2 :
        add eax, 0x30
            mov byte ptr[ecx], al
            mov edi, 0x00000001
            inc ecx
            m3 :
        mov eax, edx
            add ebx, 0x04
            cmp dword ptr[ebx], 0x00
            jne m1
            pop edi
            pop edx
            pop ebx
            pop eax
            ret
    }
}

unsigned RaidInj1jmp = 0x53E879;
void __declspec(naked) RaidInj1()
{
    __asm
    {
        push ecx
        mov ecx, [raid_number]
        imul ecx, ecx, 0x1E
        add esi, ecx
        pop ecx
        push esi
        mov ecx, 0x022AA748
        mov[ecx], esi
        mov ecx, 0x022AA71C
        jmp RaidInj1jmp
    }
}

unsigned RaidInj2jmp = 0x53F52D;
void __declspec(naked) RaidInj2()
{
    __asm
    {
        mov ecx, [raid_number]
        imul ecx, ecx, 0x1E
        add esi, ecx
        push esi
        mov ecx, 0x022AA71C
        jmp RaidInj2jmp
    }
}

unsigned RaidInj4jmp = 0x54073B;
unsigned RaidInj4call = 0x451B80;
void __declspec(naked) RaidInj4()
{
    __asm
    {
        mov eax, [raid_number]
        imul eax, eax, 0x1E
        add ebx, eax
        mov dword ptr[esp + 0x30], ebx
        call RaidInj4call
        jmp RaidInj4jmp
    }
}

unsigned RaidInj5jmp = 0x53F65F;
unsigned RaidInj5call = 0x451B80;
void __declspec(naked) RaidInj5()
{
    __asm
    {
        mov eax, [raid_number]
        imul eax, eax, 0x1E
        add ebx, eax
        mov dword ptr[esp + 0x30], ebx
        call RaidInj5call
        jmp RaidInj5jmp
    }
}

unsigned RaidInj6jmp = 0x4A44E0;
unsigned RaidInj6cazz = 0x227E334;
unsigned RaidInj6altjmp = 0x4A4B59;
unsigned RaidInj6jgl = 0x4A491F;
unsigned RaidInj6exjmp = 0x4A4B59;
unsigned RaidInj6jgc = 0x4A491F;
void __declspec(naked) RaidInj6()
{
    __asm
    {
        cmp ebx, 0x00002AF9
        je pt1
        cmp ebx, 0x00002AFA
        je pt2
        cmp ebx, 0x00002AFB
        je pt3
        cmp ebx, 0x00002AFC
        je pt4
        cmp ebx, 0x00002AFD
        je pt5
        cmp ebx, 0x00001964
        jg RaidInj6jgs
        jmp RaidInj6jmp
        RaidInj6jgs :
        jmp RaidInj6jgc

            pt1 :
        mov[raid_number], 0x0
            jmp RaidInj6exjmp

            pt2 :
        mov[raid_number], 0x1
            jmp RaidInj6exjmp

            pt3 :
        mov[raid_number], 0x2
            jmp RaidInj6exjmp

            pt4 :
        mov[raid_number], 0x3
            jmp RaidInj6exjmp

            pt5 :
        mov[raid_number], 0x4
            jmp RaidInj6exjmp
    }
}

unsigned RaidInj7jmp = 0x0048DE73;
unsigned RaidInj7call = 0x0048D970;
void __declspec(naked) RaidInj7()
{
    __asm
    {
        cmp dword ptr[esp + 0x20], 0x00
        je leaderfirstje
        lea ecx, [esp + 0x20]
        push ecx
        mov dword ptr[esp + 0x24], 0x00001969
        mov ecx, edi
        jmp leaderfirstjmp

        leaderfirstje :
        mov dword ptr[esp + 0x20], 0x00001968
            mov ecx, edi
            lea edx, [esp + 0x20]
            push edx

            leaderfirstjmp :
        call RaidInj7call
            cmp dword ptr[raid_pointer_lead], 01
            jb RaidInj7jb
            lea ecx, [esp + 0x20]
            push ecx
            mov ecx, edi
            mov dword ptr[esp + 0x24], 0x00002AF9
            call RaidInj7call

            cmp dword ptr[raid_pointer_lead], 02
            jb RaidInj7jb
            lea ecx, [esp + 0x20]
            push ecx
            mov ecx, edi
            mov dword ptr[esp + 0x24], 0x00002AFA
            call RaidInj7call

            cmp dword ptr[raid_pointer_lead], 03
            jb RaidInj7jb
            lea ecx, [esp + 0x20]
            push ecx
            mov ecx, edi
            mov dword ptr[esp + 0x24], 0x00002AFB
            call RaidInj7call

            cmp dword ptr[raid_pointer_lead], 04
            jb RaidInj7jb
            lea ecx, [esp + 0x20]
            push ecx
            mov ecx, edi
            mov dword ptr[esp + 0x24], 0x00002AFC
            call RaidInj7call

            cmp dword ptr[raid_pointer_lead], 05
            jb RaidInj7jb
            lea ecx, [esp + 0x20]
            push ecx
            mov ecx, edi
            mov dword ptr[esp + 0x24], 0x00002AFD
            call RaidInj7call

            RaidInj7jb :
        jmp RaidInj7jmp
    }
}

unsigned RaidInj8jmp = 0x48DE73;
unsigned RaidInj8call = 0x48D970;
void __declspec(naked) RaidInj8()
{
    __asm
    {
        cmp dword ptr[esp + 0x20], 0x00
        mov ecx, edi
        je playerfirstje
        mov dword ptr[esp + 0x20], 0x00001969
        lea edx, [esp + 0x20]
        push edx
        jmp playerfirstjmp

        playerfirstje :
        lea eax, [esp + 0x20]
            mov dword ptr[esp + 0x20], 0x00001968
            push eax
            playerfirstjmp :
        call RaidInj8call

            cmp dword ptr[raid_pointer_lead], 01
            jb RaidInj8jb
            lea ecx, [esp + 0x20]
            push ecx
            mov ecx, edi
            mov dword ptr[esp + 0x24], 0x00002AF9
            call RaidInj8call

            cmp dword ptr[raid_pointer_lead], 02
            jb RaidInj8jb
            lea ecx, [esp + 0x20]
            push ecx
            mov ecx, edi
            mov dword ptr[esp + 0x24], 0x00002AFA
            call RaidInj8call

            cmp dword ptr[raid_pointer_lead], 03
            jb RaidInj8jb
            lea ecx, [esp + 0x20]
            push ecx
            mov ecx, edi
            mov dword ptr[esp + 0x24], 0x00002AFB
            call RaidInj8call

            cmp dword ptr[raid_pointer_lead], 04
            jb RaidInj8jb
            lea ecx, [esp + 0x20]
            push ecx
            mov ecx, edi
            mov dword ptr[esp + 0x24], 0x00002AFC
            call RaidInj8call

            cmp dword ptr[raid_pointer_lead], 05
            jb RaidInj8jb
            lea ecx, [esp + 0x20]
            push ecx
            mov ecx, edi
            mov dword ptr[esp + 0x24], 0x00002AFD
            call RaidInj8call

            RaidInj8jb :
        jmp RaidInj8jmp
    }
}

unsigned RaidInj9jmp = 0x53D4C8;
void __declspec(naked) RaidInj9()
{
    __asm
    {
        mov[esi + 0x10], 0x5D
        mov edx, [esi + 0x10]
        sub esp, 0x10
        jmp RaidInj9jmp
    }
}

unsigned RaidInj10jmp = 0x53D51E;
unsigned RaidInj10call = 0x573C00;
unsigned RaidInj10offsetcall = 0x22A011C;
unsigned RaidInj6Adr1 = 0x22A011C;
unsigned RaidInj6Adr2 = 0x227E338;
unsigned RaidInj6Adr3 = 0x227E334;
void __declspec(naked) RaidInj10()
{
    __asm
    {
        add esp, 0x0C
        add ebp, 0x0A
        add edi, 0xA
        pushad
        pushfd
        sub esp, 0x50
        mov dword ptr[esp + 0x30], 0x626D654D
        mov dword ptr[esp + 0x34], 0x3A737265
        mov byte ptr[esp + 0x38], 0x20
        lea ecx, [esp + 0x39]
        mov eax, [0x022AA728]
        mov eax, [eax]
        call Members
        mov byte ptr[ecx], 0x00
        lea edx, [esp + 0x30]
        mov ebx, 0x00000000
        push edx
        push ebx
        push 0xFFFFFF4D
        push edi
        push ebp
        push 0x022B69B0
        call RaidInj10call
        lea eax, [esp + 0x48]
        add esp, 0x18
        add esp, 0x50
        popfd
        popad
        add edi, 0x0F
        pushad
        pushfd
        sub esp, 0x50
        mov dword ptr[esp + 0x30], 0x656C6F52
        mov word ptr[esp + 0x34], 0x203A
        mov edx, [0x022AA730]
        cmp dword ptr[edx], 0x00
        je n1
        cmp dword ptr[edx], 0x01
        je n2
        mov dword ptr[esp + 0x36], 0x626D654D
        mov dword ptr[esp + 0x3A], 0x20737265
        mov word ptr[esp + 0x3E], 0x0020
        jmp n3
        n1 :
        mov dword ptr[esp + 0x36], 0x6461654C
            mov dword ptr[esp + 0x3A], 0x00007265
            jmp n3
            n2 :
        mov dword ptr[esp + 0x36], 0x20627553
            mov dword ptr[esp + 0x3A], 0x6461654C
            mov dword ptr[esp + 0x3E], 0x00007265
            n3 :
            lea edx, [esp + 0x30]
            mov ebx, 0x00000000
            push edx
            push ebx
            push 0xFFFFFF4D
            push edi
            push ebp
            push 0x022B69B0
            call RaidInj10call
            lea eax, [esp + 0x48]
            add esp, 0x18
            add esp, 0x50
            popfd
            popad
            add edi, 0x0F
            pushad
            pushfd
            sub esp, 0x50
            mov dword ptr[esp + 0x30], 0x72727543
            mov dword ptr[esp + 0x34], 0x20746E65
            mov dword ptr[esp + 0x38], 0x64696152
            mov dword ptr[esp + 0x3C], 0x2020203a
            mov edx, [raid_number]
            add edx, 0x31
            mov byte ptr[esp + 0x3F], dl
            mov byte ptr[esp + 0x40], 0x00
            lea edx, [esp + 0x30]
            mov ebx, 0x00000000
            push edx
            push ebx
            push 0xFFFFFF4D
            push edi
            push ebp
            push 0x022B69B0
            call RaidInj10call
            lea eax, [esp + 0x48]
            add esp, 0x18
            add esp, 0x50
            popfd
            popad
            add edi, 0x0F
            jmp RaidInj10jmp
    }
}

unsigned RaidInj11jmp = 0x446010;
void __declspec(naked) RaidInj11()
{
    __asm
    {
        lea eax, [eax + edx * 0x2]
        mov ecx, [raid_number]
        imul ecx, ecx, 0x1E
        add eax, ecx
        push eax
        mov ecx, 0x022AA71C
        jmp RaidInj11jmp
    }
}

unsigned RaidInj12jmp = 0x53CDC1;
unsigned RaidInj12r1 = 0x22AA750;
unsigned RaidInj12r2 = 0x227E334;
unsigned RaidInj12r3 = 0x227E338;
void __declspec(naked) RaidInj12()
{
    __asm
    {
        push eax
        push ecx
        push edx
        mov eax, ebx
        xor edx, edx
        mov ecx, 0x00000005
        div ecx
        mov ebx, edx
        mov ecx, RaidInj12r1
        mov[ecx], edx
        cmp dword ptr[esp + 0x1C], 0x005752D6
        je partychange
        cmp dword ptr[esp + 0x1C], 0x005757D8
        je partychange
        cmp dword ptr[esp + 0x1C], 0x00574D60
        je partychange
        jmp exits
        partychange :
        mov dword ptr[raid_number], eax
            mov dword ptr[RaidInj12r3], eax
            exits :
        pop edx
            pop ecx
            pop eax
            jmp RaidInj12jmp
    }
}

unsigned RaidInj14jmp = 0x53E97B;
unsigned RaidInj14call = 0x451B80;
void __declspec(naked) RaidInj14()
{
    __asm
    {
        mov eax, [raid_number]
        imul eax, eax, 0x1E
        add ebx, eax
        mov[esp + 0x3C], ebx
        call RaidInj14call
        jmp RaidInj14jmp
    }
}

unsigned RaidInj15jmp = 0x5403C9;
void __declspec(naked) RaidInj15()
{
    __asm
    {
        mov ecx, [raid_number]
        imul ecx, ecx, 0x1E
        add esi, ecx
        push esi
        mov ecx, 0x22AA71C
        jmp RaidInj15jmp
    }
}

void hook::raid()
{
    util::detour((void*)0x53F8CE, Render_Raid_Buttons, 6);
    util::detour((void*)0x53F464, Click_Raid_Button, 6);
    util::detour((void*)0x550F53, Send_Holding, 10);
    util::detour((void*)0x42B6CB, New_Send_BUttons_To_Memory, 7);

    uint8_t Raidtextcolor1[5]{ 0x68, 0x4D, 0xFF, 0xFF, 0xFF };
    util::write_memory((void*)0x53D5B5, Raidtextcolor1, 5);
    uint8_t Raidtextcolor2[5]{ 0x68, 0x4D, 0xFF, 0xFF, 0xFF };
    util::write_memory((void*)0x0053D655, Raidtextcolor2, 5);

    util::detour((void*)0x53E873, RaidInj1, 6);
    util::detour((void*)0x53F527, RaidInj2, 6);
    uint8_t RaidInjAdr3[3]{ 0x83, 0xFE, 0x96 };
    util::write_memory((void*)0x4EC981, RaidInjAdr3, 3);

    util::detour((void*)0x540732, RaidInj4, 9);
    util::detour((void*)0x53F656, RaidInj5, 9);
    util::detour((void*)0x4A44DA, RaidInj6, 6);
    util::detour((void*)0x48DE47, RaidInj7, 5);
    util::detour((void*)0x48DDE6, RaidInj8, 5);
    util::detour((void*)0x53D4C2, RaidInj9, 6);
    util::detour((void*)0x53D515, RaidInj10, 6);
    util::detour((void*)0x446007, RaidInj11, 9);
    util::detour((void*)0x53CDBB, RaidInj12, 6);

    uint8_t RaidInjAdr13[5]{ 0x83, 0xFB, 0x18, 0x77, 0x36 };
    util::write_memory((void*)0x53CD98, RaidInjAdr13, 5);
    util::detour((void*)0x53E972, RaidInj14, 9);
    util::detour((void*)0x5403C3, RaidInj15, 6);
}