#include "TearShooter.h"
#include "TimeManager.h"

#include "World/World.h"
#include "World/Collider.h"

#include "../Chapter.h"


CTearShooter::CTearShooter()
{
}

CTearShooter::~CTearShooter()
{
}

bool CTearShooter::Init()
{
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
	//발사 가능한지 확인하기
	//
	std::shared_ptr<CChapter> chptr = std::dynamic_pointer_cast<CChapter>(mWorld.lock());
	
}

void CTearShooter::UpdateUnitAttributeData(const bool Synchronize, FUnitAttribute Attribute)
{
	mIsSynchronized = Synchronize;
	mUnitAttribute = Attribute;
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
	std::list<FVector2>::iterator iter = mFirePoints.begin();
	std::list<FVector2>::iterator iterEnd = mFirePoints.end();
	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Point)
		{
			mFirePoints.erase(iter);
			return;
		}
	}
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
