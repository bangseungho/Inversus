#include "Board.h"

# define ERRORGRID 2

// ��ü ȭ�� ȸ�� �κ�
void Board::rectScene(HDC memDC, const RECT& rectWindow)
{
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(100, 100, 100));
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	Rectangle(memDC, rectWindow.left, 100, rectWindow.right, rectWindow.bottom); 
	SelectObject(memDC, oldBrush);
	DeleteObject(hBrush);
}


// ���ھ� �κ�
void Board::scoreScene(HDC memDC, const RECT& rectWindow)
{
	HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	HPEN hPen = (HPEN)CreatePen(PS_NULL, 0, NULL);
	HPEN oldPen = (HPEN)SelectObject(memDC, hPen);
	Rectangle(memDC, -1, -1, rectWindow.right + 1, 100);
	SelectObject(memDC, oldBrush);
	SelectObject(memDC, oldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

// ���� �÷��� ȭ�� �κ�
void Board::playScene(HDC memDC, const RECT& playWindow)
{
	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	HPEN hPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
	HPEN oldPen = (HPEN)SelectObject(memDC, hPen);
	Rectangle(memDC, playWindow.left, playWindow.top, playWindow.right, playWindow.bottom); 
	SelectObject(memDC, oldBrush);
	SelectObject(memDC, oldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

void Board::gridScene(HDC memDC, const RECT& playWindow, const int& ver, const int& hor, const int& BOARDSIZE)
{
	HPEN hPen = (HPEN)CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
	HPEN oldPen = (HPEN)SelectObject(memDC, hPen);

	for (int i = 1; i < ver; i++)
	{
		MoveToEx(memDC, playWindow.left + (i * BOARDSIZE), playWindow.top, NULL);
		LineTo(memDC, playWindow.left + (i * BOARDSIZE), playWindow.bottom - ERRORGRID);
	}
	for (int j = 1; j < hor; j++)
	{
		MoveToEx(memDC, playWindow.left, playWindow.top + (j * BOARDSIZE), NULL);
		LineTo(memDC, playWindow.right - ERRORGRID, playWindow.top + (j * BOARDSIZE));
	}

	SelectObject(memDC, oldPen);
	DeleteObject(hPen);
}

void Board::isOffScene(HDC memDC, const RECT& playWindow, const int& ver, const int& hor, const int& BOARDSIZE, int i, int j)
{
	HBRUSH hBrush1 = (HBRUSH)GetStockObject(BLACK_BRUSH);
	HBRUSH hBrush2 = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// ���� ��ֹ��� ���� �÷��� ����
	HBRUSH hBrush3 = (HBRUSH)CreateSolidBrush(RGB(76, 100, 0)); 

	HPEN hPen = (HPEN)CreatePen(PS_NULL, 0, NULL);
	HPEN oldPen = (HPEN)SelectObject(memDC, hPen);
	static HBRUSH oldBrush;

	if (isOn == 0 && _obstacle == false)
		oldBrush = (HBRUSH)SelectObject(memDC, hBrush1);
	if (isOn == 1 && _obstacle == false)
		oldBrush = (HBRUSH)SelectObject(memDC, hBrush2);
	if (_obstacle == true)
		oldBrush = (HBRUSH)SelectObject(memDC, hBrush3);
	Rectangle(memDC, playWindow.left + (j * BOARDSIZE), playWindow.top + (i * BOARDSIZE), playWindow.left + (j * BOARDSIZE + BOARDSIZE), playWindow.top + (i * BOARDSIZE + BOARDSIZE));

	SelectObject(memDC, oldPen);
	SelectObject(memDC, oldBrush);
	DeleteObject(hBrush1);
	DeleteObject(hBrush2);
	DeleteObject(hBrush3);
	DeleteObject(hPen);
}