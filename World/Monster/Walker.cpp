#include "Walker.h"

#include "LogManager.h"

#include "World/ColliderSphere2D.h"

CWalker::CWalker()
{
}

CWalker::CWalker(const CWalker& src)
	:CMonster(src)
{
}

CWalker::CWalker(CWalker&& src) noexcept
	:CMonster(std::move(src))
{
}

CWalker::~CWalker()
{
}

bool CWalker::Init()
{
	if (!CMonster::Init())
		return false;

	std::shared_ptr<CColliderSphere2D> hurt = mHurtBox.lock();
	hurt->SetCollisionProfile("Monster");

	mHitBox = CreateComponent<CColliderSphere2D>("Hit");
	if (mHitBox.expired())
		return false;

	std::shared_ptr<CColliderSphere2D> hit = mHitBox.lock();
	hit->SetCollisionProfile("ContactHit_Monster");
	hit->SetDebugDraw(true);

	return true;
}

void CWalker::Update(float DeltaTime)
{
	/*if (!mTarget.expired())
	{
		mTarget.lock()->GetHit(GetThisPtr<CUnitbase>());
	}*/

	CMonster::Update(DeltaTime);
}

void CWalker::Destory()
{
	CMonster::Destroy();
}

void CWalker::GetHit(std::weak_ptr<CUnitbase> From)
{

}

//피격
void CWalker::OnHurtOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
	std::shared_ptr<CActor> actor = Collider.lock()->GetOwner().lock();
	LOG_DEBUG(GetName(), "유닛이 ", actor->GetName(), " 에게 공격받았습니다");
}

void CWalker::ExitHurtOverlaps(std::weak_ptr<CCollider> Collider)
{
}

//공격
void CWalker::OnHitOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
	std::shared_ptr<CGameObject> gobj = std::dynamic_pointer_cast<CGameObject>(Collider.lock()->GetOwner().lock());
	if (!gobj)
	{
		assert("몬스터가 게임 객체가 아닌 무언가와 충돌함\n충돌체 프로파일상 불가능하고 생성되는 객체들 중에서도 충돌체를 가진 객체들은 모두 게임 객체여야함");
		return;
	}

	switch (gobj->GetObjType())
	{
	case EObjectType::PlayerCharacter: {
		std::shared_ptr<CUnitbase> unit = std::dynamic_pointer_cast<CUnitbase>(gobj);
		if (!unit)
		{
			LOG_DEBUG(GetName(), ":", "충돌체에 유닛이 없습니다. 충돌 프로파일을 수정하세요");
			return;
		}
		mTarget = unit;
		unit->GetHit(GetThisPtr<CUnitbase>());
	}
		break;
	case EObjectType::Monster:
	case EObjectType::Door:
	case EObjectType::Obstacle:
	case EObjectType::Pickup:
		//밀기
		break;
	}
}

void CWalker::ExitHitOverlaps(std::weak_ptr<CCollider> Collider)
{
	std::shared_ptr<CGameObject> gobj = std::dynamic_pointer_cast<CGameObject>(Collider.lock()->GetOwner().lock());
	if (gobj->GetID() == mTarget.lock()->GetID())
		mTarget.reset();
}
