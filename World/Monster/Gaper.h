#pragma once
#include "Walker.h"

class CGaper :
    public CWalker
{
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

private:
    void MoveToTarget();
        
};

