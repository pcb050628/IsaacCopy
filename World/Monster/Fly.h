#pragma once
#include "../Base/RoomMember.h"
#include "../Base/Monster.h"

class CFly :
    public CRoomMember, public CMonster
{
public:
    CFly();
    virtual ~CFly();

protected:
    std::weak_ptr<class CRigidBodyComponent> mRigidBody;
    std::weak_ptr<class CColliderSphere2D> mHurt;
    std::weak_ptr<class CColliderSphere2D> mHit;
    std::weak_ptr<class CMeshComponent> mMesh;
    std::weak_ptr<class CAnimation2DComponent> mAnimator;

    float mMoveSpeed = 100.f;
    float mHitIntensity = 0.f;

    std::unordered_map<int, std::weak_ptr<CRoomMember>> mHitList;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

    virtual void Dead() override;

    virtual void GetHit(std::weak_ptr<CGameObject> From) override;
    virtual void Reset(bool HardReset = false) override;

protected:
    virtual void OnDead();

    virtual void OnHitBoxEnter(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
    virtual void ExitHitBoxEnter(std::weak_ptr<class CCollider> Collider);

};

