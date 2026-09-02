#pragma once
#include "../Base/Roombase.h"

class CTreasureRoom :
    public CRoombase
{
    GAMEOBJCLASS(CTreasureRoom, 11)
public:
    CTreasureRoom();
    virtual ~CTreasureRoom();

private:
    int mItemID = -1;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destroy();

protected:
    virtual void OnEnterRoom();
    virtual void WinRoom();
    virtual void OnExitRoom();
    virtual bool WinCheck();
};

