#include "Walker.h"

#include "LogManager.h"
#include "TimeManager.h"

#include "Asset/AssetManager.h"
#include "Asset/SoundManager.h"

#include "World/ColliderBox2D.h"
#include "World/ColliderSphere2D.h"
#include "World/SoundComponent.h"
#include "World/Animation2DComponent.h"

#include "../Manager/GameRuleManager.h"

#include "../Chapter.h"
#include "../Base/Roombase.h"
#include "../Component/RigidBodyComponent.h"
#include "../Component/RouteMaker.h"

#include "../Base/Tear.h"
#include "../Pickup/PickupBomb.h"

CWalker::CWalker()
	:CUnitbase(EObjectType::Monster)
{
}

CWalker::CWalker(const CWalker& src)
	:CUnitbase(src)
{
}

CWalker::CWalker(CWalker&& src) noexcept
	:CUnitbase(std::move(src))
{
}

CWalker::~CWalker()
{
}

bool CWalker::Init()
{
	if (!CUnitbase::Init())
		return false;

	std::shared_ptr<CColliderBox2D> hurt = mHurtBox.lock();
	hurt->SetCollisionProfile("Monster");

	mHitBox = CreateComponent<CColliderSphere2D>("Hit");
	if (mHitBox.expired())
		return false;

	std::shared_ptr<CColliderSphere2D> hit = mHitBox.lock();
	hit->SetCollisionProfile("ContactHit_Monster");
	hit->SetDebugDraw(true);
	hit->SetRadius(20.f);
	hit->SetBeginOverlapFunc(this, &CWalker::OnHitOverlaps);
	hit->SetEndOverlapFunc(this, &CWalker::ExitHitOverlaps);

	mRouteMaker = CreateComponent<CRouteMaker>("RouteMaker");
	if (mRouteMaker.expired())
		return false;

	mRouteMaker.lock()->SetRoom(mRoomOwner);
	mRouteMaker.lock()->SetChapter(mChapter);

	return true;
}

void CWalker::Update(float DeltaTime)
{
	std::shared_ptr rb = mRigidBody.lock();
	FVector3 velocity = rb->GetVelocity();
	if (FVector3::Zero != velocity)
	{
		FVector3 dir = velocity;
		dir.Normalize();

		rb->AddForce(dir * mAttribute.Speed * 5.f);
		if (fabs(dir.x) > fabs(dir.y))
		{
			if (dir.x > 0)
				SetBodyDirection(FVector2(1, 0));
			else
				SetBodyDirection(FVector2(-1, 0));
		}
		else
		{
			if (dir.y > 0)
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

	if (!mOverlaps.empty())
	{
		for (std::pair<int, std::weak_ptr<CCollider>> pair : mOverlaps)
		{
			std::shared_ptr<CUnitbase> unit = std::dynamic_pointer_cast<CUnitbase>(pair.second.lock()->GetOwner().lock());
			assert(unit && "공격할 수 없는 객체를 포함하였음");
			unit->GetHit(GetThisPtr<CGameObject>());
		}
	}

	CUnitbase::Update(DeltaTime);
}

void CWalker::Destory()
{
	CUnitbase::Destroy();
}

void CWalker::Reset(bool HardReset)
{
	mCurrentHP = mMaxHP;
	mRouteMaker.lock()->SetRoom(mRoomOwner);
}

void CWalker::SetEnable(bool Enable)
{
	CUnitbase::SetEnable(Enable);
	FTimerHandle handle = (mMumblingTimerID);
	CTimeManager::ClearTimer(handle);
	mMumblingTimerID = 0;
}

void CWalker::GetHit(std::weak_ptr<CGameObject> From)
{
	if (From.expired())
		return;
	if (mHurtSound.size() > 1)
	{
		int rand = CGameRuleManager::GetInst()->GenerateRandomI();
		mSoundPlayer.lock()->mSound = mHurtSound[rand % mHurtSound.size()].lock();
	}
	else if (!mHurtSound.empty())
	{
		mSoundPlayer.lock()->mSound = mHurtSound[0].lock();
	}
	mSoundPlayer.lock()->Play();

	std::shared_ptr<CGameObject> obj = From.lock();
	EObjectType t = obj->GetObjType();
	if (EObjectType::Tear == t)
	{
		std::shared_ptr<CTear> tear = std::dynamic_pointer_cast<CTear>(obj);
		t = tear->GetOwnerType();
		obj = tear->GetShooterOwner().lock();
	}

	if (EObjectType::PlayerCharacter == t || EObjectType::Monster == t)
	{
		std::shared_ptr<CUnitbase> unit = std::static_pointer_cast<CUnitbase>(obj);
		FUnitAttribute atrbt = unit->GetAttribute();
		mCurrentHP -= atrbt.Damage;
		LOG_DEBUG(GetName(), "유닛이 ", obj->GetActorTag(), " 에게 공격받았습니다\n 피해량: ", atrbt.Damage);
		if (mCurrentHP <= 0)
			ReturnToChapter(); //이 부분 나중에 수정하기 -> 애니메이션 같은거 출력하고 리턴하기 | 일단 비활성화는 해야함
	}
	else if (EObjectType::Obstacle == t)
	{

	}
	else if (EObjectType::Pickup == t)
	{
		std::shared_ptr<CPickupBomb> bomb = std::static_pointer_cast<CPickupBomb>(obj);
		if (!bomb)
			return;

		mCurrentHP -= 100;
		if (mCurrentHP <= 0)
			ReturnToChapter(); //이 부분 나중에 수정하기 -> 애니메이션 같은거 출력하고 리턴하기 | 일단 비활성화는 해야함
	}
}


//피격 -> 꼬여있던 것 수정함 | 기존에는 때리고 맞고 섞여있었는데 때리는걸로 통일 맞는 쪽이 맞을 수 있는지 확인하고 맞ㅇ므
void CWalker::OnHurtOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{/*
	std::shared_ptr<CGameObject> obj = std::dynamic_pointer_cast<CGameObject>(Collider.lock()->GetOwner().lock());
	if (!obj) 
		return;

	EObjectType t = obj->GetObjType();	
	if (t == EObjectType::Tear)
	{
		std::shared_ptr<CTear> tear = std::static_pointer_cast<CTear>(obj);
		std::shared_ptr<CGameObject> owner = tear->GetShooterOwner().lock();
		if (!owner)
			return;
		EObjectType onwerT = owner->GetObjType();
		if(onwerT == EObjectType::PlayerCharacter || onwerT == EObjectType::Monster)
			GetHit(std::dynamic_pointer_cast<CUnitbase>(owner));
	}
	else if (t == EObjectType::Obstacle)
	{

	}*/
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
		assert(false && "몬스터가 게임 객체가 아닌 무언가와 충돌함\n충돌체 프로파일상 불가능하고 생성되는 객체들 중에서도 충돌체를 가진 객체들은 모두 게임 객체여야함");
		return;
	}
	
	switch (gobj->GetObjType())
	{
	case EObjectType::PlayerCharacter: {
		std::shared_ptr<CUnitbase> unit = std::static_pointer_cast<CUnitbase>(gobj);
		if (!unit)
		{
			LOG_DEBUG(GetName(), ":", "충돌체에 유닛이 없습니다. 충돌 프로파일을 수정하세요");
			return;
		}
		mOverlaps.insert(std::make_pair(gobj->GetID(), Collider));
		//unit->GetHit(GetThisPtr<CUnitbase>());
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
	//std::shared_ptr<CGameObject> gobj = std::dynamic_pointer_cast<CGameObject>(Collider.lock()->GetOwner().lock());
	//if (gobj->GetID() == mTarget.lock()->GetID())
	//	mTarget.reset();

	std::shared_ptr<CGameObject> gobj = std::dynamic_pointer_cast<CGameObject>(Collider.lock()->GetOwner().lock());
	if (!gobj)
	{
		assert(false && "몬스터가 게임 객체가 아닌 무언가와 충돌함\n충돌체 프로파일상 불가능하고 생성되는 객체들 중에서도 충돌체를 가진 객체들은 모두 게임 객체여야함");
		return;
	}
	if (mOverlaps.find(gobj->GetID()) != mOverlaps.end())
	{
		mOverlaps.erase(gobj->GetID());
	}
}

void CWalker::SetMumblingSound(const std::string& soundName, float Time, bool loop)
{
	std::shared_ptr<CSoundManager> mgr = CAssetManager::GetInst()->GetSubManager<CSoundManager>(EAssetType::Sound);
	std::weak_ptr<CSound> sound = mgr->FindSound(soundName);
	if (sound.expired())
		return;

	mMumblingSound = sound;
	mMumblingTimerID = CTimeManager::SetTimer(Time, loop, this, &CWalker::PlayerMumbling).GetID();
}

void CWalker::SetMumblingSound(float Time, bool loop)
{
	mMumblingTimerID = CTimeManager::SetTimer(Time, loop, this, &CWalker::PlayerMumbling).GetID();
}

void CWalker::PlayerMumbling()
{
	mSoundPlayer.lock()->mSound = mMumblingSound.lock();
	mSoundPlayer.lock()->Play();
}
