#include "CharacterIsaac.h"

#include "World/Animation2DComponent.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "World/ColliderSphere2D.h"

#include "../Component/TearShooter.h"

REGISTER_GAMEOBJCLASS(CCharacterIsaac, "Isaac", EObjectType::PlayerCharacter);

CCharacterIsaac::CCharacterIsaac()
{
}

CCharacterIsaac::~CCharacterIsaac()
{
}

bool CCharacterIsaac::Init()
{
    if (!CCharacter::Init())
        return false;

    if (!AddAnim("Isaac_Head_Back", TEXT("Isaac_Head_Back"), true, 1.f, 1.f, true))
        return false;
    if (!AddAnim("Isaac_Head_Front", TEXT("Isaac_Head_Front"), true, 1.f, 1.f, true))
        return false;
    if (!AddAnim("Isaac_Head_Left", TEXT("Isaac_Head_Left"), true, 1.f, 1.f, true))
        return false;
    if (!AddAnim("Isaac_Head_Right", TEXT("Isaac_Head_Right"), true, 1.f, 1.f, true))
        return false;

    if (!AddAnim("Isaac_Body_Walk_V", TEXT("Isaac_Body_Walk_V"), false, 0.7f, 1.f, true))
        return false;
    if (!AddAnim("Isaac_Body_Walk_H", TEXT("Isaac_Body_Walk_H"), false, 0.7f, 1.f, true))
        return false;

    mHurtBox.lock()->SetRadius(20.f);
    mHurtBox.lock()->SetDebugDraw(true);

    SetBodyDirection(FVector2(0, -1));

    mAttribute.Range = 85.f * 5.f;
    mAttribute.ShotSpeed = 7.f; //이게 힘이고
    mAttribute.ShotTerm = 1.f;

    mShooter.lock()->UpdateUnitAttributeData(false, mAttribute);

    return true;
}

void CCharacterIsaac::Update(float DeltaTime)
{
    CCharacter::Update(DeltaTime);
}

void CCharacterIsaac::Destroy()
{
    CCharacter::Destroy();
}

void CCharacterIsaac::PlayBodyVerticalAnim()
{
    mBody.lock()->ChangeAnimation("Isaac_Body_Walk_V");
    bool symmetry = mBodyDirection.y > 0 ? true : false;
    mBody.lock()->SetSymmetry("Isaac_Body_Walk_V", symmetry);
    mBody.lock()->Play();
    if (!mbIsFiring)
        SetHeadDirection(mBodyDirection);
}

void CCharacterIsaac::PlayBodyHorizontalAnim()
{
    mBody.lock()->ChangeAnimation("Isaac_Body_Walk_H");
    bool symmetry = mBodyDirection.x > 0 ? false : true;
	mBody.lock()->SetSymmetry("Isaac_Body_Walk_H", symmetry);
    mBody.lock()->Play();
    if(!mbIsFiring)
        SetHeadDirection(mBodyDirection);
}

void CCharacterIsaac::PlayHeadVerticalAnim()
{
    bool isUp = mHeadDirection.y > 0 ? false : true;
    if(isUp)
        mHead.lock()->ChangeAnimation("Isaac_Head_Back");
    else
        mHead.lock()->ChangeAnimation("Isaac_Head_Front");

    if (mbIsFiring)
    {
        mHead.lock()->SetFrame(1);
        mHead.lock()->Play();
    }
    else
    {
        mHead.lock()->Stop(true);
    }
}

void CCharacterIsaac::PlayHeadHorizontalAnim()
{
    bool isRight = mHeadDirection.x > 0 ? false : true;
    if (isRight)
        mHead.lock()->ChangeAnimation("Isaac_Head_Left");
    else
        mHead.lock()->ChangeAnimation("Isaac_Head_Right");

    if (mbIsFiring)
    {
        mHead.lock()->SetFrame(1);
        mHead.lock()->Play();
    }
    else
    {
        mHead.lock()->Stop(true);
    }
}
