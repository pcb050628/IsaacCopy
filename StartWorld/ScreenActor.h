#pragma once
#include "World\Actor.h"

class CScreenActor :
    public CActor
{
public:
    CScreenActor();
    virtual ~CScreenActor() = 0;

public:
    virtual int OnUp() = 0;
    virtual int OnDown() = 0;
    virtual int OnRight() = 0;
    virtual int OnLeft() = 0;
    virtual int OnSubmit() = 0;
    virtual int OnEscape() = 0;
};

