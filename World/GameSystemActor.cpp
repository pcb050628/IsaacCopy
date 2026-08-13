#include "GameSystemActor.h"
#include "World/CameraComponent.h"

#include "World/World.h"
#include "World/Input.h"

#include "Chapter.h"
#include "Component/RigidBodyComponent.h"

CChapterSystemActor::CChapterSystemActor()
{
}

CChapterSystemActor::~CChapterSystemActor()
{
}

bool CChapterSystemActor::Init()
{
    mRb = CreateComponent<CRigidBodyComponent>("Root");
    mCam = CreateComponent<CCameraComponent>("SystemCam");
    if (mCam.expired() || mRb.expired())
        return false;

    mRb.lock()->SetLimit(5000.f);
    mRb.lock()->SetUseGravity(false);
    mRb.lock()->SetMass(0.f);

    std::shared_ptr<CInput> input = mWorld.lock()->GetInput().lock();
    input->AddBindKey("SystemManagerUp", VK_NUMPAD8);
    input->AddBindKey("SystemManagerDown", VK_NUMPAD5);
    input->AddBindKey("SystemManagerRight", VK_NUMPAD6);
    input->AddBindKey("SystemManagerLeft", VK_NUMPAD4);

    input->SetBindFunction("SystemManagerUp", EInputType::Hold, this, &CChapterSystemActor::MoveUp);
    input->SetBindFunction("SystemManagerDown", EInputType::Hold, this, &CChapterSystemActor::MoveDown);
    input->SetBindFunction("SystemManagerRight", EInputType::Hold, this, &CChapterSystemActor::MoveRight);
    input->SetBindFunction("SystemManagerLeft", EInputType::Hold, this, &CChapterSystemActor::MoveLeft);

    return true;
}

void CChapterSystemActor::Update(float DeltaTime)
{
    if (mbIsMoving)
    {
        MoveToTarget(DeltaTime);
    }

    CActor::Update(DeltaTime);
}

void CChapterSystemActor::MoveUp()
{
    AddWorldPos(0, 10.f);
}

void CChapterSystemActor::MoveDown()
{
    AddWorldPos(0, -10.f);
}

void CChapterSystemActor::MoveRight()
{
    AddWorldPos(10.f, 0);
}

void CChapterSystemActor::MoveLeft()
{
    AddWorldPos(-10.f, 0);
}

void CChapterSystemActor::MoveToTarget(float DeltaTime)
{
    FVector3 dir = mTargetPosition - GetWorldPos();
    dir.Normalize();
    mRb.lock()->AddForce(dir * 200.f);
    mCompareDist += DeltaTime * 60.f;
    if (GetWorldPos().Distance(mTargetPosition) < mCompareDist)
    {
        mbIsMoving = false;
        SetWorldPos(mTargetPosition);
        mRb.lock()->SetVelocity(FVector3::Zero);
        mTargetPosition = -FVector3::One;
        mMoveEndFunc();
    }
}

void CChapterSystemActor::Move(FVector2 dir)
{
    std::shared_ptr<CChapter> chptr = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
    std::shared_ptr<CRoombase> targetRoom = chptr->GetRoom(chptr->GetFocusedRoomCoord() + dir).lock();
    if (!targetRoom)
        return;

    mTargetPosition = targetRoom->GetWorldPos();
    mCompareDist = 10.f;
    mbIsMoving = true;
}
