#pragma once
#include<Windows.h>
#include"Board.h"

class Player;
class Enemy;
class DieEmotion;
class SpecialBullet;
class InBullet;

#define DIEMOTION_NUM 3

class Bullet
{
public:
	Bullet();
	~Bullet();

public:
	int _PosX;
	int _PosY;
	int _dir;
	BOOL _spcial = false;
	void Paint(HDC hdc, int xP, int yP, int _dir);
	void gPaint(HDC hdc, int xP, int yP, int _dir, int j);
	void collision(Board board[][19], int hor, int ver, const RECT& playWindow);
	void enemycollision(Enemy* enemy[10], int hor, int ver, const RECT& playWindow, int* score, HWND hWnd, int* iscollision, SpecialBullet* spbullet[10], int* spbullet_index);
	RECT bullRect = { 0, };
	RECT brect[13][19] = { 0, };
	RECT enemyRect[10] = { 0, };
	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	bool _isOn;
	DieEmotion* _dieEmotion[DIEMOTION_NUM];
};

