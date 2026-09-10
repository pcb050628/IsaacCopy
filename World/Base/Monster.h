#pragma once

class CMonster
{
protected:
    float mMaxHP = 10.f;
    float mCurrentHP = mMaxHP;

public:
    virtual void Dead() = 0;

};