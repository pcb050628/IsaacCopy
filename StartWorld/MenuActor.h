#pragma once
#include "World\Actor.h"

class CMenuActor :
    public CActor
{
public:
    CMenuActor();
    virtual ~CMenuActor();

private:

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
};

