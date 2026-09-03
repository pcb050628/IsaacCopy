#pragma once
#include "Base/RoomMember.h"
#include "../GameObjectStructs.h"

class CDoor :
    public CRoomMember
{
    GAMEOBJCLASS(CDoor, 4000)
public:
    CDoor();
    CDoor(const CDoor& src);
    CDoor(CDoor&& src) noexcept;
    virtual ~CDoor();

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destroy();

    virtual void Reset(bool HardReset = false);
protected:
    std::weak_ptr<class CColliderBox2D> mBoxColComp;
    std::weak_ptr<class CSpriteComponent> mFrameRenderer;  //문틀
    std::weak_ptr<class CSpriteComponent> mLayerRenderer1; //그림자
    std::weak_ptr<class CSpriteComponent> mLayerRenderer2; //문짝
    std::weak_ptr<class CSpriteComponent> mLayerRenderer3; //문짝
    
    std::weak_ptr<class CRigidBodyComponent> mLayer2Rigid;
    std::weak_ptr<class CRigidBodyComponent> mLayer3Rigid;

    std::weak_ptr<class CSoundComponent> mSound;

    FOpenInfo mInfo;
    FVector2 mDirection;
    bool mbIsOpen = false;

    std::weak_ptr<CRoomMember> mPlayer; //? 이거 왜 들고 있지

    std::weak_ptr<class CSound> mOpenSound;
    std::weak_ptr<class CSound> mCloseSound;

public:
    void SetDoorFrameType(ERoomType Type);

    FVector2 GetDirection() const { return mDirection; }
    void SetDirection(FVector2 Dir) { mDirection = Dir; }
    void SetOpen(bool Val);
    void SetBoxSize(FVector2 size);
    void SetBoxSize(float x, float y);

    void MetRequirement(EOpenRequirement requirement);

    void SetDoorOpenInfo(FOpenInfo state);
    FOpenInfo GetDoorOpenInfo() const { return mInfo; }

private:
    void OnOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
    void ExitOverlaps(std::weak_ptr<class CCollider> Collider);
};

