#pragma once
#include "../Base/Roombase.h"

class CShopRoom :
    public CRoombase
{
    GAMEOBJCLASS(CShopRoom, 12)
public:
    CShopRoom();
    virtual ~CShopRoom();

private:
    bool mbFlag = false;
    std::vector<std::pair<bool, int>> mSlots;
    std::vector<std::weak_ptr<class CRoomMember>> mSlotObstacles;

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

