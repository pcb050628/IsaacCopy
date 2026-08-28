#include "GameSystemActor.h"

#include "Device.h"

#include "World/CameraComponent.h"
#include "World/SoundComponent.h"

#include "World/World.h"
#include "World/Input.h"

#include "Asset/AssetManager.h"
#include "Asset/SoundManager.h"

#include "Manager/GameRuleManager.h"

#include "Chapter.h"
#include "Component/RigidBodyComponent.h"
#include "Component/SpriteComponent.h"

CChapterSystemActor::CChapterSystemActor()
{
}

CChapterSystemActor::~CChapterSystemActor()
{
}

bool CChapterSystemActor::Init()
{
    mRb = CreateComponent<CRigidBodyComponent>("Root");
    mSound = CreateComponent<CSoundComponent>("SystemSound");
    mCam = CreateComponent<CCameraComponent>("SystemCam");
    if (mCam.expired() || mSound.expired() || mRb.expired())
        return false;

    mRb.lock()->SetLimit(5000.f);
    mRb.lock()->SetUseGravity(false);
    mRb.lock()->SetMass(0.f);
    
    std::shared_ptr<CSoundManager> mgr = CAssetManager::GetInst()->GetSubManager<CSoundManager>(EAssetType::Sound);
    if (!mgr)
        return false;

    //사운드 찾아서 연결하기
    mSound.lock()->mSound = mgr->FindSound("BGM_Basic").lock();
    mSound.lock()->Play();

    std::shared_ptr<CInput> input = mWorld.lock()->GetInput().lock();
    input->AddBindKey("SystemManagerUp", VK_NUMPAD8);
    input->AddBindKey("SystemManagerDown", VK_NUMPAD5);
    input->AddBindKey("SystemManagerRight", VK_NUMPAD6);
    input->AddBindKey("SystemManagerLeft", VK_NUMPAD4);

    input->SetBindFunction("SystemManagerUp", EInputType::Hold, this, &CChapterSystemActor::MoveUp);
    input->SetBindFunction("SystemManagerDown", EInputType::Hold, this, &CChapterSystemActor::MoveDown);
    input->SetBindFunction("SystemManagerRight", EInputType::Hold, this, &CChapterSystemActor::MoveRight);
    input->SetBindFunction("SystemManagerLeft", EInputType::Hold, this, &CChapterSystemActor::MoveLeft);

    FResolution resol = CDevice::GetInst()->GetResolution();
    mHeartSpriteOffset.x = -resol.Width / 2 + mHeartSpriteSpacing;
    mHeartSpriteOffset.y = resol.Height / 2 - mHeartSpriteSpacing;

    CGameRuleManager::GetInst()->RegisterPlayerHeartOnUpdate("GSA", this, &CChapterSystemActor::UpdateHeart);

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

void CChapterSystemActor::UpdateHeart(int id, FPlayerHeartContainer container)
{
    int length = container.RedContainer.size() + container.OtherContainer.size();
    if (mHearts.size() < length)
        mHearts.resize(length);

    for (std::weak_ptr<CSpriteComponent> sp : mHearts)
    {
        if (sp.expired())
            continue;
        sp.lock()->SetRenderEnable(false);
    }

    int point = 0;
    for (FPlayerHeartData d : container.RedContainer)
    {
        std::string Name = "UI_Heart_";
        switch (d.State)
        {
        case EPlayerHeartState::Empty:
            Name += "Empty";
            break;
        case EPlayerHeartState::Half:
            Name += "Red_Half";
            break;
        case EPlayerHeartState::Full:
            Name += "Red_Full";
            break;
        }

        std::shared_ptr<CSpriteComponent> comp = mHearts[point].lock();
        if (!comp)
        {
            mHearts[point] = CreateComponent<CSpriteComponent>("HeartSprite");
            comp = mHearts[point].lock();
            comp->SetMesh("TexRect"); comp->SetShader("Sprite2D");
        }

        comp->SetSpriteData(Name);
        comp->SetRelativePos(mHeartSpriteOffset + FVector3(mHeartSpriteSize * point + mHeartSpritePadding, 0, 0));
        comp->SetWorldScale(mHeartSpriteSize, mHeartSpriteSize);
        comp->SetRenderEnable(true);
        comp->SetRenderLayer("UI");
        ++point;
    }
    for (FPlayerHeartData d : container.OtherContainer)
    {
        std::string Name = "UI_Heart_";

        switch (d.Type)
        {
        case EPlayerHeartType::Blue:
            Name += "Blue_";
            break;
        case EPlayerHeartType::Black:
            Name += "Black_";
            break;
        }

        switch (d.State)
        {
        case EPlayerHeartState::Empty:
            continue;
        case EPlayerHeartState::Half:
            Name += "Red_Half";
            break;
        case EPlayerHeartState::Full:
            Name += "Red_Full";
            break;
        }

        std::shared_ptr<CSpriteComponent> comp = mHearts[point].lock();
        if (!comp)
        {
            mHearts[point] = CreateComponent<CSpriteComponent>("HeartSprite");
            comp = mHearts[point].lock();
            comp->SetMesh("TexRect"); comp->SetShader("Sprite2D");
        }

        comp->SetSpriteData(Name);
        comp->SetRelativePos(mHeartSpriteOffset + FVector3(mHeartSpriteSize * point + mHeartSpritePadding, 0, 0));
        comp->SetWorldScale(mHeartSpriteSize, mHeartSpriteSize);
        comp->SetRenderEnable(true);
        comp->SetRenderLayer("UI");
        ++point;
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
