#pragma once
#include "Fly.h"

class CSucker :
    public CFly
{
    GAMEOBJCLASS(CSucker, 22)
public:
    CSucker();
    virtual ~CSucker();

private:
    std::weak_ptr<class CTearShooter> mShooter;
    std::weak_ptr<CActor> mTarget;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Reset(bool HardReset = false) override;

    virtual void Dead() override;

};

