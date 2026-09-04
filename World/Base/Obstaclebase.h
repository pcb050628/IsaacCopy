#pragma once
#include "RoomMember.h"

class CObstaclebase :
    public CRoomMember
{
public:
    CObstaclebase(EObstacleType ObstacleType);
    CObstaclebase(const CObstaclebase& src);
    CObstaclebase(CObstaclebase&& src) noexcept;
    virtual ~CObstaclebase();

protected:
    const EObstacleType mObstacleType;

    std::weak_ptr<class CColliderBox2D> mHitBox;
    std::weak_ptr<class CMeshComponent> mMeshComp;
    std::weak_ptr<class CAnimation2DComponent> mAnimator;

    //상태를 명시해야할까 
    //어떤 상태들이 있지
    //파괴됨(돌, 해골
    //비활성화됨(불, 버튼)
    //
    //그건 상속받을 객체에서 알아서 하고

public:
    virtual bool Init();

    const EObstacleType GetObstacleType() const { return mObstacleType; }
    virtual bool GetHasInfo() { return false; }
    virtual FRoomMemberInfo GetInfo() { return FRoomMemberInfo(); }
    virtual void SetInfo(const FRoomMemberInfo& info) {}
};

