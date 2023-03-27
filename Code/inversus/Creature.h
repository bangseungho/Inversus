#pragma once
#include<Windows.h>

enum CreatureType
{
	CT_PLAYER = 0,
	CT_ENEMY = 1,
};

class Creature
{
public:
	Creature(int PosX, int PosY, int creatureType) : _PosX(PosX), _PosY(PosY), _creatureType(creatureType)
	{

	}
	virtual ~Creature()
	{

	}
	
	void CreateMotion(HDC memDC);

public: 
	BOOL CreateFlag = false;
	int CreateSize = 50;
	int _PosX;
	int _PosY;
	BOOL CreateSizeCnt = false;
	int _creatureType;

};

