#include "Character.h"
#include "World/World.h"
#include "World/Input.h"

#include "LogManager.h"

#include "Asset/AssetManager.h"
#include "Asset/AnimationManager.h"
#include "Asset/Animation2D.h"

#include "World/Collider.h"
#include "World/ColliderSphere2D.h"
#include "World/Animation2DComponent.h"

#include "../Chapter.h"
#include "Monster.h"
#include "../Component/RigidBodyComponent.h"
#include "../Component/TearShooter.h"
#include "../Component/ItemContainer.h"

CCharacter::CCharacter()
	:CUnitbase(EObjectType::PlayerCharacter)
{
}

CCharacter::~CCharacter()
{
}

bool CCharacter::Init()
{
	if (!CUnitbase::Init())
		return false;

	//키 입력 설정만
	//캐릭터 텍스쳐 등 개인적인 설정은 상속받은 객체에서

	//일단은 하드코딩으로 하지만
	//나중에 챕터 로드만들때 키 입력 데이터 만들어 두기(1p, 2p)

	//아래도 플레이 인풋이랑 유아이 인풋나눠서 챕터에서 보관하기

	mShooter = CreateComponent<CTearShooter>("Shooter");
	std::shared_ptr<CTearShooter> shooter = mShooter.lock();
	if (!shooter)
		return false;
	
	shooter->AddFirePoint(FVector2(-10, -20)); //왼
	shooter->AddFirePoint(FVector2(10, -20));	//오

	shooter->AddOnCollision(this, &CCharacter::Attack);
	shooter->UpdateUnitAttributeData(false, mAttribute);

	std::shared_ptr<CRigidBodyComponent> rb = mRigidBody.lock();
	rb->SetMass(1.f);
	rb->SetLimit(500.f);

	mHurtBox.lock()->SetCollisionProfile("Player");

	mItemContainer = CreateComponent<CItemContainer>("ItemContainer");
	if (mItemContainer.expired())
		return false;

	std::shared_ptr<CInput> input = mWorld.lock()->GetInput().lock();
	if (!input)
		return false;

	input->AddBindKey("MoveUp", 'W');
	input->AddBindKey("MoveLeft", 'A');
	input->AddBindKey("MoveDown", 'S');
	input->AddBindKey("MoveRight", 'D');

	input->AddBindKey("FireUp", VK_UP);
	input->AddBindKey("FireDown", VK_DOWN);
	input->AddBindKey("FireLeft", VK_LEFT);
	input->AddBindKey("FireRight", VK_RIGHT);

	input->AddBindKey("UseItem", VK_SPACE);
	input->AddBindKey("UsePickup", 'Q');
	input->AddBindKey("UseBomb", 'E');

	input->AddBindKey("DropPickup_Down", VK_LCONTROL);
	input->AddBindKey("DropPickup_Up", VK_LCONTROL);

	input->SetBindFunction("MoveUp", EInputType::Hold, this, &CCharacter::MoveUp);
	input->SetBindFunction("MoveDown", EInputType::Hold, this, &CCharacter::MoveDown);
	input->SetBindFunction("MoveLeft", EInputType::Hold, this, &CCharacter::MoveLeft);
	input->SetBindFunction("MoveRight", EInputType::Hold, this, &CCharacter::MoveRight);

	input->SetBindFunction("FireUp", EInputType::Hold, this, &CCharacter::FireUp);
	input->SetBindFunction("FireDown", EInputType::Hold, this, &CCharacter::FireDown);
	input->SetBindFunction("FireLeft", EInputType::Hold, this, &CCharacter::FireLeft);
	input->SetBindFunction("FireRight", EInputType::Hold, this, &CCharacter::FireRight);

	OnAttributeChanged(mAttribute);

	return true;
}

void CCharacter::Update(float DeltaTime)
{
	if (FVector3::Zero != mMoveDirection)
	{
		std::shared_ptr rb = mRigidBody.lock();
		if (rb)
		{
			mMoveDirection.Normalize();
			rb->AddForce(mMoveDirection * mAttribute.Speed * 5.f);
		}

		if (fabs(mMoveDirection.x) == fabs(mMoveDirection.y))
		{
		}
		else if (fabs(mMoveDirection.x) > fabs(mMoveDirection.y))
		{
			if (mMoveDirection.x > 0)
				SetBodyDirection(FVector2(1, 0));
			else
				SetBodyDirection(FVector2(-1, 0));
		}
		else
		{
			if (mMoveDirection.y > 0)
				SetBodyDirection(FVector2(0, 1));
			else
				SetBodyDirection(FVector2(0, -1));
		}

		mBody.lock()->Play();
	}
	else
	{
		SetBodyDirection(FVector2(0, -1));
		mBody.lock()->Stop(true);

	}

	if (mbIsFiring)
		mHead.lock()->Play();
	else
	{
		SetHeadDirection(mBodyDirection);
		mHead.lock()->Stop(true);
	}

	mMoveDirection = FVector3::Zero;
	CUnitbase::Update(DeltaTime);

	mbIsFiring = false;
	mbIsJustFired = false;
}

void CCharacter::Destroy()
{
	CUnitbase::Destroy();
}

void CCharacter::GetHit(std::weak_ptr<CGameObject> From)
{
}

void CCharacter::Reset(bool hard) //캐릭터는 사용할 일이 없음
{
}

void CCharacter::OnHurtOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
}

void CCharacter::ExitHurtOverlaps(std::weak_ptr<CCollider> Collider)
{
}

void CCharacter::OnHitOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
}

void CCharacter::ExitHitOverlaps(std::weak_ptr<CCollider> Collider)
{
}

void CCharacter::OverrideHeadAnim(const std::string& Name)
{
	mHead.lock()->AddAnimation(Name + "_Front", 1.f, mAttribute.ShotTerm, true);
	mHead.lock()->AddAnimation(Name + "_Back", 1.f, mAttribute.ShotTerm, true);
	mHead.lock()->AddAnimation(Name + "_Right", 1.f, mAttribute.ShotTerm, true);
	mHead.lock()->AddAnimation(Name + "_Left", 1.f, mAttribute.ShotTerm, true);

	mHeadAnimName = Name;
}

void CCharacter::OverrideBodyAnim(const std::string& Name)
{
	mBody.lock()->AddAnimation(Name + "_V", 0.7f, 0.7f, true);
	mBody.lock()->AddAnimation(Name + "_H", 0.7f, 0.7f, true);

	mBodyAnimName = Name;
}

void CCharacter::OnGetPickup(EPickupType Type, int Count)
{

}

void CCharacter::OnLosePickup(EPickupType Type, int Count)
{
}

void CCharacter::OnAttributeChanged(FUnitAttribute Attribute, bool IsMagnification)
{
	//여ㅑ기서 능력치 변동 해주기
	mAttribute = Attribute;

	mHead.lock()->SetPlayTime(mHeadAnimName + "_Front", mAttribute.ShotTerm);
	mHead.lock()->SetPlayTime(mHeadAnimName + "_Back", mAttribute.ShotTerm);
	mHead.lock()->SetPlayTime(mHeadAnimName + "_Left", mAttribute.ShotTerm);
	mHead.lock()->SetPlayTime(mHeadAnimName + "_Right", mAttribute.ShotTerm);
}

void CCharacter::PlayBodyVerticalAnim()
{
	mBody.lock()->ChangeAnimation(mBodyAnimName + "_V");
	bool symmetry = mBodyDirection.y > 0 ? true : false;
	mBody.lock()->SetSymmetry(mBodyAnimName + "_V", symmetry);
	mBody.lock()->Play();
	if (!mbIsFiring)
		SetHeadDirection(mBodyDirection);

	if (mItemContainer.expired())
		return;
	mItemContainer.lock()->SetBodyDirection(mBodyDirection);
}

void CCharacter::PlayBodyHorizontalAnim()
{
	mBody.lock()->ChangeAnimation(mBodyAnimName + "_H");
	bool symmetry = mBodyDirection.x > 0 ? false : true;
	mBody.lock()->SetSymmetry(mBodyAnimName + "_H", symmetry);
	mBody.lock()->Play();
	if (!mbIsFiring)
		SetHeadDirection(mBodyDirection);

	if (mItemContainer.expired())
		return;
	mItemContainer.lock()->SetBodyDirection(mBodyDirection);
}

void CCharacter::PlayHeadVerticalAnim()
{
	bool isUp = mHeadDirection.y > 0 ? false : true;
	if (isUp)
		mHead.lock()->ChangeAnimation(mHeadAnimName + "_Back");
	else
		mHead.lock()->ChangeAnimation(mHeadAnimName + "_Front");

	if (!mbIsFiring)
	{
		mHead.lock()->Stop(true);
	}

	if (mItemContainer.expired())
		return;
	mItemContainer.lock()->SetHeadDirection(mHeadDirection);
}

void CCharacter::PlayHeadHorizontalAnim()
{
	bool isRight = mHeadDirection.x > 0 ? false : true;
	if (isRight)
		mHead.lock()->ChangeAnimation(mHeadAnimName + "_Left");
	else
		mHead.lock()->ChangeAnimation(mHeadAnimName + "_Right");

	if (!mbIsFiring)
	{
		mHead.lock()->Stop(true);
	}

	if (mItemContainer.expired())
		return;
	mItemContainer.lock()->SetHeadDirection(mHeadDirection);
}

void CCharacter::MoveUp()
{
	mMoveDirection.y += 1;
}

void CCharacter::MoveLeft()
{
	mMoveDirection.x -= 1;
}

void CCharacter::MoveDown()
{
	mMoveDirection.y -= 1;
}

void CCharacter::MoveRight()
{
	mMoveDirection.x += 1;
}

void CCharacter::Fire()
{
	if (mShooter.expired())
		return;

	mbIsFiring = true;
	std::shared_ptr<CTearShooter> shooter = mShooter.lock();

	//일단 해놓긴 했는데
	//발사 가능 상태를 어디서 검사하는게 맞는지 확신이 없네
	//일단은 슈터에서 확인하는게 맞는거같긴함
	//유닛쪽에서는 모든 역할을 슈터에 위임한거니까
	//아는건 아무것도 없고 능력치만 던져주고 발사하라고만 하는거니까
	//검사는 슈터에서 해야겟지?
	//아닌거같긴한데 아닌거같은데
	FVector3 dir = mRigidBody.lock()->GetVelocity();
	dir.Normalize();
	if (shooter->FireWithVelocityOffset(FVector2(dir.x, dir.y)))
	{
		mbIsJustFired = true;

		mHead.lock()->Play(true);
		mHead.lock()->SetFrame(1);
	}
}

void CCharacter::FireUp()
{
	SetHeadDirection(FVector2(0, 1));
	Fire();
}

void CCharacter::FireLeft()
{
	SetHeadDirection(FVector2(-1, 0));
	Fire();
}

void CCharacter::FireDown()
{
	SetHeadDirection(FVector2(0, -1));
	Fire();
}

void CCharacter::FireRight()
{
	SetHeadDirection(FVector2(1, 0));
	Fire();
}

void CCharacter::UseItem()
{
}

void CCharacter::UsePickup()
{
}

void CCharacter::UseBomb()
{
}

void CCharacter::DropPickupPress()
{
}

void CCharacter::DropPickupRelease()
{
}

void CCharacter::Attack(const FVector3& Point, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
	if (typeid(CMonster) != typeid(Collider.lock()->GetOwner().lock().get()))
		return;

	std::shared_ptr<CUnitbase> unit = std::dynamic_pointer_cast<CUnitbase>(Collider.lock()->GetOwner().lock());
	unit->GetHit(GetThisPtr<CUnitbase>());
	//노말로 넉백주기
}
