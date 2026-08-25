#include "TearShooter.h"
#include "TimeManager.h"

#include "Asset/AssetManager.h"
#include "Asset/SoundManager.h"

#include "World/World.h"
#include "World/Collider.h"
#include "World/MeshComponent.h"
#include "World/SoundComponent.h"

#include "../Chapter.h"


CTearShooter::CTearShooter()
{
}

CTearShooter::~CTearShooter()
{
}

bool CTearShooter::Init()
{
	std::weak_ptr<CUnitbase> obj = std::dynamic_pointer_cast<CUnitbase>(mOwner.lock());
	if (obj.expired())
		return false;

	if (EObjectType::PlayerCharacter == obj.lock()->GetObjType())
		mbIsOwnerPlayer = true;

	mOwnerUnit = obj;
	mOwnerUnitHead = mOwnerUnit.lock()->GetHeadComp();
	mOwnerUnitBody = mOwnerUnit.lock()->GetBodyComp();
	if (mOwnerUnitHead.expired() || mOwnerUnitBody.expired())
		return false;

	mOwnerObjType = mOwnerUnit.lock()->GetObjType();

	mSounds.resize(2);

	std::shared_ptr<CSoundManager> soundMgr = CAssetManager::GetInst()->GetSubManager<CSoundManager>(EAssetType::Sound);
	mSounds[0] = soundMgr->FindSound("Character_tear_fire_1");
	mSounds[1] = soundMgr->FindSound("Character_tear_fire_2");

	return true;
}

void CTearShooter::Update(float DeltaTime)
{
	CActorComponent::Update(DeltaTime);
}

void CTearShooter::Destroy()
{
	CActorComponent::Destroy();
}

void CTearShooter::Fire() 
{
	if (static_cast<float>(CTimeManager::GetTime()) - mLastFireTime < mUnitAttribute.ShotTerm)
		return;

	mLastFireTime = static_cast<float>(CTimeManager::GetTime());

	if (!mOwnerUnit.expired()) //오, 왼, 위, 아래
	{
		mTearAttribute.Direction = mOwnerUnit.lock()->GetHeadDirection();
	}

	std::shared_ptr<CChapter> chptr = std::dynamic_pointer_cast<CChapter>(mWorld.lock());


	if (mbIsSynchronized)
	{
		for (FVector2 startPos : mFirePoints)
		{
			std::shared_ptr<CTear> tear = chptr->GetTear().lock();
			if (!tear)
			{
				assert("ERROR: TEAR IS EMPTY");
				return;
			}
			tear->Set(mbIsOwnerPlayer, FVector3(startPos.x, startPos.y, 0), mTearAttribute, GetThisPtr<CTearShooter>(), mSounds[mFirePointIndex % 2]);
		}
	}
	else
	{
		std::shared_ptr<CTear> tear = chptr->GetTear().lock();
		if (!tear)
		{
			assert("ERROR: TEAR IS EMPTY");
			return;
		}
		tear->Set(mbIsOwnerPlayer, mOwnerUnitHead.lock()->GetWorldPos() + FirePointCalculate(mFirePointIndex), mTearAttribute, GetThisPtr<CTearShooter>(), mSounds[mFirePointIndex % 2]);
		mFirePointIndex = (mFirePointIndex + 1) % mFirePoints.size();
	}
}

void CTearShooter::Fire(FVector3 firePoint, bool IsSet)
{
	if (static_cast<float>(CTimeManager::GetTime()) - mLastFireTime < mUnitAttribute.ShotTerm)
		return;

	mLastFireTime = static_cast<float>(CTimeManager::GetTime());
	std::shared_ptr<CChapter> chptr = std::dynamic_pointer_cast<CChapter>(mWorld.lock());

	if (IsSet)
	{
		std::shared_ptr<CTear> tear = chptr->GetTear().lock();
		if (!tear)
		{
			assert("ERROR: TEAR IS EMPTY");
			return;
		}
		tear->Set(mbIsOwnerPlayer, firePoint, mTearAttribute, GetThisPtr<CTearShooter>(), mSounds[mFirePointIndex % 2]);
	}
	else
	{
		if (!mOwnerUnit.expired()) //오, 왼, 위, 아래
		{
			mTearAttribute.Direction = mOwnerUnit.lock()->GetHeadDirection();
		}

		if (mbIsSynchronized)
		{
			for (FVector2 startPos : mFirePoints)
			{
				std::shared_ptr<CTear> tear = chptr->GetTear().lock();
				if (!tear)
				{
					assert("ERROR: TEAR IS EMPTY");
					return;
				}
				tear->Set(mbIsOwnerPlayer, FVector3(startPos.x, startPos.y, 0), mTearAttribute, GetThisPtr<CTearShooter>(), mSounds[mFirePointIndex % 2]);
			}
		}
		else
		{
			std::shared_ptr<CTear> tear = chptr->GetTear().lock();
			if (!tear)
			{
				assert("ERROR: TEAR IS EMPTY");
				return;
			}
			tear->Set(mbIsOwnerPlayer, firePoint + mOwnerUnitHead.lock()->GetWorldPos() + FirePointCalculate(mFirePointIndex), mTearAttribute, GetThisPtr<CTearShooter>(), mSounds[mFirePointIndex % 2]);
		}
		mFirePointIndex = (mFirePointIndex + 1) % mFirePoints.size();
	}
}

bool CTearShooter::FireWithVelocityOffset(FVector2 vOffset)
{
	if (static_cast<float>(CTimeManager::GetTime()) - mLastFireTime < mUnitAttribute.ShotTerm)
		return false;

	mLastFireTime = static_cast<float>(CTimeManager::GetTime());

	if (!mOwnerUnit.expired()) //오, 왼, 위, 아래
	{
		mTearAttribute.Direction = mOwnerUnit.lock()->GetHeadDirection() + vOffset / 2;
	}

	std::shared_ptr<CChapter> chptr = std::dynamic_pointer_cast<CChapter>(mWorld.lock());

	if (mbIsSynchronized)
	{
		for (FVector2 startPos : mFirePoints)
		{
			std::shared_ptr<CTear> tear = chptr->GetTear().lock();
			if (!tear)
			{
				assert(false && "ERROR: TEAR IS EMPTY");
				return false;
			}
			tear->Set(mbIsOwnerPlayer, FVector3(startPos.x, startPos.y, 0), mTearAttribute, GetThisPtr<CTearShooter>(), mSounds[mFirePointIndex % 2]);
		}
	}
	else
	{
		std::shared_ptr<CTear> tear = chptr->GetTear().lock();
		if (!tear)
		{
			assert(false && "ERROR: TEAR IS EMPTY");
			return false;
		}
		tear->Set(mbIsOwnerPlayer, mOwnerUnitHead.lock()->GetWorldPos() + FirePointCalculate(mFirePointIndex), mTearAttribute, GetThisPtr<CTearShooter>(), mSounds[mFirePointIndex % 2]);
		mFirePointIndex = (mFirePointIndex + 1) % mFirePoints.size();
	}

	return true;
}

void CTearShooter::UpdateUnitAttributeData(const bool Synchronize, FUnitAttribute Attribute)
{
	mbIsSynchronized = Synchronize;
	mUnitAttribute = Attribute;
	
	mTearAttribute.Damage = mUnitAttribute.Damage;
	mTearAttribute.Speed = mUnitAttribute.ShotSpeed;
	mTearAttribute.Range = mUnitAttribute.Range;
	mTearAttribute.Height = mUnitAttribute.Height;
}

void CTearShooter::UpdateTearAttributeData(FTearAttribute Attribute)
{
	mTearAttribute = Attribute;
}

void CTearShooter::UpdateAttributeData(FUnitAttribute Unit, FTearAttribute Tear)
{
	mUnitAttribute = Unit;
	mTearAttribute = Tear;
}

void CTearShooter::AddFirePoint(const FVector2& Point)
{
	mFirePoints.push_back(Point);
}

void CTearShooter::RemoveFirePoint(const FVector2& Point)
{
	std::vector<FVector2>::iterator iter = mFirePoints.begin();
	std::vector<FVector2>::iterator iterEnd = mFirePoints.end();
	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Point)
		{
			mFirePoints.erase(iter);
			return;
		}
	}
}

FVector3 CTearShooter::FirePointCalculate(int Index)
{
	if (Index < 0 || Index >= mFirePoints.size())
	{
		assert("ERROR: INDEX POINT NULL");
	}

	FVector2 origin = mFirePoints[Index];
	FVector2 headDir = mOwnerUnit.lock()->GetHeadDirection();
	if(headDir == FVector2(0, -1))
		return FVector3(origin.x, origin.y, 0);
	else
	{
		if (CChapter::FourDirections[0] == headDir)
		{
			int y = origin.x;
			int x = -origin.y;
			return FVector3(x, y, 0);
		}
		else if (CChapter::FourDirections[1] == headDir)
		{
			int y = origin.x;
			int x = origin.y;
			return FVector3(x, y, 0);
		}
		else if (CChapter::FourDirections[2] == headDir)
		{
			return FVector3(-origin.x, -origin.y, 0);
		}
	}
	return FVector3();
}

void CTearShooter::OnDestroy(const FVector3& WorldPos)
{
	for (std::pair<int, std::function<void(const FVector3&)>> pair : mOnDestroyFunc)
	{
		pair.second(WorldPos);
	}
}

void CTearShooter::OnCollision(const FVector3& Point, const FVector3& Normal, std::weak_ptr<CCollider> Collider)
{
	for (std::pair<int, std::function<void(const FVector3&, const FVector3&, std::weak_ptr<CCollider>)>> pair : mOnCollisionFunc)
	{
		pair.second(Point, Normal, Collider);
	}
}
