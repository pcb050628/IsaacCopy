#include "Walker.h"

#include "LogManager.h"

#include "World/ColliderSphere2D.h"
#include "World/Animation2DComponent.h"

#include "../Chapter.h"
#include "../Base/Roombase.h"
#include "../Component/RigidBodyComponent.h"

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

void CWalker::GetHit(std::weak_ptr<CGameObject> From)
{
	if (From.expired()) 
		return;

	std::shared_ptr<CGameObject> obj = From.lock();
	EObjectType t = obj->GetObjType();
	if (EObjectType::PlayerCharacter == t || EObjectType::Monster == t)
	{
		std::shared_ptr<CUnitbase> unit = std::static_pointer_cast<CUnitbase>(obj);
		FUnitAttribute atrbt = unit->GetAttribute();
		mCurrentHP -= atrbt.Damage;
		LOG_DEBUG(GetName(), "유닛이 ", From.lock()->GetActorTag(), " 에게 공격받았습니다\n 피해량: ", atrbt.Damage);
		if (mCurrentHP <= 0)
			ReturnToChapter(); //이 부분 나중에 수정하기 -> 애니메이션 같은거 출력하고 리턴하기 | 일단 비활성화는 해야함
	}
	else if (EObjectType::Obstacle == t)
	{

	}
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
	std::shared_ptr<CChapter> chptr = mChapter.lock();
	if (mRoomOwner.expired() || !chptr)
		return;

	std::shared_ptr<CRoombase> room = mRoomOwner.lock();
	if (!room->CanGetToPlayerCharacter(GetWorldPos()))
		return;

	FVector2 playerCoord = room->GetPlayerCoordInGrid();
	if (-FVector2::One == playerCoord)
		return;

	routeCount = 0;
	FVector2 myCoord = room->WorldPosToCoord(GetWorldPos());

	FVector2 dist = playerCoord - myCoord;
	if (dist.Length() <= 1)
	{
		mRoute.clear();
		mRoute.push_back(playerCoord);
		return;
	}

	int focus = 0;
	bool check = false;
	std::vector<std::pair<int, std::list<FVector2>>> routes;
	routes.resize(4);
	for (int i = 0; i < 4; ++i)
	{
		FVector2 start = myCoord + CChapter::FourDirections[i];
		if (!room->CheckCell(start))
		{
			routes[i].first = 100;
			continue;
		}
		routes[i].first = CoordDistance(playerCoord, start);
		routes[i].second.push_back(start);
		focus = i;
	}

	mRoute.clear();
	CheckRoute(playerCoord, focus, routes, check);
	if (!check)
	{
		return;
	}
	for (std::pair<int, std::list<FVector2>> route : routes)
	{
		if (-1 == route.first)
		{
			mRoute = std::move(route.second);
			break;
		}
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

void CWalker::CheckRoute(const FVector2& Target, int& focus, std::vector<std::pair<int, std::list<FVector2>>>& routes, bool& Complete)
{
	if (Complete)
		return;

	int count = 0; ++routeCount;
	for (int i = 0; i < 4; i++)
	{
		if (-1 == routes[i].first)
			return;

		if (100 == routes[i].first)
		{
			++count;
			continue;
		}

		routes[i].first = CoordDistance(Target, routes[i].second.back()) + routes[i].second.size();
		if (routes[focus].first > routes[i].first)
		{
			CheckRoute(Target, i, routes, Complete);
		}
	}

	if (count > 3)
		return;

	FVector2 current = routes[focus].second.back();
	FVector2 dir = Target - current;
	if (fabs(dir.x) > fabs(dir.y))
		dir.y = 0;
	else
		dir.x = 0;
	dir.Normalize();
	FVector2 next = current + dir;
	if (routes[focus].second.size() > 2)
	{
		auto iter = routes[focus].second.end();
		--iter;	--iter;
		if (next == *iter)
			return;
	}

	if (CheckCellValid(next))
	{
		routes[focus].second.push_back(next);
		if (Target == next)
		{
			Complete = true;
			routes[focus].first = -1;
			return;
		}
		else
		{
			CheckRoute(Target, focus, routes, Complete);
			return;
		}
	}
	else
	{
		FVector2 ndir = Target - current;
		if (fabs(dir.x) > fabs(dir.y))
		{
			ndir.x = 0;
			if (0 == ndir.y)
				ndir.y = -1;
			else
				ndir.y = FVector2(Target - current).y;
		}
		else
		{
			ndir.y = 0;
			if (0 == ndir.x)
				ndir.x = -1;
			else
				ndir.x = FVector2(Target - current).x;
		}
		ndir.Normalize();
		
		next = current + ndir;
		if (CheckCellValid(next))
		{
			routes[focus].second.push_back(next);
			CheckRoute(Target, focus, routes, Complete);
			return;
		}

		next = current + -ndir;
		if (CheckCellValid(next))
		{
			routes[focus].second.push_back(next);
			CheckRoute(Target, focus, routes, Complete);
			return;
		}

		next = current + -dir;
		if (CheckCellValid(next))
		{
			routes[focus].second.push_back(next);
			CheckRoute(Target, focus, routes, Complete);
			return;
		}

		routes[focus].first = 100;
	}
}

bool CWalker::CheckCellValid(const FVector2& Coord)
{
	std::shared_ptr<CChapter> chptr = mChapter.lock();
	if (mRoomOwner.expired() || !chptr)
		return false;

	std::shared_ptr<CRoombase> room = mRoomOwner.lock();
	if (room->CheckCell(Coord))
		return true;
	return false;
}

int CWalker::CoordDistance(FVector2 to, FVector2 from)
{
	return abs(to.x - from.x) + abs(to.y - from.y);
}

void CWalker::RouteCountCheck()
{
	LOG_DEBUG(mName, "-", mID, " : ", routeCount);
}
