#include "CharacterIsaac.h"

#include "LogManager.h"

#include "World/Input.h"
#include "World/Animation2DComponent.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "World/ColliderSphere2D.h"

#include "../Component/TearShooter.h"
#include "../Component/ItemContainer.h"

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

    mDefaultHeadAnimName = "Isaac_Head";
    mDefaultBodyAnimName = "Isaac_Body_Walk";

    mHeadAnimName = mDefaultHeadAnimName;
    mBodyAnimName = mDefaultBodyAnimName;

    mHurtBox.lock()->SetRadius(20.f);
    mHurtBox.lock()->SetDebugDraw(true);

    SetBodyDirection(FVector2(0, -1));

    mAttribute.Damage = 7.f;
    mAttribute.Range = 85.f * 5.f;
    mAttribute.ShotSpeed = 7.f; //이게 힘이고
    mAttribute.ShotTerm = 1.f;
    mAttribute.Speed = 10.f;

    mShooter.lock()->UpdateUnitAttributeData(false, mAttribute);

    auto input = mWorld.lock()->GetInput().lock();
    input->AddBindKey("TestFunc", 'J');
    input->SetBindFunction("TestFunc", EInputType::Press, this, &CCharacterIsaac::TestFuncForItemContainer);

    mActorTag = "Isaac";

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

void CCharacterIsaac::TestFuncForItemContainer()
{
    mItemContainer.lock()->ContainItem(91);
    mItemContainer.lock()->ContainItem(92);
    mItemContainer.lock()->ContainItem(93);
}
