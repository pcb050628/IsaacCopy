#include "RigidBodyComponent.h"

CRigidBodyComponent::CRigidBodyComponent()
{
}

CRigidBodyComponent::CRigidBodyComponent(const CRigidBodyComponent& src)
    :CSceneComponent(src)
{
}

CRigidBodyComponent::CRigidBodyComponent(CRigidBodyComponent&& src) noexcept
    :CSceneComponent(std::move(src))
{
}

CRigidBodyComponent::~CRigidBodyComponent()
{
}

bool CRigidBodyComponent::Init()
{


    CSceneComponent::Init();
    return true;
}

void CRigidBodyComponent::Update(float DeltaTime)
{
    if (!mRBVelocity.IsZero())
    {
        float friction = mMass * mGravity * DeltaTime;
        mRBVelocity -= friction;

        AddRelativePos(mRBVelocity);

        if (mRBVelocity.Length() < 0.1f)
        {
            mRBVelocity = FVector3::Zero;
        }
    }
}

void CRigidBodyComponent::PostUpdate(float DeltaTime)
{
}

void CRigidBodyComponent::Destroy()
{
    CSceneComponent::Destroy();
}
