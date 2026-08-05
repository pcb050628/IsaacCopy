#pragma once
#include "Walker.h"

class CGaper :
    public CWalker
{
    GAMECLASS(CGaper, 20);
public:
    CGaper();
    CGaper(const CGaper& src);
    CGaper(CGaper&& src) noexcept;
    virtual ~CGaper();

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destory();
    virtual void Dead();

    virtual void Reset(bool HardReset = false);

private:
    void MoveToTarget();
        
};

