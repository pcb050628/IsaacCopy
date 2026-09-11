#include "AttackFly.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "World/ColliderSphere2D.h"
#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"

#include "../Component/RigidBodyComponent.h"

REGISTER_GAMEOBJCLASS(CAttackFly, "Attack_Fly", EObjectType::Monster)

CAttackFly::CAttackFly()
{
}

CAttackFly::~CAttackFly()
{
}

bool CAttackFly::Init()
{
    if (!CFly::Init())
        return false;

    mAnimator.lock()->AddAnimation("Attack_Fly", 1.f, 10.f, true);
    mAnimator.lock()->ChangeAnimation("Attack_Fly");
    
    mMoveSpeed = 50.f;

    return true;
}

void CAttackFly::Update(float DeltaTime)
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

void CAttackFly::Reset(bool HardReset)
{
    CFly::Reset(HardReset);
    mAnimator.lock()->ChangeAnimation("Attack_Fly");
    mAnimator.lock()->Play(true);
    mTarget.reset();
}
