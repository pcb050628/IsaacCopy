#include "StartCameraActor.h"

#include "World/CameraComponent.h"
#include "../World/Component/RigidBodyComponent.h"

CStartCameraActor::CStartCameraActor()
{
}

CStartCameraActor::~CStartCameraActor()
{
}

bool CStartCameraActor::Init()
{
    mRb = CreateComponent<CRigidBodyComponent>("Root");
    mCam = CreateComponent<CCameraComponent>("Cam");
    if(mRb.expired() || mCam.expired())
	    return false;

    mRb.lock()->SetLimit(1000.f);

    return true;
}

void CStartCameraActor::Update(float DeltaTime)
{
    if (-1.f != mTargetHeight)
    {
        FVector3 worldpos = GetWorldPos();
        if(worldpos.y < mTargetHeight)
            mRb.lock()->AddForce(FVector3(0, 1, 0) * 100.f);
        else
            mRb.lock()->AddForce(FVector3(0, -1, 0) * 100.f);

        if (fabs(worldpos.y - mTargetHeight) < 10.f)
        {
            SetWorldPos(FVector3(worldpos.x, mTargetHeight, worldpos.z));
            mRb.lock()->SetVelocity(FVector3::Zero);
            mTargetHeight = -1.f;
            mOnMoveEnd();
        }
    }

    CActor::Update(DeltaTime);
}

void CStartCameraActor::MoveUp()
{
    mTargetHeight = GetWorldPos().y + 800.f;
}

void CStartCameraActor::MoveDown()
{
    mTargetHeight = GetWorldPos().y - 800.f;
}
