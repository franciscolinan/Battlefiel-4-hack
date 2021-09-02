#pragma once
#include <Windows.h>

auto DrawFilledRect(HDC hdc, HBRUSH brush, int x, int y, int w, int h) -> void
{
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdc, &rect, brush);
}

auto DrawBorderBox(HDC hdc, HBRUSH brush, int x, int y, int w, int h, int thickness) -> void
{
	DrawFilledRect(hdc, brush, x, y, w, thickness);
	DrawFilledRect(hdc, brush, x, y, thickness, h);
	DrawFilledRect(hdc, brush, (x + w), y, thickness, h);
	DrawFilledRect(hdc, brush, x, y + h, w + thickness, thickness);
}
