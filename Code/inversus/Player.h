#pragma once
#include<windows.h>
#include"Board.h"
#include"Bullet.h"
#include"Creature.h"

class Player : public Creature
{
public:
	Player() : Creature( 505, 435, CT_PLAYER ),  _startcircle{ 0, 0 }, _dir(10), _dir2(0), _stickflag(false), rectlize{ 0, 0 }, _cango(true)
		, _dieMotionSize(-30 + rand() % 60), _dieMotionSize2(-30 + rand() % 60), _dieMotionSize3(-30 + rand() % 60), _dieMotionFlag(false)
	{
		
	}
	~Player()
	{

	}

private:
	POINT _startcircle;
	int _hor = 0;
	int _ver = 0;
	int savedir = 0;
public:
	void Paint(HDC memDC);
	void SetPos(int level, Board board[][19], int ver, int hor, const RECT& playWindow);
	void startrange(HDC memDC, Board board[][19], int ver, int hor, const RECT& playWindow);
	void moving(HDC hdc, HWND hWnd, Board board[][19], const RECT& playWindow, Enemy* enemy[]);
	void DiePaint(HDC memDC, int hor, int ver, Board board[][19], const RECT& playWindow, int iscollision);
	int _dieMotionSize;
	int _dieMotionSize2;
	int _dieMotionSize3;
	int _dieMotionFlag;
	POINT IJ;
	int _dir;
	int _dir2;
	int _dir3;
	RECT prect = { 0, };
	RECT brect[13][19] = { 0, };
	RECT isOn_rect[13][19] = { 0, };
	//int _PosX;
	//int _PosY;
	bool _stickflag;
	bool _cango;
	POINT rectlize;
	BOOL _invincivleMode = false;
	int combo = 0;
	BOOL die = true;
	POINT _lastPos = { 0 };
	BOOL overcnt = false;
	int life = 3;
};

