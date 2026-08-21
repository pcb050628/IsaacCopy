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

enum class EUnitEffect
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
	float Speed = 0.f;
	float Luck = 0.f;

	//Attack
	float Damage = 0.f;
	float ShotSpeed = 0.f;
	float Range = 0.f;
	float Height = 0.f;
	float knockback = 0.f;

	FUnitAttribute operator+(const FUnitAttribute& other)
	{
		FUnitAttribute ret;
		ret = *this;
		ret.ShotTerm += other.ShotTerm;
		ret.Speed += other.Speed;
		ret.Luck += other.Luck;

		ret.Damage += other.Damage;
		ret.ShotSpeed += other.ShotSpeed;
		ret.Range += other.Range;
		ret.Height += other.Height;
		ret.knockback += other.knockback;
		return ret;
	}

	FUnitAttribute operator-(const FUnitAttribute& other)
	{
		FUnitAttribute ret;
		ret = *this;
		ret.ShotTerm -= other.ShotTerm;
		ret.Speed -= other.Speed;
		ret.Luck -= other.Luck;

		ret.Damage -= other.Damage;
		ret.ShotSpeed -= other.ShotSpeed;
		ret.Range -= other.Range;
		ret.Height -= other.Height;
		ret.knockback -= other.knockback;
		return ret;
	}

	FUnitAttribute operator*(const FUnitAttribute& other)
	{
		FUnitAttribute ret;
		ret = *this;
		ret.ShotTerm *= other.ShotTerm;
		ret.Speed *= other.Speed;
		ret.Luck *= other.Luck;

		ret.Damage *= other.Damage;
		ret.ShotSpeed *= other.ShotSpeed;
		ret.Range *= other.Range;
		ret.Height *= other.Height;
		ret.knockback *= other.knockback;
		return ret;
	}
};

//공격시 효과들도 그냥 다 집어넣기
//독이나 석화나 화상이나 상관없음

//namespace ETearEffect
//{
//	enum Effect
//	{
//		None = 0,
//		Homing = 1,
//		Piercing = 2,
//		Spectral = 4,
//		Orbiting = 8,
//		Boomerang = 16,
//		Shield = 32,
//		Bouncing = 64,
//		Split = 128,
//	};
//};

struct FTearAttribute
{
	FVector2 Direction = FVector2::Zero;
	float Size = 10.f;
	float Damage = 0.f;
	float Speed = 0.f;
	float Range = 0.f;
	float Height = 0.f;
	float knockback = 10.f;

	bool Homing = false; //적 추적
	bool Piercing = false; //관통(유닛만)
	bool Spectral = false; //관통(장애물만)
	bool Orbiting = false; //특정 객체 공전
	bool Boomerang = false; //부메랑(절반 갔다가 캐릭터 방향으로 돌아옴)
	bool Shield = false; //눈물 파괴
	bool Bouncing = false; //튕김
	int SplitStack = 0; //분열 횟수
	int SplitCount = 0; //분열되는 갯수

	bool Sine = false; //사인파
	bool SquareWave = false; //직각
	bool Spiral = false; //나선
};

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

struct FPlayerHeartData
{
	EPlayerHeartType Type;
	EPlayerHeartState State;
};

namespace CharacterEffect
{
	enum LimitType
	{
		Room = 1,
		Time = 2,
	};
	enum EffectType
	{
		Attribute,
		Blasting,
		Fire,
		Spike,
		Knockback,
		Slow,
		Bleed,
		Flight,
	};
	struct FPlayerEffect
	{
		bool Temporary = true;
		int Range = LimitType::Room;
		int Type = EffectType::Attribute;

		int LimitCount = 1;
		int CurrentCount = 0;

		//0: 유닛 | 1: 눈물
		int attributeType = 0;
		FUnitAttribute unitAttribute;
		FTearAttribute tearAttribute;

		FPlayerEffect(int LimitMax)
			:LimitCount(LimitMax)
		{

		}

		FPlayerEffect(FPlayerEffect&& src) noexcept
			:LimitCount(src.LimitCount)
		{

		}
	};
};