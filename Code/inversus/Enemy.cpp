#include "Enemy.h"
#include "SpecialBullet.h"
#include "Player.h"

Enemy::Enemy() : Creature((50 + rand() % 900), (100 + rand() % 600), CT_ENEMY), _die(false), _exixt(false),
_dieMotionSize(-30 + rand() % 60), _dieMotionSize2(-30 + rand() % 60), _dieMotionSize3(-30 + rand() % 60), _dieMotionFlag(false)
{
	// 플레이어와 적 사이의 거리가 200이하이면 적을 재생성
	if (abs(player._PosX - _PosX) < 100 && abs(player._PosY - _PosY) < 100)
	{
		_delete = true;
		delete this;
		new Enemy();
	}
}

Enemy::~Enemy()
{

	if (_delete == false)
	{
		dethcnt++;

		if (rand() % 3 == 1)
		{

			spbullet[spbullet_index] = new SpecialBullet();
			spbullet[spbullet_index]->_PosX = _PosX;
			spbullet[spbullet_index]->_PosY = _PosY;
			spbullet[spbullet_index]->_fixPosX = _PosX;
			spbullet[spbullet_index]->_fixPosY = _PosY;
			spbullet[spbullet_index]->_birth = true;

			spbullet_index += 1;
		}

		//적 충돌시 스코어 랜덤값으로 올리기
		score += 20 + rand() % 40;
	}
}

void Enemy::Paint(HDC hdc, HWND hWnd)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(200, 100, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	HPEN hPen = (HPEN)CreatePen(PS_NULL, 1, NULL);
	HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

	RoundRect(hdc, _PosX - 15, _PosY - 15, _PosX + 15, _PosY + 15, 5, 5);

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

void Enemy::induce(const Player& player, int hor, int ver, Board board[][19], const RECT& playWindow)
{
	RECT rect;

	if (_PosX < player._PosX)
		_PosX += 1 + rand() % 3;
	if (_PosX > player._PosX)
		_PosX -= 1 + rand() % 3;
	if (_PosY < player._PosY)
		_PosY += 1 + rand() % 3;
	if (_PosY > player._PosY)
		_PosY -= 1 + rand() % 3;

	for (int i = 0; i < hor; i++)
		for (int j = 0; j < ver; j++)
		{
			brect[i][j].left = board[i][j]._PosX = playWindow.left + (j * 50);
			brect[i][j].right = board[i][j]._PosX = playWindow.left + (j * 50) + 50;
			brect[i][j].top = board[i][j]._PosY = playWindow.top + (i * 50);
			brect[i][j].bottom = board[i][j]._PosY = playWindow.top + (i * 50) + 50;
		}

	for (int i = 0; i < 10; i++)
	{
		if (this != nullptr)
			enemyRect[i] = { _PosX - 1, _PosY - 1, _PosX + 1, _PosY + 1 };
	}

	for (int i = 0; i < hor; i++)
		for (int j = 0; j < ver; j++)
		{
			for(int k = 0; k < 10; k++)
			if (IntersectRect(&rect, &enemyRect[k], &brect[i][j]))
			{
				board[i][j].isOn = false;
			}
		}

}

void Enemy::DiePaint(HDC memDC, int hor, int ver, Board board[][19], const RECT& playWindow, int iscollision)
{
	RECT rect;

	HBRUSH hBrush = CreateSolidBrush(RGB(200, 90, 0));
	HBRUSH hBrush2 = CreateSolidBrush(RGB(190, 80, 0));
	HBRUSH hBrush3 = CreateSolidBrush(RGB(210, 110, 0));
	HPEN hPen = (HPEN)CreatePen(PS_NULL, 1, NULL);
	HPEN oldPen = (HPEN)SelectObject(memDC, hPen);

	// hBrush에 세개 다 때려 박으면 메모리 문제로 팅김
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	Ellipse(memDC, _PosX - _dieMotionSize, _PosY - _dieMotionSize,
		_PosX + _dieMotionSize2, _PosY + _dieMotionSize2);

	HBRUSH oldBrush2 = (HBRUSH)SelectObject(memDC, hBrush2);
	Ellipse(memDC, _PosX - _dieMotionSize2, _PosY - _dieMotionSize2,
		_PosX + _dieMotionSize3, _PosY + _dieMotionSize3);

	HBRUSH oldBrush3 = (HBRUSH)SelectObject(memDC, hBrush3);
	Ellipse(memDC, _PosX - _dieMotionSize3, _PosY - _dieMotionSize3,
		_PosX + _dieMotionSize, _PosY + _dieMotionSize);

	_DieMotion[0] = { _PosX - _dieMotionSize, _PosY - _dieMotionSize, _PosX + _dieMotionSize2, _PosY + _dieMotionSize2 };
	_DieMotion[1] = { _PosX - _dieMotionSize2, _PosY - _dieMotionSize2, _PosX + _dieMotionSize3, _PosY + _dieMotionSize3 };
	_DieMotion[2] = { _PosX - _dieMotionSize3, _PosY - _dieMotionSize3, _PosX + _dieMotionSize, _PosY + _dieMotionSize };

	for (int i = 0; i < hor; i++)
		for (int j = 0; j < ver; j++)
		{
			brect[i][j].left = board[i][j]._PosX = playWindow.left + (j * 50);
			brect[i][j].right = board[i][j]._PosX = playWindow.left + (j * 50) + 50;
			brect[i][j].top = board[i][j]._PosY = playWindow.top + (i * 50);
			brect[i][j].bottom = board[i][j]._PosY = playWindow.top + (i * 50) + 50;
		}

	for (int i = 0; i < hor; i++)
		for (int j = 0; j < ver; j++)
		{
			if (IntersectRect(&rect, &_DieMotion[0], &brect[i][j]) && board[i][j].isOn == false)
			{
				board[i][j].isOn = true;
			}
			if (IntersectRect(&rect, &_DieMotion[1], &brect[i][j]) && board[i][j].isOn == false)
			{
				board[i][j].isOn = true;
			}
			if (IntersectRect(&rect, &_DieMotion[2], &brect[i][j]) && board[i][j].isOn == false)
			{
				board[i][j].isOn = true;
			}
		}

	SelectObject(memDC, oldPen);
	SelectObject(memDC, oldBrush3);
	DeleteObject(hBrush);
	DeleteObject(hBrush2);
	DeleteObject(hBrush3);
	DeleteObject(hPen);
}

void Enemy::DiePaint2(HDC memDC, int hor, int ver, Board board[][19], const RECT& playWindow, int iscollision, HWND hWnd)
{
	RECT rect;

	_DieMotion[0] = { _PosX - _dieMotionSize, _PosY - _dieMotionSize, _PosX + _dieMotionSize2, _PosY + _dieMotionSize2 };
	_DieMotion[1] = { _PosX - _dieMotionSize2, _PosY - _dieMotionSize2, _PosX + _dieMotionSize3, _PosY + _dieMotionSize3 };
	_DieMotion[2] = { _PosX - _dieMotionSize3, _PosY - _dieMotionSize3, _PosX + _dieMotionSize, _PosY + _dieMotionSize };

	if (IntersectRect(&rect, &_DieMotion[0], enemyRect))
	{
		if (this != nullptr)
			this->_die = true;

		SetTimer(hWnd, 8, 10, NULL);
	}


}

void Enemy::tileSlash(HDC memDC, int hor, int ver)
{
	RECT rect;

	RECT temp_enemyRect = { 0, }; // 임시 렉트 형성 -> 현재 렉트의 확장 부분 저장
	static RECT temp_brect = { 0, }; // 임시 렉트 형성 -> 현재 보드의 축소 부분 저장

	temp_enemyRect = { _PosX - ADD8TILE, _PosY - ADD8TILE, _PosX + ADD8TILE, _PosY + ADD8TILE };

	HBRUSH hBrush = CreateSolidBrush(RGB(200, 100, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(200, 100, 0));
	HPEN oldPen = (HPEN)SelectObject(memDC, hPen);

	// 빗금 그리기
	for (int i = 0; i < hor; i++)
	{
		for(int j = 0; j<ver; j++)
		{
			if (IntersectRect(&rect, &temp_enemyRect, &brect[i][j]))
			{
				temp_brect = { brect[i][j].left + 1, brect[i][j].top + 1, brect[i][j].right - 1, brect[i][j].bottom - 1 };

				/*
				[빗금1]
				for (int i = 0; i < 7; i++)
				{
					MoveToEx(memDC, temp_brect.left, temp_brect.top + 8 * i, NULL);
					LineTo(memDC, temp_brect.left + 8 * i, temp_brect.top);
				}

				for (int i = 0; i < 6; i++)
				{
					MoveToEx(memDC, temp_brect.left + 8 * i, temp_brect.bottom, NULL);
					LineTo(memDC, temp_brect.right, temp_brect.top + 8 * i);
				}
				*/
				FrameRect(memDC, &temp_brect, hBrush);
			}
		}
	}
	
	// [빗금2]
	// [빗금1]처럼 내부에다가 렉트 하나씩 그리게 되면 메모리 낭비가 심함 따라서 전체 부분에 한번에 그리기

	for (int i = 0; i < 19; i++)
	{
		MoveToEx(memDC, temp_brect.left - 100, temp_brect.top + 8 * i - 100, NULL);
		LineTo(memDC, temp_brect.left + 8 * i - 100, temp_brect.top - 100);
	}
	for (int i = -12; i < 6; i++)
	{
		MoveToEx(memDC, temp_brect.left + 8 * i, temp_brect.bottom, NULL);
		LineTo(memDC, temp_brect.right , temp_brect.top + 8 * i);
	}

	SelectObject(memDC, oldBrush);
	SelectObject(memDC, oldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}
