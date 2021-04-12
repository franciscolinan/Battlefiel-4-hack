#include <iostream>
#include "Utils.h"
#include "Drawing.h"

int main()
{
	std::uint64_t local_player = 0;
	float health = 0;
	DWORD pid = 0;

	HWND hwnd = FindWindowA(NULL, "Battlefield 4");
	HBRUSH brush = CreateSolidBrush(0x000000FF);
	HDC hdc = GetDC(hwnd);

	if (!hwnd) 
	{
		std::cout << "Window not found!\n";
		std::cin.get();
		return 0;
	}

	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE phandle = OpenProcess(PROCESS_VM_READ, 0, pid);

	if (!phandle)
	{
		std::cout << "Could not get handle!\n";
		std::cin.get();
		return 0;
	}
	
	// Local player
	//
	std::uint64_t address = 0;
	ReadProcessMemory(phandle, (void*)CLIENT_GAME_CONTEXT, &address, sizeof(address), 0);
	ReadProcessMemory(phandle, (void*)(address + PLAYER_MANAGER), &address, sizeof(address), 0);
	ReadProcessMemory(phandle, (void*)(address + LOCAL_PLAYER), &local_player, sizeof(local_player), 0);

	ReadProcessMemory(phandle, (void*)(address + PLAYER_ARRAY), &address, sizeof(address), 0);
	std::uint64_t player = 0;
	std::uint64_t renderer = 0;
	ReadProcessMemory(phandle, (void*)GAME_RENDERER, &renderer, sizeof(renderer), 0);
	ReadProcessMemory(phandle, (void*)(renderer + RENDER_VIEW), &renderer, sizeof(renderer), 0);
	D3DXVECTOR3 position;
	D3DXVECTOR3 head_position;

	while (true)
	{
		for (size_t i = 0; i < 64; i++)
		{
			ReadProcessMemory(phandle, (void*)(address + (i * 0x8)), &player, sizeof(player), 0);
			health = GetHealth(phandle, player);
			position = GetPosition(phandle, player);

			head_position = position;
			head_position.y = head_position.y + 2;

			if (GetTeamId(phandle, player) != GetTeamId(phandle, local_player) && health > 0 && health <= 100 && (WorldToScreen(phandle, renderer, position) || WorldToScreen(phandle, renderer, head_position)))
			{	
				float height = head_position.y - position.y;
				float width = height / 2.4f;
				DrawBorderBox(hdc, brush, head_position.x - (width / 2), head_position.y - height, width, height, 1);
			}
		}
	}
}