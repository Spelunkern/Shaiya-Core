#include "util/util.h"
#include "include/main.h"

BYTE xz[] = "\x00\xCA\x9A\x3B\x00\xE1\xF5\x05\x80\x96\x98\x00\x40\x42\x0F\x00\xA0\x86\x01\x00\x10\x27\x00\x00\xE8\x03\x00\x00\x64\x00\x00\x00\x0A\x00\x00\x00\x01\x00\x00\x00";
DWORD* xzadr = (DWORD*)&xz; // 

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

int raid_pointer = 0;
int raid_pointer_lead = 5;
DWORD RaidInj1jmp = 0x0053E879;
void __declspec(naked) RaidInj1()

{
	__asm
	{
		push ecx
		mov ecx, [raid_pointer]
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

DWORD RaidInj2jmp = 0x0053F52D;
void __declspec(naked) RaidInj2()

{
	__asm
	{
		mov ecx, [raid_pointer]
		imul ecx, ecx, 0x1E
		add esi, ecx
		push esi
		mov ecx, 0x022AA71C
		jmp RaidInj2jmp
	}
}

DWORD RaidInj4jmp = 0x0054073B;
DWORD RaidInj4call = 0x00451B80;
void __declspec(naked) RaidInj4()

{
	__asm
	{
		mov eax, [raid_pointer]
		imul eax, eax, 0x1E
		add ebx, eax
		mov dword ptr[esp + 0x30], ebx
		call RaidInj4call
		jmp RaidInj4jmp

	}
}

DWORD RaidInj5jmp = 0x0053F65F;
DWORD RaidInj5call = 0x00451B80;
void __declspec(naked) RaidInj5()

{
	__asm
	{
		mov eax, [raid_pointer]
		imul eax, eax, 0x1E
		add ebx, eax
		mov dword ptr[esp + 0x30], ebx
		call RaidInj5call
		jmp RaidInj5jmp

	}
}

DWORD RaidInj6jmp = 0x004A44E0;
DWORD RaidInj6cazz = 0x0227E334;
DWORD RaidInj6altjmp = 0x004A4B59;
DWORD RaidInj6jgl = 0x004A491F;
//-------------------
DWORD RaidInj6exjmp = 0x4A4B59;
DWORD RaidInj6jgc = 0x4A491F;
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
		mov[raid_pointer], 0x0
			jmp RaidInj6exjmp

			pt2 :
		mov[raid_pointer], 0x1
			jmp RaidInj6exjmp

			pt3 :
		mov[raid_pointer], 0x2
			jmp RaidInj6exjmp

			pt4 :
		mov[raid_pointer], 0x3
			jmp RaidInj6exjmp

			pt5 :
		mov[raid_pointer], 0x4
			jmp RaidInj6exjmp


	}
}

DWORD RaidInj7jmp = 0x0048DE73;
DWORD RaidInj7call = 0x0048D970;
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

DWORD RaidInj8jmp = 0x0048DE73;
DWORD RaidInj8call = 0x0048D970;
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

DWORD RaidInj9jmp = 0x0053D4C8;
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

DWORD RaidInj10jmp = 0x0053D51E;
DWORD RaidInj10call = 0x00573C00;
DWORD RaidInj10offsetcall = 0x022A011C;
DWORD RaidInj6Adr1 = 0x022A011C;
DWORD RaidInj6Adr2 = 0x0227E338;
DWORD RaidInj6Adr3 = 0x0227E334;

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
		push 0xFFFFFFFF
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
			push 0xFFFFFFFF
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
			mov edx, [raid_pointer]
			add edx, 0x31
			mov byte ptr[esp + 0x3F], dl
			mov byte ptr[esp + 0x40], 0x00
			lea edx, [esp + 0x30]
			mov ebx, 0x00000000
			push edx
			push ebx
			push 0xFFFFFFFF
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

DWORD RaidInj11jmp = 0x00446010;
void __declspec(naked) RaidInj11()

{
	__asm
	{
		lea eax, [eax + edx * 0x2]
		mov ecx, [raid_pointer]
		imul ecx, ecx, 0x1E
		add eax, ecx
		push eax
		mov ecx, 0x022AA71C
		jmp RaidInj11jmp

	}
}


DWORD RaidInj12jmp = 0x0053CDC1;
DWORD RaidInj12r1 = 0x022AA750;
DWORD RaidInj12r2 = 0x0227E334;
DWORD RaidInj12r3 = 0x0227E338;
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
		mov dword ptr[raid_pointer], eax
			mov dword ptr[RaidInj12r3], eax
			exits :
		pop edx
			pop ecx
			pop eax
			jmp RaidInj12jmp

	}
}

DWORD RaidInj14jmp = 0x0053E97B;
DWORD RaidInj14call = 0x00451B80;
void __declspec(naked) RaidInj14()

{
	__asm
	{
		mov eax, [raid_pointer]
		imul eax, eax, 0x1E
		add ebx, eax
		mov[esp + 0x3C], ebx
		call RaidInj14call
		jmp RaidInj14jmp
	}
}

DWORD RaidInj15jmp = 0x005403C9;
void __declspec(naked) RaidInj15()

{
	__asm
	{
		mov ecx, [raid_pointer]
		imul ecx, ecx, 0x1E
		add esi, ecx
		push esi
		mov ecx, 0x022AA71C
		jmp RaidInj15jmp
	}
}
void hook::RAID() {
	//Raid Text Color 1
	BYTE Raidtextcolor1[] = { 0x68, 0xFF, 0xFF, 0xFF, 0xFF };
	util::write_memory((void*)0x0053D5B5, Raidtextcolor1, 5);
	//Raid Text Color 2
	BYTE Raidtextcolor2[] = { 0x68, 0xFF, 0xFF, 0xFF, 0xFF };
	util::write_memory((void*)0x0053D655, Raidtextcolor2, 5);
	util::detour((void*)0x0053E873, RaidInj1, 6);
	util::detour((void*)0x0053F527, RaidInj2, 6);
	BYTE RaidInjAdr3[] = { 0x83, 0xFE, 0x96 };
	util::write_memory((void*)0x004EC981, RaidInjAdr3, 3);
	util::detour((void*)0x00540732, RaidInj4, 9);
	util::detour((void*)0x0053F656, RaidInj5, 9);
	util::detour((void*)0x004A44DA, RaidInj6, 6);
	util::detour((void*)0x0048DE47, RaidInj7, 5);
	util::detour((void*)0x0048DDE6, RaidInj8, 5);
	util::detour((void*)0x0053D4C2, RaidInj9, 6);
	util::detour((void*)0x0053D515, RaidInj10, 6);
	util::detour((void*)0x00446007, RaidInj11, 9);
	util::detour((void*)0x0053CDBB, RaidInj12, 6);
	BYTE RaidInjAdr13[] = { 0x83, 0xFB, 0x18, 0x77, 0x36 };
	util::write_memory((void*)0x0053CD98, RaidInjAdr13, 5);
	util::detour((void*)0x0053E972, RaidInj14, 9);
	util::detour((void*)0x005403C3, RaidInj15, 6);
}