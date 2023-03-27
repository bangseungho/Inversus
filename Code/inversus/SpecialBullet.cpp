#include "SpecialBullet.h"

void SpecialBullet::Paint(HDC memDC)
{
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 100, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	HPEN hPen = (HPEN)CreatePen(PS_NULL, 1, NULL);
	HPEN oldPen = (HPEN)SelectObject(memDC, hPen);

	spbrect = { _PosX - SPBULLETSIZE, _PosY - SPBULLETSIZE, _PosX + SPBULLETSIZE, _PosY + SPBULLETSIZE };
	Ellipse(memDC, spbrect.left, spbrect.top, spbrect.right, spbrect. bottom);

	SelectObject(memDC, oldBrush);
	SelectObject(memDC, oldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}