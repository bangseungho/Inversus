#include "Bullet.h"
#include"Enemy.h"
#include"DieEmotion.h"
#include"SpecialBullet.h"
#include"InBullet.h"

#define SPBULLET_NUM 500
#define BULLET_NUM 6

extern SpecialBullet* spbullet[SPBULLET_NUM];
extern InBullet inbullet[BULLET_NUM];
extern int spbullet_index;
extern int index;


Bullet::Bullet() : _PosX(0), _PosY(0), _dir(0), _isOn(false)
{

} 

Bullet::~Bullet()
{
	
}

void Bullet::Paint(HDC hdc, int xP, int yP, int d)
{
	HPEN hPen = CreatePen(PS_NULL, 0, BLACK_PEN);
	HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
	
	HBRUSH hBrush2 = CreateSolidBrush(RGB(190, 70, 0));

	static HBRUSH oldBrush;
	static HBRUSH oldBrush2;

	if (inbullet[index]._spcial == true)
	{
		oldBrush2 = (HBRUSH)SelectObject(hdc, hBrush2);
	}
	else
	{
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}

	DeleteObject(oldBrush);
	DeleteObject(oldBrush2);
	DeleteObject(hBrush);
	DeleteObject(hBrush2);
	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}

void Bullet::gPaint(HDC hdc, int xP, int yP, int d, int i)
{
	SetROP2(hdc, R2_COPYPEN);
	HPEN hPen = CreatePen(PS_NULL, 0, BLACK_PEN);
	HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(RGB(i * 40 + 30, i * 40 + 30, i * 40 + 30));
	HBRUSH hBrush2 = CreateSolidBrush(RGB(i * 10 + 200, i * 30 + 100, i * 51));

	if (inbullet[index]._spcial == true)
	{
		_spcial = true;
	}

	static HBRUSH oldBrush;
	static HBRUSH oldBrush2;

	if (inbullet[index]._spcial == true)
	{
		oldBrush2 = (HBRUSH)SelectObject(hdc, hBrush2);
	}
	else
	{
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	}

	if (_spcial == false)
	{
		switch (_dir) {				
		case 0:
			Rectangle(hdc, xP, yP - 7, xP - 10, yP + 3);
			break;
		case 1:
			Rectangle(hdc, xP - 7, yP, xP + 3, yP - 10);
			break;
		case 2:
			Rectangle(hdc, xP, yP - 7, xP + 10, yP + 3);
			break;
		case 3:
			Rectangle(hdc, xP - 7, yP, xP + 3, yP + 10);
			break;
		}
	}
	else if (_spcial == true)
	{
		switch (_dir) {				
		case 0:
			Rectangle(hdc, xP, yP - 50, xP - 10, yP - 40);
			Rectangle(hdc, xP, yP - 7, xP - 10, yP + 3);
			Rectangle(hdc, xP, yP + 50, xP - 10, yP + 40);
			break;
		case 1:
			Rectangle(hdc, xP, yP - 50, xP - 10, yP - 40);
			Rectangle(hdc, xP, yP - 7, xP - 10, yP + 3);
			Rectangle(hdc, xP, yP + 50, xP - 10, yP + 40);
			break;
		case 2:
			Rectangle(hdc, xP - 50, yP, xP - 40, yP - 10);
			Rectangle(hdc, xP - 7, yP, xP + 3, yP - 10);
			Rectangle(hdc, xP + 50, yP, xP + 40, yP - 10);
			break;
		case 3:
			Rectangle(hdc, xP - 50, yP, xP - 40, yP - 10);
			Rectangle(hdc, xP - 7, yP, xP + 3, yP - 10);
			Rectangle(hdc, xP + 50, yP, xP + 40, yP - 10);
			break;
		}
	}
	if (i == 5)
		_spcial = false;

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldBrush2);
	DeleteObject(hBrush);
	DeleteObject(hBrush2);
	DeleteObject(hPen);
}

void Bullet::collision(Board board[][19], int hor, int ver, const RECT& playWindow)
{
	if (inbullet[index]._spcial == true)
	{
		bullRect = { _PosX - 50, _PosY - 50, _PosX + 50, _PosY + 50 };
	}
	else
	bullRect = { _PosX - 3, _PosY - 3, _PosX + 3, _PosY + 3 };

	RECT rect;

	for (int i = 0; i < hor; i++)
		for (int j = 0; j < ver; j++)
		{
			brect[i][j].left = board[i][j]._PosX = playWindow.left + (j * 50);
			brect[i][j].right = board[i][j]._PosX = playWindow.left + (j * 50) + 50;
			brect[i][j].top = board[i][j]._PosY = playWindow.top + (i * 50);
			brect[i][j].bottom = board[i][j]._PosY = playWindow.top + (i * 50) + 50;
		}

	if (bullRect.left < 0)
	{
		bullRect = { 0 ,};
	}

	for (int i = 0; i < hor; i++)
		for (int j = 0; j < ver; j++)
		{
			if (IntersectRect(&rect, &bullRect, &brect[i][j]) && board[i][j]._obstacle)
			{
				// 특수 총알은 장애물을 관통하지만 부시지는 못함
				if (inbullet[index]._spcial == false)
				{
					_PosY = -10;
					_PosX = -10;
				}
			}
			if (IntersectRect(&rect, &bullRect, &brect[i][j]) && board[i][j].isOn == false)
			{
				board[i][j].isOn = true;
			}
		}
}

void Bullet::enemycollision(Enemy* enemy[10], int hor, int ver, const RECT& playWindow, int* score, HWND hWnd, int* iscollision, SpecialBullet* spbullet[10], int* spbullet_index)
{
	RECT rect;


	if (inbullet[index]._spcial == true)
	{
		bullRect = { _PosX - 50, _PosY - 50, _PosX + 50, _PosY + 50 };
	}
	else
	bullRect = { _PosX - 3, _PosY - 3, _PosX + 3, _PosY + 3 };
	
	for (int i = 0; i < 10; i++)
	{
		if(enemy[i] != nullptr)
		enemyRect[i] = { enemy[i]->_PosX - 20, enemy[i]->_PosY - 20, enemy[i]->_PosX + 20, enemy[i]->_PosY + 20 };
	}

	for (int i = 0; i < 10; i++)
	{
		if(enemy[i] != nullptr && enemy[i]->_die == false )
		if (IntersectRect(&rect, &bullRect, &enemyRect[i]))
		{ 
			*iscollision = i;
			enemy[i]->_Slash = false;

			// 특수총알과 일반 총알 공격 나누기
			if (inbullet[index]._spcial == false)
			{
				_PosY = -10;
				_PosX = -10;
			}


			if (enemy[i] != nullptr)
				enemy[i]->_die = true;

				SetTimer(hWnd, 8, 10, NULL);

			
		}
	}
}
