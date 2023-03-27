#pragma once
#include<Windows.h>

class Player;
class InBullet;

class InBullet
{
public:
	int _PosX;
	int _PosY;
	double th = 1;
	int speed = 25;
	BOOL _spcial = false;

public:
	void Init(int index);
	void Paint(HDC hdc, Player& player, int index);
};

