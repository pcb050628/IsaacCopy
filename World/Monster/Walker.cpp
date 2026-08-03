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
	if (CMonster::Init())
		return false;

	std::shared_ptr<CColliderSphere2D> hurt = mHurtBox.lock();

	hurt->SetCollisionProfile("Monster");

	mHitBox = CreateComponent<CColliderSphere2D>("Hit");
	if (mHitBox.expired())
		return false;

	std::shared_ptr<CColliderSphere2D> hit = mHitBox.lock();

	hit->SetCollisionProfile("ContactHit_Monster");

	return true;
}

void CWalker::Update(float DeltaTime)
{
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
	std::shared_ptr<CUnitbase> unit = std::dynamic_pointer_cast<CUnitbase>(Collider.lock()->GetOwner().lock());
	if (!unit)
	{
		LOG_DEBUG(GetName(), ":","충돌체에 유닛이 없습니다. 충돌 프로파일을 수정하세요");
		return;
	}

	unit->GetHit(std::dynamic_pointer_cast<CUnitbase>(GetThisPtr()));
}

void CWalker::ExitHitOverlaps(std::weak_ptr<CCollider> Collider)
{
}
