#include "Fly.h"

#include "World/ColliderSphere2D.h"
#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"
#include "../Component/RigidBodyComponent.h"

#include "../Base/Character.h"
#include "../Pickup/PickupBomb.h"

#include "../Base/Tear.h"

CFly::CFly()
	:CRoomMember(EObjectType::Monster)
{
}

CFly::~CFly()
{
}

bool CFly::Init()
{
	if (!CRoomMember::Init())
		return false;

	mRigidBody = CreateComponent<CRigidBodyComponent>("Root");
	mHurt = CreateComponent<CColliderSphere2D>("hurt");
	mHit = CreateComponent<CColliderSphere2D>("hit");
	mMesh = CreateComponent<CMeshComponent>("Mesh");
	mAnimator = CreateComponent<CAnimation2DComponent>("Animator");
	if (mRigidBody.expired() || mHurt.expired() || mHit.expired() || mMesh.expired() || mAnimator.expired())
		return false;

	std::shared_ptr<CRigidBodyComponent> rb = mRigidBody.lock();
	std::shared_ptr<CColliderSphere2D> hurt = mHurt.lock();
	std::shared_ptr<CColliderSphere2D> hit = mHit.lock();
	std::shared_ptr<CMeshComponent> mesh = mMesh.lock();
	std::shared_ptr<CAnimation2DComponent> animator = mAnimator.lock();

	mesh->SetWorldScale(30.f, 30.f);

	hurt->SetRadius(10.f);
	hurt->SetDebugDraw(true);
	hurt->SetCollisionProfile("Monster"); //Monster 피격 | Contact_Fly 공격

	hit->SetRadius(15.f);
	hit->SetDebugDraw(true);
	hit->SetBeginOverlapFunc(this, &CFly::OnHitBoxEnter);
	hit->SetEndOverlapFunc(this, &CFly::ExitHitBoxEnter);
	hit->SetCollisionProfile("ContactHit_Fly");


	mesh->SetMesh("TexRect"); mesh->SetShader("Animation2D"); mesh->SetRenderLayer("Head");
	animator->SetUpdateComponent(mMesh);

	animator->AddAnimation("Fly_Die", 0.2f, 1.f);
	animator->SetFinishFunction("Fly_Die", this, &CFly::OnDead);

	return true;
}

void CFly::Update(float DeltaTime)
{
	if (0.f < mHitIntensity)
	{
		mMesh.lock()->SetHitEffect(0, true, mHitIntensity, FVector4(1.f, 0.f, 0.f, 1.f));
		mHitIntensity -= DeltaTime * 10.f;
		if (0.f >= mHitIntensity)
		{
			mMesh.lock()->SetHitEffect(0, false, 0.f, FVector4::One);
		}
	}

	for (std::pair<int, std::weak_ptr<CRoomMember>> pair : mHitList)
	{
		pair.second.lock()->GetHit(GetThisPtr<CGameObject>());
		break;
	}

	CRoomMember::Update(DeltaTime);
}

void CFly::Dead()
{
	mAnimator.lock()->ChangeAnimation("Fly_Die");
	mAnimator.lock()->Play(true);
	mHurt.lock()->SetEnable(false);
	mHit.lock()->SetEnable(false);
}

void CFly::GetHit(std::weak_ptr<CGameObject> From)
{
	if (From.expired())
		return;

	std::shared_ptr<CGameObject> obj = From.lock();

	if (From.lock()->GetObjType() == EObjectType::Tear)
	{
		std::shared_ptr<CTear> tear = std::dynamic_pointer_cast<CTear>(From.lock());
		obj = tear->GetShooterOwner().lock();
	}

	switch (obj->GetObjType())
	{
	case EObjectType::PlayerCharacter: { //능력치 가져와서 피 깎기
		std::shared_ptr<CCharacter> chara = std::dynamic_pointer_cast<CCharacter>(obj);
		assert(chara && "객체가 잘못된 타입을 가지고 있습니다.");
		mCurrentHP -= chara->GetTotalAttribute().Damage;
	}break;
		break;
	case EObjectType::Pickup: { //폭탄인지 확인하고 100깎기
		std::shared_ptr<CPickupBomb> bomb = std::dynamic_pointer_cast<CPickupBomb>(obj);
		if (!bomb)
			return;
		mCurrentHP -= 100;
	}break;
	case EObjectType::Tear: //오너 능력치 가져와서 피깎기
	case EObjectType::Monster: //일단 예외
	case EObjectType::Item: //일단 예외
	case EObjectType::Obstacle: //일단 예외
	case EObjectType::Boss: //일단 예외
	case EObjectType::Door:
	case EObjectType::Room:
	case EObjectType::End:
	default:
		return;
	}

	if (0 >= mCurrentHP)
		Dead();

	FVector3 dir = From.lock()->GetWorldPos() - GetWorldPos(); dir.Normalize();
	mRigidBody.lock()->AddForce(dir * 500.f);
	mHitIntensity = 1.f;
}

void CFly::Reset(bool HardReset)
{
	mHurt.lock()->SetEnable(true);
	mHit.lock()->SetEnable(true);
	mCurrentHP = mMaxHP;
}

void CFly::OnDead()
{
	ReturnToChapter();
}

void CFly::OnHitBoxEnter(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
	if (Collider.expired() || Collider.lock()->GetOwner().expired())
		return;
	std::shared_ptr<CGameObject> obj = std::dynamic_pointer_cast<CGameObject>(Collider.lock()->GetOwner().lock());
	if (!obj)
		return;

	switch (obj->GetObjType())
	{
	case EObjectType::PlayerCharacter: {
	}break;
	case EObjectType::Boss:
	case EObjectType::Monster:
	case EObjectType::Obstacle:
	case EObjectType::Pickup:
	case EObjectType::Tear:
	case EObjectType::Door:
	case EObjectType::Room:
	case EObjectType::Item:
	case EObjectType::End:
	default:
		return;
	}

	mHitList.insert(std::make_pair(obj->GetID(), std::dynamic_pointer_cast<CRoomMember>(obj)));
}

void CFly::ExitHitBoxEnter(std::weak_ptr<class CCollider> Collider)
{
	if (Collider.expired() || Collider.lock()->GetOwner().expired())
		return;
	std::shared_ptr<CGameObject> obj = std::dynamic_pointer_cast<CGameObject>(Collider.lock()->GetOwner().lock());
	if (!obj)
		return;

	mHitList.erase(obj->GetID());
}
