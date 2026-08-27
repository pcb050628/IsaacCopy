#pragma once
#include "World\Actor.h"
#include "../GameObjectEnums.h"

class CDoor :
    public CActor
{
public:
    CDoor();
    CDoor(const CDoor& src);
    CDoor(CDoor&& src) noexcept;
    virtual ~CDoor();

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Destroy();

protected:
    std::weak_ptr<class CRoombase> mRoomOwner;
    std::weak_ptr<class CColliderBox2D> mBoxColComp;
    std::weak_ptr<class CSpriteComponent> mFrameRenderer;
    std::weak_ptr<class CSpriteComponent> mLayerRenderer1;
    std::weak_ptr<class CSpriteComponent> mLayerRenderer2;
    std::weak_ptr<class CSpriteComponent> mLayerRenderer3;
    
    std::weak_ptr<class CRigidBodyComponent> mLayer2Rigid;
    std::weak_ptr<class CRigidBodyComponent> mLayer3Rigid;

    std::weak_ptr<class CSoundComponent> mSound;

    FVector2 mDirection;
    bool mbIsOpen = false;
    EOpenRequirement mRequirement = EOpenRequirement::Clear;

    std::weak_ptr<class CUnitbase> mPlayer; //? 이거 왜 들고 있지

    std::weak_ptr<class CSound> mOpenSound;
    std::weak_ptr<class CSound> mCloseSound;

public:
    void SetDirection(FVector2 Dir) { mDirection = Dir; }
    void SetOpen(bool Val);
    void SetBoxSize(FVector2 size);
    void SetBoxSize(float x, float y);
    //다른 방 이동 함수 작성 및 연결하기

private:
    void OnOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
    void ExitOverlaps(std::weak_ptr<class CCollider> Collider);
};

