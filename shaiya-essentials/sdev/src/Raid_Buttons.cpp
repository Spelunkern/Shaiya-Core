#include "util/util.h"
#include "include/main.h"

#include <string>
using namespace std;
float Float_Y = -21.0f; // POSICIÓN
float Float_X_Raid_1 = 380.0f; // Separación entre botones
float Float_X_Raid_2 = 402.0f; // Separación entre botones
float Float_X_Raid_3 = 424.0f; // Separación entre botones 
float Float_X_Raid_4 = 446.0f; // Separación entre botones
float Float_X_Raid_5 = 468.0f; // Separación entre botones
LPCSTR Raid_Button_1_Targa = "RaidButton1.tga";
LPCSTR Raid_Button_2_Targa = "RaidButton2.tga";
LPCSTR Raid_Button_3_Targa = "RaidButton3.tga";
LPCSTR Raid_Button_4_Targa = "RaidButton4.tga";
LPCSTR Raid_Button_5_Targa = "RaidButton5.tga";
DWORD Button_Call = 0x00429FD0;
DWORD Button_Call_2 = 0x0054FCE0;
DWORD Button_Call_3 = 0x0054F100;
DWORD Button_Call_4 = 0x00631BE0;
DWORD Button_Call_5 = 0x00551860;
unsigned char raid_pointer_1[2048]; 
unsigned char raid_pointer_2[2048];
unsigned char raid_pointer_3[2048];
unsigned char raid_pointer_4[2048];
unsigned char raid_pointer_5[2048];


_declspec(naked) void Raid_Button_1() {
	_asm {

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
		mov[esi + 0x00002120], ebx 
		mov[esi + 0x0000211C], ebx 
		fst dword ptr ds : [esi + 0x00002150] 
		fst dword ptr ds : [esi + 0x00002154] 
		mov[esi + 0x00002130], ebx 
		mov[esi + 0x0000212C], ebx 
		fst dword ptr ds : [esi + 0x00002160] 
		fstp dword ptr ds : [esi + 0x00002164] 
		mov[esi + 0x0000214C], ebx 
		mov[esi + 0x00002148], ebx 
		lea ebp, dword ptr ds : raid_pointer_1
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

_declspec(naked) void Raid_Button_2() {
	_asm {

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
		mov[esi + 0x00002120], ebx 
		mov[esi + 0x0000211C], ebx
		fst dword ptr ds : [esi + 0x00002150]
		fst dword ptr ds : [esi + 0x00002154]
		mov[esi + 0x00002130], ebx
		mov[esi + 0x0000212C], ebx
		fst dword ptr ds : [esi + 0x00002160]
		fstp dword ptr ds : [esi + 0x00002164]
		mov[esi + 0x0000214C], ebx
		mov[esi + 0x00002148], ebx
		lea ebp, dword ptr ds : raid_pointer_2
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

_declspec(naked) void Raid_Button_3() {
	_asm {

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
		mov[esi + 0x00002120], ebx 
		mov[esi + 0x0000211C], ebx 
		fst dword ptr ds : [esi + 0x00002150] 
		fst dword ptr ds : [esi + 0x00002154] 
		mov[esi + 0x00002130], ebx 
		mov[esi + 0x0000212C], ebx 
		fst dword ptr ds : [esi + 0x00002160] 
		fstp dword ptr ds : [esi + 0x00002164] 
		mov[esi + 0x0000214C], ebx 
		mov[esi + 0x00002148], ebx 
		lea ebp, dword ptr ds : raid_pointer_3
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


_declspec(naked) void Raid_Button_4() {
	_asm {

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
		mov[esi + 0x00002120], ebx 
		mov[esi + 0x0000211C], ebx 
		fst dword ptr ds : [esi + 0x00002150] 
		fst dword ptr ds : [esi + 0x00002154] 
		mov[esi + 0x00002130], ebx 
		mov[esi + 0x0000212C], ebx 
		fst dword ptr ds : [esi + 0x00002160] 
		fstp dword ptr ds : [esi + 0x00002164] 
		mov[esi + 0x0000214C], ebx 
		mov[esi + 0x00002148], ebx 
		lea ebp, dword ptr ds : raid_pointer_4
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



_declspec(naked) void Raid_Button_5() {
	_asm {

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
		mov[esi + 0x00002120], ebx 
		mov[esi + 0x0000211C], ebx 
		fst dword ptr ds : [esi + 0x00002150] 
		fst dword ptr ds : [esi + 0x00002154] 
		mov[esi + 0x00002130], ebx 
		mov[esi + 0x0000212C], ebx 
		fst dword ptr ds : [esi + 0x00002160] 
		fstp dword ptr ds : [esi + 0x00002164] 
		mov[esi + 0x0000214C], ebx 
		mov[esi + 0x00002148], ebx 
		lea ebp, dword ptr ds : raid_pointer_5
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

DWORD Send_Buttons_Call = 0x006307F3;
DWORD Continue_Addr = 0x0042B6D2;
DWORD Pointer_Talk_Button = 0;

_declspec(naked) void New_Send_Buttons_To_Memory()
{
	__asm
	{
		pushad
		pushfd

		// Llamar a la función original
		push 0x0001AC3C
		call Send_Buttons_Call
		add esp, 4

		mov ecx, eax
		mov Pointer_Talk_Button, eax

		// Injectar nuevos botones si eax != nullptr
		test eax, eax
		jz skip_buttons

		push ecx
		call Raid_Button_1
		mov ecx, eax
		call Raid_Button_2
		mov ecx, eax
		call Raid_Button_3
		mov ecx, eax
		call Raid_Button_4
		mov ecx, eax
		call Raid_Button_5
		pop ecx

		skip_buttons :
		mov eax, ecx
			// Obtener el ESP guardado desde el stack (ubicado en [ESP+16] después de pushad/pushfd)
			mov ebx, [esp + 16]    // EBX = ESP original antes de pushad
			mov[ebx + 0x18], eax   // Replicar escritura original en [ESP+0x18]
			mov byte ptr[ebx + 0x24], 0x21 // Replicar escritura original en [ESP+0x24] con valor 0x21

			popfd
			popad

			// Replicar las instrucciones reemplazadas
			xor eax, eax
			mov ecx, 0x34
			jmp Continue_Addr
	}
}

DWORD Render_Call = 0x00550120;
DWORD Render_Call_2 = 0x00550A20;
DWORD Render_Raid_Button_Return = 0x0053F8D4;
_declspec(naked) void Render_Raid_Buttons() {
	_asm {
		pushad
		mov eax, [edi + 0x08]
		mov ecx, [edi + 0x04]
		push eax
		lea edi, dword ptr ds : raid_pointer_1
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
		lea edi, dword ptr ds : raid_pointer_2
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
		lea edi, dword ptr ds : raid_pointer_3
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
		lea edi, dword ptr ds : raid_pointer_4
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
		lea edi, dword ptr ds : raid_pointer_5
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

DWORD Get_Click = 0x00550A10;
DWORD Raid_Click_Return = 0x0053F46A;
_declspec(naked) void Click_Raid_Button() {
	_asm {
		pushad
		lea ecx, dword ptr ds : raid_pointer_1
		call Get_Click
		test eax, eax
		jne Raid_1
		popad

		pushad
		lea ecx, dword ptr ds : raid_pointer_2
		call Get_Click
		test eax, eax
		jne Raid_2
		popad


		pushad
		lea ecx, dword ptr ds : raid_pointer_3
		call Get_Click
		test eax, eax
		jne Raid_3
		popad


		pushad
		lea ecx, dword ptr ds : raid_pointer_4
		call Get_Click
		test eax, eax
		jne Raid_4
		popad

		pushad
		lea ecx, dword ptr ds : raid_pointer_5
		call Get_Click
		test eax, eax
		jne Raid_5
		popad

		originalcode :
		cmp dword ptr ds : [0x022AA800] , ebp
			jmp Raid_Click_Return


			Raid_1 :
		popad
			mov byte ptr ds : [raid_pointer] , 0x00
			jmp originalcode

			Raid_2 :
		popad
			mov byte ptr ds : [raid_pointer] , 0x01
			jmp originalcode


			Raid_3 :
		popad
			mov byte ptr ds : [raid_pointer] , 0x02
			jmp originalcode

			Raid_4 :
		popad
			mov byte ptr ds : [raid_pointer] , 0x03
			jmp originalcode

			Raid_5 :
		popad
			mov byte ptr ds : [raid_pointer] , 0x04
			jmp originalcode
	}
}

DWORD original_code_addr = 0x00551072;
DWORD Send_Holding_Return = 0x00550F5D;
_declspec(naked) void Send_Holding() {
	_asm {
		cmp byte ptr ds : [raid_pointer] , 00
		je Send_Holding_Raid_1
		cmp byte ptr ds : [raid_pointer] , 01
		je Send_Holding_Raid_2
		cmp byte ptr ds : [raid_pointer] , 02
		je Send_Holding_Raid_3
		cmp byte ptr ds : [raid_pointer] , 03
		je Send_Holding_Raid_4
		cmp byte ptr ds : [raid_pointer] , 04
		je Send_Holding_Raid_5



	originalcode:
		cmp byte ptr ds : [esi + 0x06] , 00
			je original_code_conditional
			jmp Send_Holding_Return

			Send_Holding_Raid_1 :
		mov byte ptr ds : [raid_pointer_1 + 0x06] , 01
			jmp originalcode

			Send_Holding_Raid_2 :
		mov byte ptr ds : [raid_pointer_2 + 0x06] , 01
			jmp originalcode

			Send_Holding_Raid_3 :
		mov byte ptr ds : [raid_pointer_3 + 0x06] , 01
			jmp originalcode

			Send_Holding_Raid_4 :
		mov byte ptr ds : [raid_pointer_4 + 0x06] , 01
			jmp originalcode

			Send_Holding_Raid_5 :
		mov byte ptr ds : [raid_pointer_5 + 0x06] , 01
			jmp originalcode



		original_code_conditional:
		jmp original_code_addr
	}
}

void hook::raid_buttons() {
	util::detour((void*)0x0053F8CE, Render_Raid_Buttons, 6);
	util::detour((void*)0x0053F464, Click_Raid_Button, 6);
	util::detour((void*)0x00550F53, Send_Holding, 10);
	util::detour((void*)0x0042B6CB, New_Send_Buttons_To_Memory, 7);
}