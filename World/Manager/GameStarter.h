#pragma once
#include "EngineInfo.h"

class CGameStarter
{
public:
	static bool Start();
private:
	static bool LoadSound();
	static bool LoadTexture();
};

