#include<windows.h>
#include<tchar.h>
#include<time.h>
#include<atlimage.h>
#include"헤더.h"
#include"Player.h"
#include"Board.h"
#include"InBullet.h"
#include"Enemy.h"
#include"DieEmotion.h"
#include"SpecialBullet.h"
#include<atlimage.h>

#define WINDOWSIZE_X 1025
#define WINDOWSIZE_Y 825
#define MAX_INUM 13
#define MAX_JNUM 19
#define BOARDSIZE 50
#define BULLET_NUM 6
#define GRADIAN_NUM 6
#define ENEMY_NUM 10
#define SPBULLET_NUM 500

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습 5-4";
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void resetGame(int& gamespeed, BOOL& onTimer, BOOL& onTimer2, BOOL& onTimer3, BOOL& onTimer4, int hor, int ver,
	HDC memDC, const RECT& playWindow, Bullet bullet[], Enemy* enemy[], HWND hWnd);
enum Dir { left, right, up, down, LU, LD, RU, RD};
const double PI = 3.1415926;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	srand((unsigned int)time(nullptr));
	MSG Message;
	HWND hWnd;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		500, 200, WINDOWSIZE_X, WINDOWSIZE_Y, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

static Board iBoard[MAX_INUM][MAX_JNUM];
int spbullet_index = 0;
SpecialBullet* spbullet[SPBULLET_NUM];
InBullet inbullet[BULLET_NUM];
int score = 0;
int index = 6;
Player player;
static int stage;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	PAINTSTRUCT ps;
	static HDC memDC;
	static HDC hdc;
	static int z = 5;
	HBITMAP hBitmap;
	static RECT rectWindow = { 0, };
	static RECT playWindow = { 0, };
	static int ver;
	static int hor;
	static Enemy* enemy[ENEMY_NUM];
	static int enemy_index = 0;
	static Bullet bullet[BULLET_NUM];
	static Bullet gradian[GRADIAN_NUM][BULLET_NUM];
	static int bindex = 0;
	static int speedcnt = 0;
	static Board board;
	static int iscollision = 0;
	static int spbullet_num = 0;
	static int gamespeed = 1;
	static BOOL onTimer = false;
	static BOOL onTimer2 = false;
	static BOOL onTimer3 = false;
	static BOOL onTimer4 = false;
	static int invinciblecnt = 1;
	static int combocnt = 0;
	static int firstcombo = 0;
	static int lastcombo = 0;
	static CImage gameoverimg;
	static int retrycnt = 0;
	static HFONT hFont, oldFont; // 폰트 설정
	static HFONT hFont2, oldFont2; // 폰트 설정
	static HFONT hFont3, oldFont3; // 폰트 설정
	static HFONT hFont4, oldFont4; // 폰트 설정
	static HFONT hFont5, oldFont5; // 난이도 폰트 설정
	static CImage hart1;
	static CImage hart2;
	static CImage hart3;
	static CImage hart4;
	
	if (score >= 700)
		gamespeed = 2;
	if (score >= 1500)
		gamespeed = 3;
	if (score >= 3000)
		gamespeed = 4;

	if (gamespeed == 1 && onTimer == false)
	{
		SetTimer(hWnd, 6, 2000, NULL);
		onTimer = true;
	}
	else if (gamespeed == 2 && onTimer2 == false)
	{
		KillTimer(hWnd, 6);
		SetTimer(hWnd, 6, 1500, NULL);
		onTimer2 = true;
	}
	else if (gamespeed == 3 && onTimer3 == false)
	{
		KillTimer(hWnd, 6);
		SetTimer(hWnd, 6, 1200, NULL);
		onTimer3 = true;
	}
	else if (gamespeed == 4 && onTimer4 == false)
	{
		KillTimer(hWnd, 6);
		SetTimer(hWnd, 6, 800, NULL);
		onTimer4 = true;
	}

	switch (uMsg) {
	case WM_CREATE:
	{
		GetClientRect(hWnd, &rectWindow);
		// 배경 반투명 검정색 설치 오버게임시 
		gameoverimg.Load(L"gameoverimg.png");
		// 하트 이미지 로드
		hart1.Load(L"hart1.png");
		hart2.Load(L"hart2.png");
		hart3.Load(L"hart3.png");
		hart4.Load(L"hart4.png");

		playWindow = { 30, 110, 980, 760 }; // 첫 화면 난이도 1로 설정

		player.CreateFlag = true;
		ver = 19;
		hor = 13;

		// 보드 초기화
		for (int i = 0; i < hor; i++)
			for (int j = 0; j < ver; j++)
			{
				iBoard[i][j]._PosX = playWindow.left + (j * BOARDSIZE) + 25;
				iBoard[i][j]._PosY = playWindow.top + (i * BOARDSIZE) + 25;
			}

			// 처음 태어날 때 범위공격
			player.startrange(memDC, iBoard, ver, hor, playWindow);
			
			// 내부 돌아가는 총알 
			for (int i = 0; i < BULLET_NUM; i++)
			{
				inbullet[i].th += (double)i * 60;
			}
			SetTimer(hWnd, 4, inbullet->speed, NULL);

			for (int i = 0; i < ENEMY_NUM; i++)
			{
				enemy[i] = nullptr;
			}

			// 특수총알 초기화
			for (int i = 0; i < SPBULLET_NUM; i++)
			{
				spbullet[i] = nullptr;
			}

			// 총알 색 초기화
			for (int i = 0; i < 6; i++)
			{
				bullet[i].hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
			}

			stage = 1;
			resetGame(gamespeed, onTimer, onTimer2, onTimer3, onTimer4, hor, ver, memDC, playWindow, bullet, enemy, hWnd);

			// 키 입력 타이머
			SetTimer(hWnd, 1, 10, NULL);

			// 적 유도 타이머
			SetTimer(hWnd, 7, 20, NULL);

			// 특수총알 돌아가는 효과
			SetTimer(hWnd, 9, 100, NULL);

			// 생성 효과
			SetTimer(hWnd, 10, 10, NULL);

			// 게임 오버 retry 이미지 깜빡거리기
			SetTimer(hWnd, 15, 500, NULL);
	}
	break;
	case WM_KEYDOWN:
	{
		if (player.CreateFlag == false && player.overcnt == false)
		{
			// 난이도 설정
			if (wParam == '1')
			{
				stage = 1;
				playWindow = { 30, 110, 980, 760 }; // 블럭 한개당 50씩 가로 19개 세로 13개
				ver = 19;
				hor = 13;
				player.SetPos(1, iBoard, ver, hor, playWindow);
				player.startrange(memDC, iBoard, ver, hor, playWindow);
				resetGame(gamespeed, onTimer, onTimer2, onTimer3, onTimer4, hor, ver, memDC, playWindow, bullet, enemy, hWnd);

			}
			if (wParam == '2')
			{
				stage = 2;
				playWindow = { 30, 110, 980, 760 }; // 블럭 한개당 50씩 가로 19개 세로 13개
				ver = 19;
				hor = 13;
				player.SetPos(1, iBoard, ver, hor, playWindow);
				player.startrange(memDC, iBoard, ver, hor, playWindow);
				resetGame(gamespeed, onTimer, onTimer2, onTimer3, onTimer4, hor, ver, memDC, playWindow, bullet, enemy, hWnd);
			}
			if (wParam == '3')
			{
				stage = 3;
				playWindow = { 130, 110, 880, 660 }; // 블럭 한개당 50씩 가로 15개 세로 11개
				ver = 15;
				hor = 11;
				player.SetPos(2, iBoard, ver, hor, playWindow);
				player.startrange(memDC, iBoard, ver, hor, playWindow);
				resetGame(gamespeed, onTimer, onTimer2, onTimer3, onTimer4, hor, ver, memDC, playWindow, bullet, enemy, hWnd);
			}
			if (wParam == '4')
			{
				stage = 4;
				playWindow = { 230, 110, 780, 560 }; // 블럭 한개당 50씩 가로 11개 세로 7개
				ver = 11;
				hor = 9;
				player.SetPos(3, iBoard, ver, hor, playWindow);
				player.startrange(memDC, iBoard, ver, hor, playWindow);
				resetGame(gamespeed, onTimer, onTimer2, onTimer3, onTimer4, hor, ver, memDC, playWindow, bullet, enemy, hWnd);
			}
			if (wParam == 'r' || wParam == 'R')
			{
				SetTimer(hWnd, 3, 500, NULL);
			}
			if (wParam == 't' || wParam == 'T')
			{
				for (int i = 1; i <= 7; i)
					KillTimer(hWnd, i);
			}
			if (wParam == 'i' || wParam == 'I')
			{
				static int cnt;
				
				if (cnt == false)
				{
					player._invincivleMode = true;
					invinciblecnt = 1;
					cnt = true;
				}
				else
				{
					player._invincivleMode = false;
					invinciblecnt = 0;
					cnt = false;
				}
			}

			switch (wParam) {
			case VK_RETURN:

				inbullet[index]._spcial = false;

				// 총알 발사시 내부 원 빠르게 돌기
				SetTimer(hWnd, 5, 10, NULL);

				// 만약 정지에서 총알 발사시 장전하기
				if (player._dir == 10)
				{
					index++;
					if (index > 6)
						index = 6;
					break;

				}
				index--;
				if (index < 0) // 총알 모두 소진시 못쏘기
				{
					index = 0;
					break;
				}
				bullet[index]._dir = player._dir;
				bullet[index]._PosX = player._PosX;
				bullet[index]._PosY = player._PosY;
				bullet[index]._isOn = true;
				inbullet->speed = 10;
				// 총알 발사
				SetTimer(hWnd, 2, 10, NULL);

				break;
			}
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
	break;
	case WM_KEYUP:
	{
		// 쓰레기값을 넣어 키를 땠을 경우 못움직이게 고정 
		player._dir = 10;
		// 키 때고 마지막으로 갖고있던 값 저장
	}
		break;

		// 게임 다시 시작
	case WM_LBUTTONDOWN:
	{
		int mx = LOWORD(lParam);
		int my = HIWORD(lParam);

		// retry 이미지의 렉트값
		// retry.Draw(memDC, 450, 480, 110, 40, 0, 0, 320, 130) 

		if (player.life < 0)
		{
			if (mx < 560 && mx > 450 && my < 520 && my > 480)
			{
				// 게임 리셋 함수
				resetGame(gamespeed, onTimer, onTimer2, onTimer3, onTimer4, hor, ver, memDC, playWindow, bullet, enemy, hWnd);
			}
		}
	}
	InvalidateRect(hWnd, NULL, false);
	break;

	case WM_TIMER:
	{

		switch (wParam)
		{
			// GetAsynckeyState를 활용하여 키 중복 입력 허용
		case 1:
			if (player.CreateFlag == false && player.overcnt == false)
			{
				if (GetAsyncKeyState(0x41) & 0x8000)
				{
					player._dir = left;
				}
				if (GetAsyncKeyState(0x44) & 0x8000)
				{
					player._dir = right;

				}
				if (GetAsyncKeyState(0x57) & 0x8000)
				{
					// du 1more temp 
					player._dir = up;
				}
				if (GetAsyncKeyState(0x53) & 0x8000)
				{
					player._dir = down;
				}
				player.moving(memDC, hWnd, iBoard, playWindow, enemy);
			}
			break;

			// 총알 날아가는 타이머 설정
		case 2:

			for (int i = 0; i < 6; i++)
			{
				bullet[i].collision(iBoard, hor, ver, playWindow);
				bullet[i].enemycollision(enemy, hor, ver, playWindow, &score, hWnd, &iscollision, spbullet, &spbullet_index);
				if (bullet[i]._dir == 0)
				{
					if (bullet[i]._PosX - 15 > playWindow.left)
					{
						// 프레임당 20씩 설정
						bullet[i]._PosX -= 20;
					}
					else
					{
						// 벽이나 끝에 닿으면 널처리 하여 총알이 남아있는 버그 고려
						bullet[i]._isOn = false;
						bullet[i]._PosX = -1;
						bullet[i]._PosY = -1;
					}

					for (int j = 0; j < GRADIAN_NUM; j++)
					{
						gradian[i][j] = bullet[i];
						gradian[i][j]._PosX += 8 + 9 * j;
					}
				}

				if (bullet[i]._dir == 1)
				{
					if (bullet[i]._PosX + 15 < playWindow.right)
						bullet[i]._PosX += 20;
					else
					{
						bullet[i]._isOn = false;
						bullet[i]._PosX = -1;
						bullet[i]._PosY = -1;
					}

					for (int j = 0; j < GRADIAN_NUM; j++)
					{
						gradian[i][j] = bullet[i];
						gradian[i][j]._PosX -= 8 + 9 * j;
					}
				}

				if (bullet[i]._dir == 2)
				{
					if (bullet[i]._PosY - 15 > playWindow.top)
						bullet[i]._PosY -= 20;
					else
					{
						bullet[i]._isOn = false;
						bullet[i]._PosX = -1;
						bullet[i]._PosY = -1;
					}

					for (int j = 0; j < GRADIAN_NUM; j++)
					{
						gradian[i][j] = bullet[i];
						gradian[i][j]._PosY += 8 + 9 * j;
					}
				}

				if (bullet[i]._dir == 3)
				{
					if (bullet[i]._PosY + 15 < playWindow.bottom)
						bullet[i]._PosY += 20;
					else
					{
						bullet[i]._isOn = false;
						bullet[i]._PosX = -1;
						bullet[i]._PosY = -1;
					}

					for (int j = 0; j < GRADIAN_NUM; j++)
					{
						gradian[i][j] = bullet[i];

						gradian[i][j]._PosY -= 8 + 9 * j;
					}
				}
			}
			break;

			// 한번 장전에 최대 6발만 장전하도록 설정
		case 3:

			index++;
			bindex++;
			if (index >= 6 || bindex == 6)
			{
				bindex = 0;
				KillTimer(hWnd, 3);
			}
			break;

			// 6개의 총알이 내부에서 계속 돌아가도록 설정
		case 4:
		{
			for (int i = 0; i < 6; i++)
			{
				inbullet[i].th += 5;
				inbullet[i].Init(index);
			}
		}
		break;

		// 총알 쏘았을 시 빨리 돌아가도록 설정
		case 5:
		{
			speedcnt++;
			if (speedcnt == 20)
			{
				speedcnt = 0;
				KillTimer(hWnd, 5);
			}
			for (int i = 0; i < 6; i++)
			{
				inbullet[i].th += 10;
				inbullet[i].Init(index);
			}
		}
		break;

		// 적 최대 10마리로 만약 nullptr일 시 적 생성
		case 6:
		{
			onTimer = true;
			int enemynum = 0;

			for (int i = 0; i < ENEMY_NUM; i++)
			{
				if (enemy[i] == nullptr)
				{
					enemy[i] = new Enemy();
					enemy[i]->_die = false;
					enemy[i]->_exixt = true;
					enemy[i]->CreateFlag = true;
					enemynum++;

					// level에 따른 생성
					if (gamespeed == 1)
						break;
					if (gamespeed == 2 && enemynum > 1)
						break;
					if (gamespeed == 3 && enemynum > 2)
						break;
					if (gamespeed == 4 && enemynum > 3)
						break;

					
					else if (gamespeed == 2 && rand() % 2 == 1)
						break;
					else if (gamespeed == 3 && rand() % 2 == 1)
						break;
					else if (gamespeed == 4 && rand() % 2 == 1)
						break;
				}
			}
		}
		break;

		// 적이 플레이어를 따라다니도록 설정
		// 생성 효과 모두 끝난 후 움직이도록 설정
		case 7:
		{
			for (int i = 0; i < ENEMY_NUM; i++)
			{
				if (enemy[i] != nullptr && enemy[i]->CreateFlag == false)
				{
					enemy[i]->induce(player, hor, ver, iBoard, playWindow);
				}
			}
		}
		break;

		// 적 사망 이후 모션
		case 8:
		{
			for (int i = 0; i < ENEMY_NUM; i++)
			{
				if (enemy[i] != nullptr && enemy[i]->_die == true)
				{
					// 적 사망 이모션 크기 늘어나기
					if (enemy[i]->_dieMotionFlag == false)
					{
						enemy[i]->_dieMotionSize += 8;
						enemy[i]->_dieMotionSize2 += 8;

						// 연쇄 폭발
						if (enemy[i]->_dieMotionSize > 50 + rand() % 100)
						{
							enemy[i]->_dieMotionFlag = true;
							for (int j = 0; j < ENEMY_NUM; j++)
							{
								if (enemy[j] != nullptr)
								{
									// 처음 맞은 적을 제외
									if (j == iscollision)
										continue;
									// 적 사망시 세개의 폭발이 일어나며 각각에 맞을 시 적과 블럭 사망
									else if (IntersectRect(&rect, &enemy[i]->_DieMotion[0], &enemy[j]->enemyRect[j]) ||
										IntersectRect(&rect, &enemy[i]->_DieMotion[1], &enemy[j]->enemyRect[j]) ||
										IntersectRect(&rect, &enemy[i]->_DieMotion[2], &enemy[j]->enemyRect[j]))
										enemy[j]->_die = true;
								}
							}
						}
					}
					// 적 사망 이모션 크기 줄어들기
					if (enemy[i]->_dieMotionFlag == true)
					{
						enemy[i]->_dieMotionSize -= 8;
						enemy[i]->_dieMotionSize2 -= 8;

						if (enemy[i]->_dieMotionSize < 0)
						{
							delete enemy[i];
							enemy[i] = nullptr;

							// 5초안에 적 죽일 시 콤보 생성
							combocnt = 0;
							player.combo += 1;
							SetTimer(hWnd, 12, 1000, NULL);
						}

					}
				}

			}
		}
		break;

		// 특수총알 돌아가는 효과
		case 9:
		{
			for (int i = 0; i < SPBULLET_NUM; i++)
			{
				if (spbullet[i] != nullptr && spbullet[i]->_birth == true)
				{
					spbullet[i]->_th += 10;
					spbullet[i]->_PosX = spbullet[i]->_fixPosX + 5 * cos(spbullet[i]->_th * PI / 180);
					spbullet[i]->_PosY = spbullet[i]->_fixPosY + 5 * sin(spbullet[i]->_th * PI / 180);
				}
			}
		}

		// 적 생성 효과 이모션
		// 플레이어 생성 효과 이모션
		case 10:
		{
			for (int i = 0; i < ENEMY_NUM; i++)
			{
				if (enemy[i] != nullptr && enemy[i]->CreateFlag == true)
				{
					if (enemy[i]->CreateSize < 0)
						enemy[i]->CreateSizeCnt = true;

					if (enemy[i]->CreateSizeCnt == true)
					{
						enemy[i]->CreateSize += 1;

						if (enemy[i]->CreateSize == 15)
						{
							enemy[i]->CreateFlag = false;
						}
					}
					else
					{
						enemy[i]->CreateSize -= 1;
					}

				}
			}

			if (player.CreateSize < 0)
				player.CreateSizeCnt = true;
			if (player.CreateSizeCnt == true)
			{
				player.CreateSize += 1;

				if (player.CreateSize == 20)
				{
					// 화면 흔들림 효과 종료
					if(player.life >= 0)
						KillTimer(hWnd, 14);
					// 플레이어 죽고 다시 생성과 동시에 범위 공격과 5초 무적의 기간
					player.CreateFlag = false;
					player.startrange(memDC, iBoard, ver, hor, playWindow);
					SetTimer(hWnd, 8, 10, NULL);

					for (int j = 0; j < ENEMY_NUM; j++)
					{
						if (enemy[j] != nullptr)
						{
							RECT tempRect = { 405, 335, 605, 535 };

							if (IntersectRect(&rect, &tempRect, enemy[j]->enemyRect))
							{
								enemy[j]->_die = true;
							}
						}
					}
				}
			}
			else
			{
				player.CreateSize -= 1;
			}

		}
		break;

		// 플레이어 사망시 몇초간 무적 모드 
		case 11:
		{
			invinciblecnt++;

			if (invinciblecnt == 5)
			{
				invinciblecnt = 0;
				player._invincivleMode = false;
				KillTimer(hWnd, 11);
			}

		}
		break; 

		// 콤보 타이머 설정
		case 12:
		{
			// 적을 처치시 콤보 타이머가 시작되고 적을 처치할 때마다 
			// 콤보 카운트가 계속 초기화 되며 만약 적을 죽이지 않으면 1초에 한번씩 콤보 횟수 증가
			// 콤보 카운트가 5일 때(적을 죽이지 않고 5초가 지났을 때) 콤보 삭제
			combocnt++;

			if (combocnt == 5)
			{
				combocnt = 0;
				player.combo = 0;
			}
		}
		break;

		// 플레이어 사망시 이펙트
		case 13:
		{
			if (player._dieMotionFlag == false)
			{
				player._dieMotionSize += 5;
				player._dieMotionSize2 += 5;
			}

			// 연쇄 폭발
			if (player._dieMotionSize > 50 + rand() % 100)
			{
				player._dieMotionFlag = true;
			}
			// 이모션 크기 줄어들기
			if (player._dieMotionFlag == true)
			{
				player._dieMotionSize -= 8;
				player._dieMotionSize2 -= 8;
				if (player._dieMotionSize < 0)
				{
					KillTimer(hWnd, 13);
					player._dieMotionSize = -30 + rand() % 60;
					player._dieMotionSize2 = -30 + rand() % 60;
					player._dieMotionSize3 = -30 + rand() % 60;
					player.die = false;
					player._dieMotionFlag = false;
				}
			}
		}
		break;

		// 충돌시 화면 흔들림 효과 타이머
		case 14:
		{
			static BOOL shakeWindow;
			
			if (shakeWindow == true)
			{
				playWindow.left -= 2;
				playWindow.right -= 2;
				shakeWindow = false;
			}
			else
			{
				playWindow.left += 2;
				playWindow.right += 2;
				shakeWindow = true;
			}
		}
			break;

		case 15:
		{
			retrycnt++;
		}
		break;
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
		break;


	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		memDC = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, rectWindow.right, rectWindow.bottom);
		SelectObject(memDC, hBitmap);

		// 화면 뒤 회색 배경 부분 출력
		board.rectScene(memDC, rectWindow);

		// 화면의 실제 플레이 부분 출력
		board.playScene(memDC, playWindow);



		// 검은색의 벽돌 출력
		for (int i = 0; i < hor; i++)
		{
			for (int j = 0; j < ver; j++)
			{
				iBoard[i][j].isOffScene(memDC, playWindow, ver, hor, BOARDSIZE, i, j);
			}
		}

		// 적 출력
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy[i] != nullptr && enemy[i]->_die == false && enemy[i]->CreateFlag == false)
			{
				// 적의 빗금 영역 출력
				enemy[i]->tileSlash(memDC, hor, ver);
			}
		}

		// 그리드 출력
		board.gridScene(memDC, playWindow, ver, hor, BOARDSIZE);

		// 적 생성 효과
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy[i] != nullptr && enemy[i]->CreateFlag == true)
			{
				// 적 생성 효과 함수 출력
				enemy[i]->CreateMotion(memDC);
			}
		}

		// 적 출력
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			// 적 생성 효과가 끝나면 출력
			if (enemy[i] != nullptr && enemy[i]->_die == false && enemy[i]->CreateFlag == false)
			{
				// 적 출력
				enemy[i]->Paint(memDC, hWnd);
			}
		}
		
		// 적 사망 효과
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy[i] != nullptr && enemy[i]->_die == true)
			{
				enemy[i]->DiePaint(memDC, hor, ver, iBoard, playWindow, iscollision);

			}
		}

		// 총알 출력
		for (int i = 0; i < 6; i++)
		{
			if (bullet[i]._isOn == true)
			{
				bullet[i].Paint(memDC, bullet[i]._PosX, bullet[i]._PosY, player._dir);
			}
		}
		
		// 총알 그라디언 효과
		for (int j = 0; j < GRADIAN_NUM; j++)
		{
			for (int i = 0; i < 6; i++)
			{
				if (gradian[j][i]._isOn == true)
				{
					
					gradian[j][i].gPaint(memDC, gradian[j][i]._PosX, gradian[j][i]._PosY, gradian[j][i]._dir, i);
				}
			}
		}

		// 특수 총알 생성
		for (int i = 0; i < SPBULLET_NUM; i++)
		{

			if (spbullet[i] != nullptr)
			{
				spbullet[i]->Paint(memDC);
			}
		}


		// 플레이어 생성 효과
		if(player.CreateFlag == true)
			player.CreateMotion(memDC);

		// 플레이어 사망 터짐효과
		if(player.die == true)
			player.DiePaint(memDC, hor, ver, iBoard, playWindow, iscollision);

		if (player._invincivleMode == true && invinciblecnt != 0)
		{
			// 5초가 흐를수록 색이 연해지면서 무적이 해제됨
			HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
			HPEN hPen = CreatePen(PS_SOLID, 5, RGB(50 + (43 * invinciblecnt), 150 + (25 * invinciblecnt), 255));
			HPEN oldPen = (HPEN)SelectObject(memDC, hPen);


			RoundRect(memDC, player._PosX - 25, player._PosY - 25, player._PosX + 25, player._PosY + 25, 10, 10);
			SelectObject(memDC, oldBrush);
			SelectObject(memDC, oldPen);
			DeleteObject(hBrush);
			DeleteObject(hPen);
		}

		// 플레이어 생성효과가 끝나면 플레이어 출력
		if (player.CreateFlag == false)
			player.Paint(memDC);

		// 내부 돌아가는 총알 출력
		for (int i = 0; i < index; i++)
		{
			if (player.CreateFlag == false)
				inbullet[i].Paint(memDC, player, index);
		}

		// 점수판 하얀색으로 설정
		board.scoreScene(memDC, rectWindow);

		TCHAR lpOut[10];
		TCHAR level[10];
		TCHAR combo[10];

		// 스코어판 폰트 입력
		hFont = CreateFont(24, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ARCADECLASSIC"));
		oldFont = (HFONT)SelectObject(memDC, hFont);

		// 스코어 출력
		TextOut(memDC, 40, 50, L"SCORE", 5);
		wsprintf(lpOut, L"%d", score);
		TextOut(memDC, 118, 50, lpOut, lstrlen(lpOut));

		// 콤보 출력 76
		TextOut(memDC, 190, 50, L"COMBO", 5);
		wsprintf(combo, L"%d", player.combo);
		TextOut(memDC, 266, 50, combo, lstrlen(combo));

		// 게임 레벨 출력 75
		TextOut(memDC, 340, 50, L"LEVEL", 5);
		wsprintf(level, L"%d", gamespeed);
		TextOut(memDC, 415, 50, level, lstrlen(level));

		// life 출력 65
		TextOut(memDC, 490, 50, L"life", 4);

		// 플레이어 목숨 하트 출력
		if (player.life == 3)
		{
			hart1.Draw(memDC, 560, 45, 40, 34, 0, 0, 265, 230);
		}
		else if (player.life == 2)
		{
			hart2.Draw(memDC, 560, 45, 40, 34, 0, 0, 265, 230);
		}
		else if (player.life == 1)
		{
			hart3.Draw(memDC, 560, 45, 40, 34, 0, 0, 265, 230);
		}
		else if (player.life == 0)
		{
			hart4.Draw(memDC, 560, 45, 40, 34, 0, 0, 265, 230);
		}

		// 게임오버 화면 스코어 점수 폰트
		hFont2 = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ARCADECLASSIC"));

		// retry 폰트
		hFont3 = CreateFont(30, 0, 0, 0, 100, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ARCADECLASSIC"));
		
		// gameover 폰트
		hFont4 = CreateFont(150, 0, 0, 0, 100, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ARCADECLASSIC"));
		
		// 난이도 폰트
		hFont5 = CreateFont(70, 0, 0, 0, 100, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("ARCADECLASSIC"));

		// 난이도 출력
		if (stage == 1)
		{
			SetTextColor(memDC, RGB(200, 0, 0));
			TextOut(memDC, 40, 15, L"VERY HARD", 9);
		}
		if (stage == 2)
		{
			SetTextColor(memDC, RGB(220, 100, 0));
			TextOut(memDC, 40, 15, L"HARD", 4);
		}
		if (stage == 3)
		{
			SetTextColor(memDC, RGB(100, 200, 0));
			TextOut(memDC, 40, 15, L"NORMAL", 6);
		}
		if (stage == 4)
		{
			SetTextColor(memDC, RGB(0, 150, 200));
			TextOut(memDC, 40, 15, L"EASY", 4);
		}
		// 글자 배경색 투명

		SetBkMode(memDC, TRANSPARENT);
		SetTextColor(memDC, RGB(255, 255, 255));

		if (player.life < 0)
		{
			// gameover시 뒷 배경 반투명 출력
			gameoverimg.Draw(memDC, rectWindow);
			
			// gameover 출력
			oldFont4 = (HFONT)SelectObject(memDC, hFont4);
			TextOut(memDC, playWindow.left + 300, 240, L"GAME", 4);
			TextOut(memDC, playWindow.left + 300, 340, L"OVER", 4);

			// retry 출력
			oldFont3 = (HFONT)SelectObject(memDC, hFont3);
			if(retrycnt % 3 != 2)
			TextOut(memDC, 430, 480, L"▶   RETRY   ◀", 13);

			int pontlocationX = 380;
			if (score > 100)
				pontlocationX = 365;
			if (score > 1000)
				pontlocationX = 350;
			if (score > 10000)
				pontlocationX = 330;

			// score 출력
			oldFont2 = (HFONT)SelectObject(memDC, hFont2);
			TextOut(memDC, pontlocationX, 560, L"SCORE :", 7);
			wsprintf(lpOut, L"%d", score);
			TextOut(memDC, pontlocationX + 170, 560, lpOut, lstrlen(lpOut));
		}

		BitBlt(hdc, 0, 0, rectWindow.right, rectWindow.bottom, memDC, 0, 0, SRCCOPY);

		// 메모리 해제
		for (int j = 0; j < GRADIAN_NUM; j++)
		{
			for (int i = 0; i < 6; i++)
			{
				DeleteObject(bullet[i].hBrush);
			}
		}


		SelectObject(memDC, oldFont);
		SelectObject(memDC, oldFont2);
		SelectObject(memDC, oldFont3);
		SelectObject(memDC, oldFont4);
		SelectObject(memDC, oldFont5);
		DeleteObject(hFont);
		DeleteObject(hFont2);
		DeleteObject(hFont3);
		DeleteObject(hFont4);
		DeleteObject(hFont5);
		DeleteDC(memDC);
		DeleteObject(hBitmap);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		gameoverimg.Destroy();
		hart1.Destroy();
		hart2.Destroy();
		hart3.Destroy();
		hart4.Destroy();
	}
	break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void resetGame(int& gamespeed, BOOL& onTimer, BOOL& onTimer2, BOOL& onTimer3, BOOL& onTimer4, int hor, int ver, 
				HDC memDC, const RECT& playWindow,Bullet bullet[], Enemy* enemy[], HWND hWnd)
{
	// 플레이어 생명 초기화
	player.life = 3;

	// 게임 스피드 초기화
	gamespeed = 1;
	onTimer = false;
	onTimer2 = false;
	onTimer3 = false;
	onTimer4 = false;

	// 보드 초기화
	for (int i = 0; i < hor; i++)
		for (int j = 0; j < ver; j++)
		{
			iBoard[i][j].isOn = false;
			iBoard[i][j]._obstacle = false;
		}

	// very hard 모드일 경우 장애물 설치
	if (stage == 1)
	{
		// 좌상
		iBoard[2][5]._obstacle = true;
		iBoard[2][6]._obstacle = true;
		iBoard[2][7]._obstacle = true;
		iBoard[3][5]._obstacle = true;
		iBoard[4][5]._obstacle = true;

		// 우상
		iBoard[2][11]._obstacle = true;
		iBoard[2][12]._obstacle = true;
		iBoard[2][13]._obstacle = true;
		iBoard[3][13]._obstacle = true;
		iBoard[4][13]._obstacle = true;

		// 좌하
		iBoard[10][5]._obstacle = true;
		iBoard[10][6]._obstacle = true;
		iBoard[10][7]._obstacle = true;
		iBoard[8][5]._obstacle = true;
		iBoard[9][5]._obstacle = true;

		// 우하
		iBoard[10][11]._obstacle = true;
		iBoard[10][12]._obstacle = true;
		iBoard[10][13]._obstacle = true;
		iBoard[8][13]._obstacle = true;
		iBoard[9][13]._obstacle = true;
	}



	// 플레이어 생성시 범위 공격
	player.startrange(memDC, iBoard, ver, hor, playWindow);

	// 특수총알 초기화
	spbullet_index = 0;

	// 모든 특수 총알 초기화
	for (int i = 0; i < BULLET_NUM; i++)
	{
		inbullet[i]._spcial = false;
		bullet[i]._spcial = false;
	}

	// 총알 초기화
	index = 6;

	// 적 초기화
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		delete enemy[i];
		enemy[i] = nullptr;
	}

	for (int i = 0; i < SPBULLET_NUM; i++)
	{
		delete spbullet[i];
		spbullet[i] = nullptr;
	}

	KillTimer(hWnd, 14);

	// 점수 초기화
	score = 0;

	// 콤보 초기화
	player.combo = 0;

	// 플레이어 움직일 수 있게 하기
	player.overcnt = false;

	// 적 유도 타이머 살리기
	SetTimer(hWnd, 7, 20, NULL);
}