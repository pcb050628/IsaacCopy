#pragma once
#include "Unitbase.h"

class CMonster :
    public CUnitbase
{
public:
    CMonster();
    CMonster(const CMonster& src);
    CMonster(CMonster&& src) noexcept;
    virtual ~CMonster() = 0;

protected:
    float MaxHP = 10.f;
    float CurrentHP = MaxHP;

protected:
    //플레이어에게 가는 길찾기 / 

};

