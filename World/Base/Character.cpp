#include "Character.h"
#include "World/World.h"
#include "World/Input.h"

#include "World/Collider.h"
#include "World/Animation2DComponent.h"

#include "../Chapter.h"
#include "Monster.h"
#include "../Component/RigidBodyComponent.h"
#include "../Component/TearShooter.h"

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

	shooter->AddFirePoint(FVector2());
	shooter->AddFirePoint(FVector2());

	shooter->AddOnCollision(this, &CCharacter::Attack);
	shooter->UpdateUnitAttributeData(false, mAttribute);

	std::shared_ptr<CRigidBodyComponent> rb = mRigidBody.lock();
	rb->SetMass(1.f);
	rb->SetLimit(500.f);

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


	mMoveDirection = FVector3::Zero;
	CUnitbase::Update(DeltaTime);
}

void CCharacter::Destroy()
{
	CUnitbase::Destroy();
}

void CCharacter::GetHit(std::weak_ptr<CUnitbase> From)
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
