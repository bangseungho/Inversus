#pragma once
#include<windows.h>
#include"Player.h"
#include"Creature.h"

#define ADD8TILE 50
#define SPBULLET_NUM 500

class Enemy;
class Player;
class Specialbullet;
static int dethcnt = 0;
extern SpecialBullet* spbullet[SPBULLET_NUM];
extern int spbullet_index;
extern int score;
extern Player player;


class Enemy : public Creature
{
public:
	// cpp에 선언하겠다
	Enemy();
	~Enemy();

public:
	BOOL _delete = false;
	BOOL _Slash = true;
	void Paint(HDC hdc, HWND hWnd);
	void induce(const Player& player, int hor, int ver, Board board[][19], const RECT& playWindow);
	RECT enemyRect[10] = { 0, };
	RECT brect[13][19] = { 0, };
	int _die;
	BOOL _exixt;
	int _dieMotionSize;
	int _dieMotionSize2;
	int _dieMotionSize3;
	BOOL _dieMotionFlag;
	RECT _DieMotion[3] = { 0, };
	void DiePaint(HDC memDC, int hor, int ver, Board board[][19], const RECT& playWindow, int iscollision);
	void DiePaint2(HDC memDC, int hor, int ver, Board board[][19], const RECT& playWindow, int iscollisionm, HWND hWnd);
	void tileSlash(HDC memDC, int hor, int ver);
};