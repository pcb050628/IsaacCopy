#include "Sucker.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "World/ColliderSphere2D.h"
#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"

#include "../Component/RigidBodyComponent.h"
#include "../Component/TearShooter.h"

REGISTER_GAMEOBJCLASS(CSucker, "Sucker", EObjectType::Monster)

CSucker::CSucker()
{
}

CSucker::~CSucker()
{
}

bool CSucker::Init()
{
    if (!CFly::Init())
        return false;

    mShooter = CreateComponent<CTearShooter>("Shooter");
    if (mShooter.expired())
        return false;

    FTearAttribute attribute;
    attribute.Speed = 15.f;
    attribute.Range = 400.f;

    FVector3 pos = GetWorldPos();
    mShooter.lock()->SetCenterComponent(mRoot);
    mShooter.lock()->AddFirePoint(FVector2(pos.x, pos.y));
    mShooter.lock()->SetTearAttribute(attribute);

    mAnimator.lock()->AddAnimation("Sucker_Fly", 1.f, 10.f, true);
    mAnimator.lock()->ChangeAnimation("Sucker_Fly");

    mMoveSpeed = 50.f;

    return true;
}

void CSucker::Update(float DeltaTime)
{
    if (mTarget.expired())
    {
        mTarget = mChapter.lock()->GetPlayerCharacter();
    }
    else
    {
        FVector3 dir = mTarget.lock()->GetWorldPos() - GetWorldPos(); dir.Normalize();
        mRigidBody.lock()->SetVelocity(dir * mMoveSpeed);
    }

    CFly::Update(DeltaTime);
}

void CSucker::Reset(bool HardReset)
{
    CFly::Reset(HardReset);
    mAnimator.lock()->ChangeAnimation("Sucker_Fly");
    mAnimator.lock()->Play(true);
}

void CSucker::Dead()
{
    CFly::Dead();
    std::shared_ptr<CTearShooter> shooter = mShooter.lock();
    shooter->FireWithoutCalculate(FVector2(0.f, 1.f));
    shooter->FireWithoutCalculate(FVector2(0.f, -1.f));
    shooter->FireWithoutCalculate(FVector2(1.f, 0.f));
    shooter->FireWithoutCalculate(FVector2(-1.f, 0.f));
}
