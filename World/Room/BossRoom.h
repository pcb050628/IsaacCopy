#pragma once
#include "../Base/Roombase.h"

class CBossRoom :
    public CRoombase
{
    GAMEOBJCLASS(CBossRoom, 13)
public:
    CBossRoom();
    virtual ~CBossRoom();

private:
    int mBossID = -1;
    int mRewardID = -1;
    std::weak_ptr<class CBoss> mTargetBoss;

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

