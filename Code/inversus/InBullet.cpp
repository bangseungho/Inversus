#include "InBullet.h"
#include<Windows.h>
#include<math.h>
#include"Player.h"


const double PI = 3.1415926;

void InBullet::Init(int index)
{
	this->_PosX = 12 * cos(th *  PI / 180);
	this->_PosY = 12 * sin(th *  PI / 180);
}

void InBullet::Paint(HDC hdc, Player& player, int index)
{
	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH hBrush2 = (HBRUSH)CreateSolidBrush(RGB(200, 100, 0));

	HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);
	HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

	static HBRUSH oldBrush;
	static HBRUSH oldBrush2;

	if (_spcial == true)
	{
		oldBrush2 = (HBRUSH)SelectObject(hdc, hBrush2);
	}
	else
	{
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}
	Ellipse(hdc, player._PosX + _PosX - 4, player._PosY + _PosY - 4,
		player._PosX + _PosX + 4, player._PosY + _PosY + 4);


	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldBrush2);
	SelectObject(hdc, oldPen);
	DeleteObject(hBrush2);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}