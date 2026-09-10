#include "PickupBomb.h"

#include "TimeManager.h"

#include "../Manager/GameRuleManager.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"
#include "World/ColliderSphere2D.h"
#include "../Component/RigidBodyComponent.h"

#include "../Base/Unitbase.h"
#include "../Base/Boss.h"
#include "../Base/Obstaclebase.h"



REGISTER_GAMEOBJCLASS(CPickupBomb, "Bomb", EObjectType::Pickup)


FVector4 CPickupBomb::ColorPalette[4] =
{
	{1.f, 1.f, 1.f, 1.f},
	{1.f, 0.f, 0.f, 1.f},
	{1.f, 153.f / 255.f, 28.f / 255.f, 1.f},
	{1.f, 1.f, 0.f, 1.f},
};

CPickupBomb::CPickupBomb()
	:CPickup(EPickupType::Bomb, false, true)
{
}

CPickupBomb::~CPickupBomb()
{
}

bool CPickupBomb::Init()
{
	if (!CPickup::Init())
		return false;

	mMeshComp = CreateComponent<CMeshComponent>("Mesh");
	mAnimator = CreateComponent<CAnimation2DComponent>("Animator");
	mExplosionErea = CreateComponent<CColliderSphere2D>("Area");
	if (mMeshComp.expired() || mAnimator.expired() || mExplosionErea.expired())
		return false;

	mMeshComp.lock()->SetMesh("TexRect"); mMeshComp.lock()->SetShader("Animation2D"); mMeshComp.lock()->SetRenderLayer("Pickup");
	mMeshComp.lock()->SetSampler("Point_Unwrap", 0);
	mMeshComp.lock()->SetRelativeScale(60.f, 60.f);
	mAnimator.lock()->SetUpdateComponent(mMeshComp);

	mAnimator.lock()->AddAnimation("Bomb_Explosion", 1.f, 1.f, true);
	mAnimator.lock()->Stop(true);

	mExplosionErea.lock()->SetDebugDraw(true);
	mExplosionErea.lock()->SetRadius(130.f);
	mExplosionErea.lock()->SetCollisionProfile("Blasting");
	mExplosionErea.lock()->SetBeginOverlapFunc(this, &CPickupBomb::OnAreaOverlaps);
	mExplosionErea.lock()->SetEndOverlapFunc(this, &CPickupBomb::ExitAreaOverlaps);

	mCollider.lock()->SetRadius(10.f);

	Ignite();

	return true;
}

void CPickupBomb::Update(float DeltaTime)
{
	if (mbIsIgnited)
	{
		mIntensity += (10.f / mRemainTime) * DeltaTime;
		mMeshComp.lock()->SetHitEffect(0, true, mIntensity, ColorPalette[mCurrentPaletteIndex]);

		if (mIntensity >= 0.8f)
		{
			mIntensity = 0.f;
			mCurrentPaletteIndex = (mCurrentPaletteIndex + 1) % 4;
		}

		mRemainTime -= DeltaTime;
	}

	CPickup::Update(DeltaTime);
}

void CPickupBomb::Reset(bool HardReset)
{
	CPickup::Reset(HardReset);
	mbIsIgnited = false;
	mRemainTime = 2.f;
	mCurrentPaletteIndex = 0;

}

void CPickupBomb::Ignite()
{
	mbIsIgnited = true;
	CTimeManager::SetTimer(mRemainTime, false, this, &CPickupBomb::Explosion);

	mAnimator.lock()->Play();
}

void CPickupBomb::Explosion()
{
	//범위내 적들 100 피해 주기
	for (std::pair<int, std::weak_ptr<CGameObject>> pair : mInArea)
	{
		if (pair.second.expired())
			continue;

		EObjectType t = pair.second.lock()->GetObjType();
		switch (t)
		{
		case EObjectType::Boss: {
			std::shared_ptr<CBoss> boss = std::dynamic_pointer_cast<CBoss>(pair.second.lock());
			assert(boss && "객체가 잘못된 타입을 가지고 있습니다.");
			boss->GetHit(GetThisPtr<CGameObject>());
		}break;
		case EObjectType::PlayerCharacter:
		case EObjectType::Monster: {
			std::shared_ptr<CRoomMember> unit = std::dynamic_pointer_cast<CRoomMember>(pair.second.lock());
			assert(unit && "객체가 잘못된 타입을 가지고 있습니다.");
			unit->GetHit(GetThisPtr<CGameObject>());
		}break;
		case EObjectType::Obstacle: {
			std::shared_ptr<CObstaclebase> obs = std::dynamic_pointer_cast<CObstaclebase>(pair.second.lock());
			assert(obs && "객체가 잘못된 타입을 가지고 있습니다.");
			obs->TryBreak();
		}break;
		case EObjectType::Door:
			break;
		case EObjectType::Pickup: {
			std::shared_ptr<CPickup> pickup = std::dynamic_pointer_cast<CPickup>(pair.second.lock());
			assert(pickup && "객체가 잘못된 타입을 가지고 있습니다.");
			std::shared_ptr<CRigidBodyComponent> rb = std::dynamic_pointer_cast<CRigidBodyComponent>(pickup->GetRootComponent().lock());
			FVector3 dir = pickup->GetWorldPos() - GetWorldPos(); dir.Normalize();
			rb->AddForce(dir * 100.f);
		}break;
		case EObjectType::Tear:
		case EObjectType::Item:
		case EObjectType::Room:
		case EObjectType::End:
		default:
			continue;
		}
		//폭탄같은 효과를 주는게 더 있나
		// 유닛들은 Gethit 으로 판정하고
		// 오브젝트는 파괴가능한지 확인하고 파괴해야하는데 일단 그냥 파괴하자
		// 픽업은 밀려나고, 나머지는 효과없음
	}
	mInArea.clear();
	ReturnToChapter();
}

void CPickupBomb::OnAreaOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
	if (Collider.expired() || Collider.lock()->GetOwner().expired() || Collider.lock()->GetOwner().lock() == GetThisPtr<CActor>())
		return;

	std::shared_ptr<CGameObject> obj = std::dynamic_pointer_cast<CGameObject>(Collider.lock()->GetOwner().lock());
	if (!obj)
		return;

	mInArea.insert(std::make_pair(obj->GetID(), obj));
}

void CPickupBomb::ExitAreaOverlaps(std::weak_ptr<class CCollider> Collider)
{
	if (Collider.expired() || Collider.lock()->GetOwner().expired())
		return;

	std::shared_ptr<CGameObject> obj = std::dynamic_pointer_cast<CGameObject>(Collider.lock()->GetOwner().lock());
	if (!obj)
		return;

	mInArea.erase(obj->GetID());
}

bool CPickupBomb::TryGet(std::weak_ptr<class CCharacter> chara)
{
	if (mbIsIgnited)
		return false;

	CGameRuleManager::GetInst()->AddBomb(mAffectPickupCount);

	return true;
}
