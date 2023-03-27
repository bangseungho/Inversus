#pragma once
#include<Windows.h>

#define SPBULLETSIZE 6

class SpecialBullet
{
public: 
	void Paint(HDC memDC);

public:
	int _PosX;
	int _PosY;
	int _fixPosX;
	int _fixPosY;
	int _th = 0;
	BOOL _birth = false;
	RECT spbrect = { 0, };
};

