#pragma once
#include "../Base/Monster.h"

class CWalker :
    public CMonster
{
public:
    CWalker();
    CWalker(const CWalker& src);
    CWalker(CWalker&& src) noexcept;
    virtual ~CWalker();

public:
    virtual bool Init();
    virtual void Update(float DeltaTime) = 0;
    virtual void Destory();
    virtual void Dead() = 0;

    virtual void GetHit(std::weak_ptr<CUnitbase> From);
protected:
    virtual void OnHurtOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
    virtual void ExitHurtOverlaps(std::weak_ptr<CCollider> Collider);

    virtual void OnHitOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
    virtual void ExitHitOverlaps(std::weak_ptr<CCollider> Collider);

protected:
    std::weak_ptr<class CColliderSphere2D> mHitBox;
    std::weak_ptr<CUnitbase> mTarget;
};

