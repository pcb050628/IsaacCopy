#pragma once
#include "Unitbase.h"
#include "TimeManager.h"

class CMonster :
    public CUnitbase
{
public:
    CMonster();
    CMonster(const CMonster& src);
    CMonster(CMonster&& src) noexcept;
    virtual ~CMonster() = 0;

protected:
    float mMaxHP = 10.f;
    float mCurrentHP = mMaxHP;

    std::weak_ptr<class CSound> mMumblingSound;
    FTimerHandle mMumblingTimer;

public:
    virtual void Reset(bool HardReset = false);

    virtual void SetEnable(bool Enable) override;

    virtual void GetHit(std::weak_ptr<CGameObject> From);

protected:
    void SetMumblingSound(const std::string& soundName, float Time, bool loop = false);
    void SetMumblingSound(float Time, bool loop = false);
    void PlayerMumbling();

};