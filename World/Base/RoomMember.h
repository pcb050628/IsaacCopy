#pragma once
#include "GameObject.h"

class CRoomMember :
    public CGameObject
{
public:
    CRoomMember(EObjectType Type);
    CRoomMember(const CRoomMember& src);
    CRoomMember(CRoomMember&& src) noexcept;
    virtual ~CRoomMember() = 0;

protected:
    std::weak_ptr<class CRoombase> mRoomOwner;

public:
    void SetRoom(std::weak_ptr<CRoombase> Room) { mRoomOwner = Room; }
    std::weak_ptr<CRoombase> UnsetRoom()
    {
        std::weak_ptr<CRoombase> rRoom = mRoomOwner;
        mRoomOwner.reset(); return rRoom;
    }
};

