#pragma once
#include <Windows.h>

void DrawFilledRect(HDC hdc, HBRUSH brush, int x, int y, int w, int h)
{
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdc, &rect, brush);
}

void DrawBorderBox(HDC hdc, HBRUSH brush, int x, int y, int w, int h, int thickness)
{
	DrawFilledRect(hdc, brush, x, y, w, thickness);
	DrawFilledRect(hdc, brush, x, y, thickness, h);
	DrawFilledRect(hdc, brush, (x + w), y, thickness, h);
	DrawFilledRect(hdc, brush, x, y + h, w + thickness, thickness);
}