#include "Walker.h"

#include "LogManager.h"

#include "World/ColliderSphere2D.h"
#include "World/SoundComponent.h"
#include "World/Animation2DComponent.h"

#include "../Chapter.h"
#include "../Base/Roombase.h"
#include "../Component/RigidBodyComponent.h"
#include "../Component/RouteMaker.h"

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

	CMonster::Update(DeltaTime);
}

void CWalker::Destory()
{
	CMonster::Destroy();
}

void CWalker::Reset(bool HardReset)
{
	CMonster::Reset(HardReset);
	mRouteMaker.lock()->SetRoom(mRoomOwner);
}

//피격
void CWalker::OnHurtOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
	std::shared_ptr<CActor> actor = Collider.lock()->GetOwner().lock();

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

	}
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
		std::shared_ptr<CUnitbase> unit = std::static_pointer_cast<CUnitbase>(gobj);
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

bool CWalker::UpdateNextMove()
{
	std::shared_ptr<CChapter> chptr = mChapter.lock();
	if (mRoomOwner.expired() || !chptr)
		return false;

	std::shared_ptr<CRoombase> room = mRoomOwner.lock();

	if(!room->CanGetToPlayerCharacter(GetWorldPos()))
		return false;

	FVector2 playerCoord = room->GetPlayerCoordInGrid();
	if (-FVector2::One == playerCoord)
		return false;

	FVector2 myCoord = room->WorldPosToCoord(GetWorldPos());

	FVector2 dir = playerCoord - myCoord;
	if (fabs(dir.x) > fabs(dir.y))
		dir.y = 0;
	else if (fabs(dir.x) < fabs(dir.y))
		dir.x = 0;
	dir.Normalize();
	FVector2 nextCoord = myCoord + dir;
	if (NextMoveSet(nextCoord))
		return true;
	else
	{
		if (fabs(dir.x) > fabs(dir.y))
		{
			nextCoord.y = 1;
			if (NextMoveSet(nextCoord))
				return true;

			nextCoord.y = -1;
			if (NextMoveSet(nextCoord))
				return true;
		}
		else
		{
			nextCoord.x = 1;
			if (NextMoveSet(nextCoord))
				return true;

			nextCoord.x = -1;
			if (NextMoveSet(nextCoord))
				return true;
		}
	}

	nextCoord = myCoord + -dir;
	if (NextMoveSet(nextCoord))
		return true;
	return false;
}

int routeCount = 0;
void CWalker::MakeRoute()
{
}

void CWalker::MakeRouteBFS()
{
	std::shared_ptr<CChapter> chptr = mChapter.lock();
	if (mRoomOwner.expired() || !chptr)
		return;

	std::shared_ptr<CRoombase> room = mRoomOwner.lock();
	if (!room->CanGetToPlayerCharacter(GetWorldPos()))
		return;

	FVector2 playerCoord = room->GetPlayerCoordInGrid();
	if (-FVector2::One == playerCoord)
		return;

	mRoute.clear();
	FVector2 myCoord = room->WorldPosToCoord(GetWorldPos());
	if (CoordDistance(playerCoord, myCoord) <= 1.5f)
	{
		mRoute.push_back(FVector2(playerCoord));
		return;
	}


	routeCount = 0;
	std::list<FRoute> routes;
	std::map<int, int> visited;
	for (int i = 0; i < 4; ++i)
	{
		FVector2 dest = myCoord + CChapter::FourDirections[i];
		if (!CheckCellValid(dest))
			continue;
		routes.push_back(FRoute(nullptr, dest));
	}
	visited[CChapter::Coord2Hash(myCoord)] = 1;
	CheckRouteBFS(routes, visited, playerCoord);
	if (routes.size() > 0)
	{
		FRoute route = routes.back();
		while (nullptr != route.Parent)
		{
			route = *route.Parent;
		}
		mRoute.push_back(route.Coord);
	}
}

bool CWalker::NextMoveSet(FVector2 Coord)
{
	std::shared_ptr<CChapter> chptr = mChapter.lock();
	if (mRoomOwner.expired() || !chptr)
		return false;

	std::shared_ptr<CRoombase> room = mRoomOwner.lock();
	if (room->CheckCell(Coord))
	{
		mNextMoveDir = room->CoordToWorldPos(Coord) - GetWorldPos();
		return true;
	}
	return false;
}

void CWalker::CheckRouteBFS(std::list<FRoute>& route, std::map<int, int>& visited, const FVector2& target)
{
	if (route.empty())
		return;

	FRoute check = route.front();
	route.pop_front();

	for (int i = 0; i < 4; ++i)
	{
		FVector2 dest = check.Coord + CChapter::FourDirections[i];
		int dHash = CChapter::Coord2Hash(dest);
		if (target == dest)
		{
			route.push_back(FRoute(&check, dest));
			return;
		}

		if (visited.find(dHash) != visited.end() || (nullptr != check.Parent && check.Parent->Coord == dest))
			continue;

		if (CheckCellValid(dest))
		{
			route.push_back(FRoute(&check, dest));
			visited[dHash] = 5;
		}
	}
	CheckRouteBFS(route, visited, target);
}

bool CWalker::CheckCellValid(const FVector2& Coord)
{
	if (mRoomOwner.expired())
		return false;

	std::shared_ptr<CRoombase> room = mRoomOwner.lock();
	return room->CheckCell(Coord);
}

int CWalker::CoordDistance(FVector2 to, FVector2 from)
{
	return static_cast<int>(abs(to.x - from.x) + abs(to.y - from.y));
}

void CWalker::RouteCountCheck()
{
	LOG_DEBUG(mName, "-", mID, " : ", routeCount);
}
