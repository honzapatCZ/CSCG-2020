// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include "mem.h"   
#include "geom.h"
#include <sstream>
#include <string> 

extern "C" void EmojiHook();
extern "C" uintptr_t sendEmojiBackAddress = 0;
extern "C" DWORD sendEmojiValue = 0;

namespace patch
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}

bool HookFunc(void * toHook, void * ourFunc, int len) {
	if (len < 5)
	{
		return false;
	}

	DWORD currentProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &currentProtection);

	memset(toHook, 0x90, len);

	/*DWORD relativeAddress = ((DWORD)ourFunc - (DWORD) toHook);

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = relativeAddress;
	*/
	DWORD temp;
	VirtualProtect(toHook, len, currentProtection, &temp);

}


DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "aMazeIng Hax initialized\n";

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"GameAssembly.dll");
	std::cout << "Base: " << moduleBase << std::endl;
	uintptr_t uPlayer = (uintptr_t)GetModuleHandle(L"UnityPlayer.dll");
	std::cout << "Unity Player: " << uPlayer << std::endl;
	//calling it with NULL also gives you the address of the .exe module
	//moduleBase = (uintptr_t)GetModuleHandle(NULL);


	uintptr_t sendEmojiHookAddress = moduleBase + 0x6d572f;
	std::cout << "Emoji Send Address: " << sendEmojiHookAddress << std::endl;
	int sendEmojiHookL = 5;
	sendEmojiBackAddress = sendEmojiHookAddress + sendEmojiHookL;

	HookFunc((void*)sendEmojiHookAddress, EmojiHook, sendEmojiHookL);


	bool bManual = false;

	BYTE xrestore[6] = { 0 };
	BYTE yrestore[6] = { 0 };
	BYTE zrestore[6] = { 0 };
	vec3* position = nullptr;

	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD7) & 1)
		{
			if (bManual){
				//50 8D 4C 24 1C 51 8B CE FF D2 the original stack setup and call
				mem::Patch((BYTE*)(uPlayer + 0x1155914), xrestore, 6);
				mem::Patch((BYTE*)(uPlayer + 0x115591D), yrestore, 6);
				mem::Patch((BYTE*)(uPlayer + 0x1155926), zrestore, 6);
			}
			break;
		}

		if (GetAsyncKeyState('I') & 1)
		{
			sendEmojiValue = sendEmojiValue + 1;
			std::cout << "Current emoji value: " << sendEmojiValue << std::endl;
		}
		if (GetAsyncKeyState('K') & 1)
		{
			sendEmojiValue = sendEmojiValue - 1;
			std::cout << "Current emoji value: " << sendEmojiValue << std::endl;

		}


		if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
			bManual = !bManual;
			std::cout << "[*] " << ((bManual == 1) ? "Enabled" : "Disabled");
			
			if (bManual)
			{
				position = (vec3*)mem::FindDMAAddy(moduleBase + 0x00AFA430, { 0x110, 0x788, 0x130, 0x28, 0x150 });

				//x 0x1155914
				//y 0x115591D
				//y 0x1155926

				
				mem::Patch(xrestore, (BYTE *)(uPlayer + 0x1155914), 6);
				mem::Nop((BYTE*)(uPlayer + 0x1155914), 6);

				std::cout << "y instr addr " << std::hex << static_cast<int>(uPlayer + 0x1155914);

				std::cout << " y instr ";
				for (auto val : xrestore) printf("\\x%.2x", val);
				std::cout << std::endl;
											   

				mem::Patch(yrestore, (BYTE *)(uPlayer+0x115591D), 6);
				mem::Nop((BYTE*)(uPlayer + 0x115591D), 6);

				std::cout << "y instr addr " << std::hex << static_cast<int>(uPlayer + 0x115591D);

				std::cout << " y instr ";
				for (auto val : yrestore) printf("\\x%.2x", val);
				std::cout << std::endl;
				

				mem::Patch(zrestore, (BYTE *)(uPlayer + 0x1155926), 6);
				mem::Nop((BYTE*)(uPlayer + 0x1155926), 6);
				std::cout << "y instr addr " << std::hex << static_cast<int>(uPlayer + 0x1155926);
				
				std::cout << " y instr ";
				for (auto val : zrestore) printf("\\x%.2x", val);
				std::cout << std::endl;

				
			}

			else
			{
				//50 8D 4C 24 1C 51 8B CE FF D2 the original stack setup and call
				mem::Patch((BYTE*)(uPlayer + 0x1155914), xrestore, 6);
				mem::Patch((BYTE*)(uPlayer + 0x115591D), yrestore, 6);
				mem::Patch((BYTE*)(uPlayer + 0x1155926), zrestore, 6);


			}
		}
		if (bManual)
		{
			std::cout << "X: " << (int)position->x << " Y:" << (int)position->y << " Z:" << (int)position->z << std::endl;

			if (GetAsyncKeyState(VK_NUMPAD9) & 1)
			{
				position->y += 1;
			}
			if (GetAsyncKeyState(VK_NUMPAD3) & 1)
			{
				position->y -= 1;
			}
			if (GetAsyncKeyState(VK_NUMPAD6) & 1)
			{
				position->x += 1;
			}
			if (GetAsyncKeyState(VK_NUMPAD4) & 1)
			{
				position->x -= 1;
			}
			if (GetAsyncKeyState(VK_NUMPAD8) & 1)
			{
				position->z += 1;
			}
			if (GetAsyncKeyState(VK_NUMPAD2) & 1)
			{
				position->z -= 1;
			}
			std::cout << "X: " << (int)position->x << " Y:" << (int)position->y << " Z:" << (int)position->z << std::endl;
		}
		/*
		//no recoil NOP
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bRecoil = !bRecoil;

			if (bRecoil)
			{
				mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
			}

			else
			{
				//50 8D 4C 24 1C 51 8B CE FF D2 the original stack setup and call
				mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
			}
		}

		//need to use uintptr_t for pointer arithmetic later
		uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10F4F4);

		//continuous writes / freeze

		if (localPlayerPtr)
		{
			if (bHealth)
			{

				//*localPlayerPtr = derference the pointer, to get the localPlayerAddr
				// add 0xF8 to get health address
				//cast to an int pointer, this pointer now points to the health address
				//derference it and assign the value 1337 to the health variable it points to
				*(int*)(*localPlayerPtr + 0xF8) = 1337;
			}

			if (bAmmo)
			{
				//We aren't external now, we can now efficiently calculate all pointers dynamically
				//before we only resolved pointers when needed for efficiency reasons
				//we are executing internally, we can calculate everything when needed
				uintptr_t ammoAddr = mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0 });
				int* ammo = (int*)ammoAddr;
				*ammo = 1337;

				//or just
				*(int*)mem::FindDMAAddy(moduleBase + 0x10F4F4, { 0x374, 0x14, 0x0 }) = 1337;
			}

		}*/
		Sleep(5);
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}