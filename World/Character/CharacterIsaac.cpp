#include "CharacterIsaac.h"

#include "World/Animation2DComponent.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "World/ColliderSphere2D.h"

REGISTER_GAMECLASS(CCharacterIsaac, "Isaac", EObjectType::PlayerCharacter);

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
    mHurtBox.lock()->SetRenderLayer("Debug");

    SetBodyDirection(FVector2(0, -1));

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
}

void CCharacterIsaac::PlayBodyHorizontalAnim()
{
    mBody.lock()->ChangeAnimation("Isaac_Body_Walk_H");
    bool symmetry = mBodyDirection.x > 0 ? false : true;
	mBody.lock()->SetSymmetry("Isaac_Body_Walk_H", symmetry);
    mBody.lock()->Play();
}

void CCharacterIsaac::PlayHeadVerticalAnim()
{
}

void CCharacterIsaac::PlayHeadHorizontalAnim()
{
}
