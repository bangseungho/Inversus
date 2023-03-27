#include "Creature.h"

void Creature::CreateMotion(HDC memDC)
{
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hBrush2 = (HBRUSH)CreateSolidBrush(RGB(200, 100, 0));
	HBRUSH hBrush3 = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	static HBRUSH oldBrush;
	static HBRUSH oldBrush2;
	static HBRUSH oldBrush3;

	HPEN hPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(200, 100, 0));
	HPEN hPen2 = (HPEN)CreatePen(PS_NULL, NULL, NULL);
	HPEN hPen3 = (HPEN)CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	static HPEN oldPen;
	static HPEN oldPen2;

	if (CreateSizeCnt == true)
	{
		SelectObject(memDC, hPen2);

		if(_creatureType == CT_PLAYER)
			SelectObject(memDC, hBrush3);
		if(_creatureType == CT_ENEMY)
			SelectObject(memDC, hBrush2);
	}
	else if (CreateSizeCnt == false)
	{
		SelectObject(memDC, hBrush);

		if (_creatureType == CT_PLAYER)
			SelectObject(memDC, hPen3);
		if (_creatureType == CT_ENEMY)
			SelectObject(memDC, hPen);
	}

	RoundRect(memDC, _PosX - CreateSize, _PosY - CreateSize, _PosX + CreateSize, _PosY + CreateSize, 20 + (CreateSize - 30) , 20 + (CreateSize - 30 ));

	SelectObject(memDC, oldBrush);
	SelectObject(memDC, oldBrush2);
	SelectObject(memDC, oldBrush3);
	SelectObject(memDC, oldPen);
	SelectObject(memDC, oldPen2);
	DeleteObject(hBrush);
	DeleteObject(hBrush2);
	DeleteObject(hBrush3);
	DeleteObject(hPen);
	DeleteObject(hPen2);
	DeleteObject(hPen3);
}