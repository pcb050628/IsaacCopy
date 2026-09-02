#pragma once
#include "../Base/Boss.h"

class CBossGemini :
    public CBoss
{
    GAMEOBJCLASS(CBossGemini, 91)
public:
    CBossGemini();
    virtual ~CBossGemini();

protected:
    bool mbIsAttached = true;
    //Contusion
    float mContusionHP = 10.f;
    float mMovingTimeStack = 0.f;
    float mBreathingTime = 5.f;
    bool mbIsBreathing = false;
    bool mbContusionJustHit = false;
    bool mbContusionDead = false;
    std::weak_ptr<class CRigidBodyComponent> mContusionRigidbody;
    std::weak_ptr<class CMeshComponent> mContusionFullBodyMesh;
    std::weak_ptr<class CMeshComponent> mContusionHeadMesh;
    std::weak_ptr<class CMeshComponent> mContusionBodyMesh;
    std::weak_ptr<class CAnimation2DComponent> mContusionFullBodyAnimator;
    std::weak_ptr<class CAnimation2DComponent> mContusionHeadAnimator;
    std::weak_ptr<class CAnimation2DComponent> mContusionBodyAnimator;
    std::weak_ptr<class CColliderSphere2D> mContusionHitBox;
    std::weak_ptr<class CColliderBox2D> mContusionHurtBox;
    std::weak_ptr<class CRouteMaker> mRouteMaker;
    uint64_t mBreathingStartTimerHandle = 0;
    uint64_t mBreathingEndTimerHandle = 0;

    //Suture
    float mSutureHP = 140.f;
    float mAttachDistLimit = 100.f;
    bool mbSutureJustHit = false;
    bool mbSutureDead = false;
    std::weak_ptr<class CRigidBodyComponent> mSutureRigidbody;
    std::weak_ptr<class CMeshComponent> mSutureMesh;
    std::weak_ptr<class CAnimation2DComponent> mSutureAnimator;
    std::weak_ptr<class CColliderSphere2D> mSutureHitBox;
    std::weak_ptr<class CColliderBox2D> mSutureHurtBox;
    std::weak_ptr<class CTearShooter> mShooter;
    uint64_t mShooterTimerHandle = 0;

    //rope
    std::vector<std::weak_ptr<class CSpriteComponent>> mRopeVec;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Reset(bool HardReset = false) override;
    
    virtual void GetHit(std::weak_ptr<CGameObject> From) override;

private:
    void MoveToPlayer(); //mbIsAttached 로 어떤 리지드바디를 움직일 것인지 판단하기

    void UpdateContusion(float DeltaTime);
    void StartBreathing();
    void BreathingEnd();
    void OnContusionHurtOverlap(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);

    void UpdateSuture(float DeltaTime);
    void FollowContusion();
    void ShootToPlayer();
    void Shoot();
    void OnSutureHurtOverlap(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);

    void UpdateRopePosition();

    void CheckIsDie();
    void Dettach();
};

