#pragma once
#include<windows.h>

class Board
{
public:
	Board() : _PosX(0), _PosY(0), isOn(false), _flag(true), _obstacle(false)
	{

	}
	~Board()
	{

	}

	void rectScene(HDC memDC, const RECT& rectWindow);
	void scoreScene(HDC memDC, const RECT& rectWindow);
	void playScene(HDC memDC, const RECT& playWindow);
	void gridScene(HDC memDC, const RECT& playWindow, const int& ver, const int& hor, const int& BOARDSIZE);
	void isOffScene(HDC memDC, const RECT& playWindow, const int& ver, const int& hor, const int& BOARDSIZE, int i, int j);
public:
	bool isOn;
	int _PosX;
	int _PosY;
	int _flag;
	int _obstacle;
};

