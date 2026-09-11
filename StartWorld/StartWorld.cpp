#include "StartWorld.h"

#include "Engine.h"
#include "Device.h"
#include "World/WorldManager.h"
#include "World/Input.h"

#include "StartCameraActor.h"
#include "ScreenActor.h"

#include "TitleActor.h"
#include "MenuActor.h"
#include "CharacterMenu.h"

#include "../World/Chapter.h"
#include "World/CameraComponent.h"

CStartWorld::CStartWorld()
{
}

CStartWorld::~CStartWorld()
{
}

bool CStartWorld::Init()
{
    if (!CWorld::Init())
        return false;
    mTitle = CreateActor<CTitleActor>("Title");
    mMenu = CreateActor<CMenuActor>("Menu");
    mCharacterMenu = CreateActor<CCharacterMenu>("Character");
    mCam = CreateActor<CStartCameraActor>("cam");
    if (mTitle.expired() || mMenu.expired() || mCam.expired())
        return false;

    mCam.lock()->SetMoveEndFunction(this, &CStartWorld::MoveEnd);

    mInput->AddBindKey("Up", VK_UP);
    mInput->AddBindKey("Down", VK_DOWN);
    mInput->AddBindKey("Right", VK_RIGHT);
    mInput->AddBindKey("Left", VK_LEFT);
    mInput->AddBindKey("Submit", VK_SPACE);
    mInput->AddBindKey("Escape", VK_ESCAPE);

    mInput->SetBindFunction("Up", EInputType::Press, this, &CStartWorld::OnUp);
    mInput->SetBindFunction("Down", EInputType::Press, this, &CStartWorld::OnDown);
    mInput->SetBindFunction("Right", EInputType::Press, this, &CStartWorld::OnRight);
    mInput->SetBindFunction("Left", EInputType::Press, this, &CStartWorld::OnLeft);

    mInput->SetBindFunction("Submit", EInputType::Press, this, &CStartWorld::OnSubmit);
    mInput->SetBindFunction("Escape", EInputType::Press, this, &CStartWorld::OnEscape);

    FResolution resol = CDevice::GetInst()->GetResolution();
    mMenu.lock()->SetWorldPos(0, -resol.Height);
    mCharacterMenu.lock()->SetWorldPos(0, -(resol.Height * 2));

    return true;
}

void CStartWorld::Update(float DeltaTime)
{
    CWorld::Update(DeltaTime);
}

void CStartWorld::PostRender()
{
    CWorld::PostRender();
    if(mbMoveToChapter)
        CWorldManager::GetInst()->CreateWorld<CChapter>(false);
}

void CStartWorld::OnUp()
{
    switch (mFocused)
    {
    case 0:
        mTitle.lock()->OnUp();
        break;
    case 1:
        mMenu.lock()->OnUp();
        break;
    case 2:
        mCharacterMenu.lock()->OnUp();
        break;
    }
}

void CStartWorld::OnDown()
{
    switch (mFocused)
    {
    case 0:
        mTitle.lock()->OnDown();
        break;
    case 1:
        mMenu.lock()->OnDown();
        break;
    case 2:
        mCharacterMenu.lock()->OnDown();
        break;
    }
}

void CStartWorld::OnRight()
{
    switch (mFocused)
    {
    case 0:
        mTitle.lock()->OnRight();
        break;
    case 1:
        mMenu.lock()->OnRight();
        break;
    case 2:
        mCharacterMenu.lock()->OnRight();
        break;
    }
}

void CStartWorld::OnLeft()
{
    switch (mFocused)
    {
    case 0:
        mTitle.lock()->OnLeft();
        break;
    case 1:
        mMenu.lock()->OnLeft();
        break;
    case 2:
        mCharacterMenu.lock()->OnLeft();
        break;
    }
}

void CStartWorld::OnSubmit()
{
    switch (mFocused)
    {
    case 0:
        if (mTitle.lock()->OnSubmit())
        {
            mCam.lock()->MoveDown();
            MoveStart();
            ++mFocused;
        }
        break;
    case 1: {
        int result = mMenu.lock()->OnSubmit();
        if (0 == result)
        {
            mbMoveToChapter = true;
        }
        else if (1 == result)
        {
            mCam.lock()->MoveDown();
            MoveStart();
            ++mFocused;
        }
    }break;
    case 2:
        if (mCharacterMenu.lock()->OnSubmit())
        {
            mbMoveToChapter = true;
        }
        break;
    }
}

void CStartWorld::OnEscape()
{
    switch (mFocused)
    {
    case 0:
        mTitle.lock()->OnEscape();
        break;
    case 1:
        if (mMenu.lock()->OnEscape())
        {
            mCam.lock()->MoveUp();
            MoveStart();
            --mFocused;
        }
        break;
    case 2:
        if(mCharacterMenu.lock()->OnEscape())
        {
            mCam.lock()->MoveUp();
            MoveStart();
            --mFocused;
        }
        break;
    }
}

void CStartWorld::MoveStart()
{
    mInput->SetEnable(false);
}

void CStartWorld::MoveEnd()
{
    mInput->SetEnable(true);
}
