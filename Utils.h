#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <cstdint>
#include "Offsets.h"

auto WorldToScreen(HANDLE phandle, std::uint64_t renderer, D3DXVECTOR3& vLocVec4) -> bool
{
	D3DXMATRIXA16 ViewProj;
	ReadProcessMemory(phandle, (void*)(renderer + VIEW_PROJECTION), &ViewProj, sizeof(ViewProj), 0);

	float mX = (1920) / 2;
	float mY = (1080) / 2;

	float w =
		ViewProj(0, 3) * vLocVec4.x +
		ViewProj(1, 3) * vLocVec4.y +
		ViewProj(2, 3) * vLocVec4.z +
		ViewProj(3, 3);

	if (w < 0.65f)
	{
		vLocVec4.z = w;
		return false;
	}

	float x =
		ViewProj(0, 0) * vLocVec4.x +
		ViewProj(1, 0) * vLocVec4.y +
		ViewProj(2, 0) * vLocVec4.z +
		ViewProj(3, 0);

	float y =
		ViewProj(0, 1) * vLocVec4.x +
		ViewProj(1, 1) * vLocVec4.y +
		ViewProj(2, 1) * vLocVec4.z +
		ViewProj(3, 1);

	vLocVec4.x = (mX + mX * x / w);
	vLocVec4.y = (mY - mY * y / w);
	vLocVec4.z = w;
	if (vLocVec4.x > 1920 || vLocVec4.y > 1080 || vLocVec4.x < 0 || vLocVec4.y < 0)
		return false;
	return true;
}

auto GetHealth(HANDLE phandle, std::uint64_t player) -> float
{
	std::uint64_t address = 0;
	float health = 0;
	ReadProcessMemory(phandle, (void*)(player + CONTROLLER_CONTROLLABLE), &address, sizeof(address), 0);
	ReadProcessMemory(phandle, (void*)(address + CHARACTER_DATA), &address, sizeof(address), 0);
	ReadProcessMemory(phandle, (void*)(address + HEALTH), &health, sizeof(health), 0);
	return health;
}

auto GetTeamId(HANDLE phandle, std::uint64_t player) -> std::uint32_t
{
	int team = 0;
	ReadProcessMemory(phandle, (void*)(player + TEAM_ID), &team, sizeof(team), 0);
	return team;
}

auto GetPosition(HANDLE phandle, std::uint64_t player) -> D3DXVECTOR3
{
	D3DXVECTOR3 position;
	std::uint64_t address = 0;
	ReadProcessMemory(phandle, (void*)(player + CONTROLLER_CONTROLLABLE), &address, sizeof(address), 0);
	ReadProcessMemory(phandle, (void*)(address + CHARACTER_INFO), &address, sizeof(address), 0);
	ReadProcessMemory(phandle, (void*)(address + POSITION), &position, sizeof(position), 0);
	return position;
}
