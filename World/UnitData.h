#pragma once
#include "EngineInfo.h"

enum class EUnitImmune
{
	Blasting,
	Fire,
	Spike,
	Knockback,
	End,
};

enum class EUnitStatus
{
	Fear,
	Charm,
	Burn,
	Confusion,
	Petrification, //석화
	Poision,
	Slow,
	Bleed,
	Frozen,
	End,
};

struct FUnitAttribute
{
	//Unit
	float ShotTerm = 0.f;
	float Speed = 10.f;
	float Luck = 0.f;
	
	//Attack
	float Damage = 0.f;
	float ShotSpeed = 0.f;
	float Range = 0.f;
	float Height = 0.f;
	float knockback = 10.f;
};
//공격시 효과들도 그냥 다 집어넣기
//독이나 석화나 화상이나 상관없음

enum class EPlayerHeartType
{
	Red,
	Blue,
	Black,
	White,
	Skull,
	Rotten,
	End,
};

enum class EPlayerHeartState
{
	Empty,
	Half,
	Full,
};