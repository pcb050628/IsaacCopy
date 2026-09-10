#pragma once
#include "Fly.h"

class CAttackFly :
    public CFly
{
    GAMEOBJCLASS(CAttackFly, 21)
public:
    CAttackFly();
    virtual ~CAttackFly();

private:
    std::weak_ptr<CActor> mTarget;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Reset(bool HardReset = false) override;

};

