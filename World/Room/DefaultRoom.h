#pragma once
#include "../Base/Roombase.h"

class CDefaultRoom :
    public CRoombase
{
    GAMECLASS(CDefaultRoom, 10)
public:
    CDefaultRoom();
    virtual ~CDefaultRoom();

public:
    virtual bool Init();
    virtual void Update(float DeltaTime); //클리어 여부 검사 / 방 마다 다르지만 기본으로는 모든 적이 비활성화 상태시 클리어
    virtual void Destroy();

protected:
    virtual void OnEnterRoom();
    virtual void WinRoom();
    virtual void OnExitRoom();
    virtual bool WinCheck();
};

