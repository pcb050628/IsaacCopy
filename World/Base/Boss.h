#pragma once
#include "RoomMember.h"

class CBoss :
    public CRoomMember
{
public:
    CBoss();
    virtual ~CBoss() = 0;

protected:
    //변수를 딱히 공유할게 없음
    bool mbIsDead = false;

public:
    virtual void GetHit(std::weak_ptr<CGameObject> From) = 0;
    const bool GetIsDead() const { return mbIsDead; }

};