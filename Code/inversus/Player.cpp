#include "Player.h"
#include "SpecialBullet.h"
#include "InBullet.h"
#include "Enemy.h"

#define SPBULLET_NUM 500
#define BULLET_NUM 6
#define ENEMY_NUM 10

extern SpecialBullet* spbullet[SPBULLET_NUM];
extern InBullet inbullet[BULLET_NUM];
extern int spbullet_index;
extern int index;

void Player::Paint(HDC memDC)
{
	SetROP2(memDC, R2_COPYPEN);
	HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);
	HPEN oldPen = (HPEN)SelectObject(memDC, hPen);

	RoundRect(memDC, _PosX - 20, _PosY - 20, _PosX + 20, _PosY + 20, 10, 10);

	SelectObject(memDC, oldBrush);
	SelectObject(memDC, oldPen);
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

void Player::SetPos(int level, Board board[][19], int ver, int hor, const RECT& playWindow)
{
	if (level == 1)
	{
		this->_PosX = 505;
		this->_PosY = 435;
	}
	if (level == 2)
	{
		this->_PosX = 505;
		this->_PosY = 385;
	}
	if (level == 3)
	{
		this->_PosX = 505;
		this->_PosY = 335;
	}

	for (int i = 0; i < hor; i++) // 처음으로 리셋
		for (int j = 0; j < ver; j++)
		{
			board[i][j].isOn = false;
		}

	for (int i = 0; i < hor; i++) // 보드 넘버 정의
		for (int j = 0; j < ver; j++)
		{
			board[i][j]._PosX = playWindow.left + (j * 50) + 25;
			board[i][j]._PosY = playWindow.top + (i * 50) + 25;
		}
}

void Player::startrange(HDC memDC, Board board[][19], int ver, int hor, const RECT& playWindow)
{
	for(int i = 0; i<hor; i++)
		for (int j = 0; j < ver; j++)
		{
			brect[i][j].left = board[i][j]._PosX = playWindow.left + (j * 50);
			brect[i][j].right = board[i][j]._PosX = playWindow.left + (j * 50) + 50;
			brect[i][j].top = board[i][j]._PosY = playWindow.top + (i * 50);
			brect[i][j].bottom = board[i][j]._PosY = playWindow.top + (i * 50) + 50;

			if (board[i][j]._PosY - 150 < _PosY && board[i][j]._PosY + 100 > _PosY &&
				board[i][j]._PosX - 150 < _PosX && board[i][j]._PosX + 100 > _PosX)
				board[i][j].isOn = true;
		}

	_ver = ver;
	_hor = hor;

	Ellipse(memDC, _PosX - 100, _PosY - 100, _PosX + 100, _PosY + 100);
}

void Player::moving(HDC hdc, HWND hWnd, Board board[][19], const RECT& playWindow, Enemy* enemy[])
{
	prect = { _PosX - 25, _PosY - 25, _PosX + 25, _PosY + 25 };
	RECT rect;
	for (int i = 0; i < _hor; i++)
		for (int j = 0; j < _ver; j++)
		{
			if (_PosX  < brect[i][j].right  && _PosX > brect[i][j].left &&
				_PosY < brect[i][j].bottom && _PosY > brect[i][j].top)
			{
				rectlize.x = j;
				rectlize.y = i;
			}
		}

	hdc = GetDC(hWnd);

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	// 렉트 확인 충돌 박스
	//Rectangle(hdc, brect[rectlize.y][rectlize.x].left, brect[rectlize.y][rectlize.x].top, 
	//	brect[rectlize.y][rectlize.x].right, brect[rectlize.y][rectlize.x].bottom);
	
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);



	switch (_dir)
	{
	case 0:
		if ((prect.left > playWindow.left && board[rectlize.y][rectlize.x - 1].isOn == true || prect.left + 5 > brect[rectlize.y][rectlize.x].left) 
			&& (board[rectlize.y][rectlize.x - 1]._obstacle == false || prect.left + 5 > brect[rectlize.y][rectlize.x].left))
			this->_PosX -= 4;
		break;
	case 1:
		if ((prect.right < playWindow.right && board[rectlize.y][rectlize.x + 1].isOn == true || prect.right - 5 < brect[rectlize.y][rectlize.x].right) 
			&& (board[rectlize.y][rectlize.x + 1]._obstacle == false || prect.right - 5 < brect[rectlize.y][rectlize.x].right))
			this->_PosX += 4;
		break;
	case 2:
		if ((prect.top > playWindow.top && board[rectlize.y - 1][rectlize.x].isOn == true || prect.top + 5 > brect[rectlize.y][rectlize.x].top) 
			&& (board[rectlize.y - 1][rectlize.x]._obstacle == false || prect.top + 5 > brect[rectlize.y][rectlize.x].top))
			this->_PosY -= 4;
		break;
	case 3:
		if ((prect.bottom < playWindow.bottom && board[rectlize.y + 1][rectlize.x].isOn == true || prect.bottom - 5< brect[rectlize.y][rectlize.x].bottom)
			&& (board[rectlize.y + 1][rectlize.x]._obstacle == false || prect.bottom - 5 < brect[rectlize.y][rectlize.x].bottom))
			this->_PosY += 4;
		break;
	}
	
	for (int i = 0; i < spbullet_index; i++)
	{
		if(spbullet[i] != nullptr)
		if (IntersectRect(&rect, &prect, &spbullet[i]->spbrect))
		{
				delete spbullet[i];
				spbullet[i] = nullptr;

				if(index < 6 )
				inbullet[index]._spcial = true;

				

				if (index > 5)
					index = 6;

				// 만약 총알이 꽉 차있는데 특수 총알 획득 시 일반 총알을 특수총알로 교체한다
				if (index == 6)
				{
					for (int i = 5; i >= 0; i--)
					{
						if (inbullet[i]._spcial == false)
						{
							inbullet[i]._spcial = true;
							return;
						}
					}
				}

				index++;

				if (index > 5)
				{
					index = 6;
				}

		}
	}

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i] != nullptr && _invincivleMode == false)
			if (IntersectRect(&rect, &prect, enemy[i]->enemyRect))
			{
				// life 감소
				player.life--;

				if (player.life < 0)
				{
					overcnt = true;
					KillTimer(hWnd, 6); 
					KillTimer(hWnd, 7);
					SetTimer(hWnd, 14, 50, NULL);
				}

				// 살아 생전 렉트 값을 보관한다
				_lastPos.x = _PosX;
				_lastPos.y = _PosY;

				// 플레이어 사망시 이펙트 시작 타이머 
				SetTimer(hWnd, 13, 10, NULL);

				// 적과 충돌시 맨 처음 태어나는 곳으로 이동
				_PosX = 505;
				_PosY = 435;

				combo = 0;

				CreateFlag = true;
				CreateSize = 50;
				CreateSizeCnt = false;

				_invincivleMode = true;

				die = true;

				// 충돌시 무적 시작 
				SetTimer(hWnd, 11, 1000, NULL);

				// 충돌시 화면 흔들림 효과
				SetTimer(hWnd, 14, 50, NULL);
			}
	}
}

void Player::DiePaint(HDC memDC, int hor, int ver, Board board[][19], const RECT& playWindow, int iscollision)
{
	RECT rect;
	HBRUSH hBrush = CreateSolidBrush(RGB(55, 160, 255));
	HBRUSH hBrush2 = CreateSolidBrush(RGB(65, 175, 255));
	HBRUSH hBrush3 = CreateSolidBrush(RGB(45, 145, 255));
	HPEN hPen = (HPEN)CreatePen(PS_NULL, 1, NULL);
	HPEN oldPen = (HPEN)SelectObject(memDC, hPen);

	// hBrush에 세개 다 때려 박으면 메모리 문제로 팅김
	HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
	Ellipse(memDC, _lastPos.x - _dieMotionSize, _lastPos.y - _dieMotionSize,
		_lastPos.x + _dieMotionSize2, _lastPos.y + _dieMotionSize2);

	HBRUSH oldBrush2 = (HBRUSH)SelectObject(memDC, hBrush2);
	Ellipse(memDC, _lastPos.x - _dieMotionSize2, _lastPos.y - _dieMotionSize2,
		_lastPos.x + _dieMotionSize3, _lastPos.y + _dieMotionSize3);

	HBRUSH oldBrush3 = (HBRUSH)SelectObject(memDC, hBrush3);
	Ellipse(memDC, _lastPos.x - _dieMotionSize3, _lastPos.y - _dieMotionSize3,
		_lastPos.x + _dieMotionSize, _lastPos.y + _dieMotionSize);

	SelectObject(memDC, oldPen);
	SelectObject(memDC, oldBrush3);
	DeleteObject(hBrush);
	DeleteObject(hBrush2);
	DeleteObject(hBrush3);
	DeleteObject(hPen);
}