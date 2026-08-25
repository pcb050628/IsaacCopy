#include "RouteMaker.h"

#include "TimeManager.h"
#include "LogManager.h"

#include "../Chapter.h"
#include "../Base/Roombase.h"
#include "../Base/RoomMember.h"

CRouteMaker::CRouteMaker()
{
}

CRouteMaker::~CRouteMaker()
{
}

bool CRouteMaker::Init()
{
	mPoint.resize(100);

	//소유한 액터가 RoomMember를 상속받지 않앗을 경우 실패한다
	if (!std::dynamic_pointer_cast<CRoomMember>(mOwner.lock()))
	{
		return false;
	}

	return true;
}

void CRouteMaker::Update(float DeltaTime)
{
	CActorComponent::Update(DeltaTime);
}

bool CRouteMaker::CanGetToTarget()
{
	std::shared_ptr<CRoombase> room = mRoom.lock();
	FVector3 pos = mOwner.lock()->GetWorldPos();
	if (!room->CanGetToPlayerCharacter(pos))
		return false;

	FVector2 playerCoord = room->GetPlayerCoordInGrid();
	if (-FVector2::One == playerCoord)
		return false;

	return true;
}

FVector2 CRouteMaker::MakeRoute()
{
	std::shared_ptr<CChapter> chptr = mChapter.lock();
	if (mRoom.expired() || !chptr || !CanGetToTarget())
	{
		std::shared_ptr<CRoomMember> owner = std::dynamic_pointer_cast<CRoomMember>(mOwner.lock());
		if (!owner)
			return -FVector2::One;

		mRoom = owner->GetRoom();
		mChapter = owner->GetChater();
	}

	std::shared_ptr<CRoombase> room = mRoom.lock();
	FVector3 pos = mOwner.lock()->GetWorldPos();
	if (mTargetObj.expired())
		mTargetCoord = room->GetPlayerCoordInGrid();
	else
		mTargetCoord = room->WorldPosToCoord(mTargetObj.lock()->GetWorldPos());

	mMyCoord = room->WorldPosToCoord(pos);
	if (CoordDistance(mTargetCoord, mMyCoord) <= 1.5f)
	{
		return mTargetCoord;
	}
	if (CheckStraightRoute(mTargetCoord, mMyCoord))
	{
		return mMyCoord + GetDir(mTargetCoord, mMyCoord);
	}

	mPoint.clear();	mPoint.resize(10);
	mDeadEnd.clear();

	std::map<int, FRoute> routes; //좌표마다 유일성을 주기 위해서 맵을 사용함 / unordered map 아 아닌 map을 사용한 이유는 순회를 해야하기 때문임
	std::map<int, int> deadEnd;
	for (int i = 0; i < 4; ++i)
	{
		FVector2 dest = mMyCoord + CChapter::FourDirections[i];
		if (!room->CheckCell(dest))
			continue;
		routes.insert(std::make_pair(CChapter::Coord2Hash(dest), FRoute(nullptr, dest)));
	}
	RouteCheck(routes);
	if (routes.find(-1) != routes.end())
	{
		FRoute route = routes[-1];
		while (nullptr != route.Parent)
		{
			route = *route.Parent;
		}
		return route.Coord;
	}

    return -FVector2::One;
}

FVector2 CRouteMaker::MakeRouteBFS()
{
	std::shared_ptr<CChapter> chptr = mChapter.lock();
	if (mRoom.expired() || !chptr || !CanGetToTarget())
	{
		std::shared_ptr<CRoomMember> owner = std::dynamic_pointer_cast<CRoomMember>(mOwner.lock());
		if (!owner)
			return -FVector2::One;

		mRoom = owner->GetRoom();
		mChapter = owner->GetChater();
	}

	std::shared_ptr<CRoombase> room = mRoom.lock();
	FVector3 pos = mOwner.lock()->GetWorldPos();
	if (mTargetObj.expired())
		mTargetCoord = room->GetPlayerCoordInGrid();
	else
		mTargetCoord = room->WorldPosToCoord(mTargetObj.lock()->GetWorldPos());
	mMyCoord = room->WorldPosToCoord(pos);

	if (CoordDistance(mTargetCoord, mMyCoord) <= 1.5f)
	{
		return mTargetCoord;
	}
	if (CheckStraightRoute(mTargetCoord, mMyCoord))
	{
		return mTargetCoord + GetDir(mTargetCoord, mMyCoord);
	}

	std::list<FRoute> routes;
	std::map<int, int> visited;
	for (int i = 0; i < 4; ++i)
	{
		FVector2 dest = mMyCoord + CChapter::FourDirections[i];
		if (!room->CheckCell(dest))
			continue;
		routes.push_back(FRoute(nullptr, dest));
	}
	visited[CChapter::Coord2Hash(mMyCoord)] = 1;
	RouteCheckBFS(routes, visited, mTargetCoord);
	if (routes.size() > 0)
	{
		FRoute route = routes.back();
		while (nullptr != route.Parent)
		{
			route = *route.Parent;
		}
		return route.Coord;
	}
	return -FVector2::One;
}

bool CRouteMaker::CheckStraightRoute(FVector2 to, FVector2 from)
{
	std::shared_ptr<CChapter> chptr = mChapter.lock();
	if (mRoom.expired() || !chptr)
	{
		std::shared_ptr<CRoomMember> owner = std::dynamic_pointer_cast<CRoomMember>(mOwner.lock());
		if (!owner)
			return false;

		mRoom = owner->GetRoom();
		mChapter = owner->GetChater();
	}

	std::shared_ptr<CRoombase> room = mRoom.lock();
	FVector3 pos = mOwner.lock()->GetWorldPos();
	if (!room->CanGetToPlayerCharacter(pos))
		return false;
	if (to.x != from.x && to.y != from.y)
		return false;

	FVector2 dir = GetDir(to, from);
	while (to != from)
	{
		from += dir;
		if (!room->CheckCell(from))
			return false;
	}
	return true;
}

void CRouteMaker::RouteCheck(std::map<int, FRoute>& route)
{
	bool flag = false;

	if (route.size() > mPoint.size())
		mPoint.resize(route.size());

	std::map<int, FRoute>::iterator iter = route.begin();
	std::map<int, FRoute>::iterator iterEnd = route.end();
	int i = 0;
	for (; iter != iterEnd; ++i) //포함되어있는 루트들 중 가장 빨라 보이는 길을 우선 탐색함
	{
		FRoute val = iter->second;
		if (val.Coord == mTargetCoord)
			return;
		else if (mDeadEnd.find(CChapter::Coord2Hash(val.Coord)) != mDeadEnd.end())
		{
			//만약 막힌길인 경우에 해당 해당 길목을 삭제하고 이전 길이 있다면 해당 길을 다시 등록한다.
			if (nullptr != val.Parent)
			{
				int pH = CChapter::Coord2Hash(val.Parent->Coord);
				if (route.find(pH) == route.end() && route[pH].Depth <= val.Parent->Depth)
				{ }
				else
				{
					route[pH] = FRoute(*val.Parent);
				}
			}
			iter = route.erase(iter);
			continue;
		}

		mPoint[i] = CoordDistance(mTargetCoord, val.Coord) + val.Depth; //뎊스는 이동 거리임
		++iter;
	}
	if (route.empty())
		return;

	if (mFocused >= route.size())
		mFocused = 0;
	for(int j = 0; j < route.size(); ++j)
	{
		if (mPoint[j] + 3 < mPoint[mFocused])
			mFocused = j;
	}

	std::map<int, FRoute>::iterator current = std::next(route.begin(), mFocused);
	FRoute next = current->second;
	route.erase(current);

	for (int i = 0; i < 4; ++i) //현재 선택된 루트에서 진행 가능한 루트들을 전부 기록함
	{
		FVector2 dest = next.Coord + CChapter::FourDirections[i];
		if (mTargetCoord == dest)
		{
			route[-1] = FRoute(&next, dest);
			return;
		}

		int dHash = CChapter::Coord2Hash(dest);
		if (!mRoom.lock()->CheckCell(dest) || mDeadEnd.find(dHash) != mDeadEnd.end() || (nullptr != next.Parent && next.Parent->Coord == dest))
			continue;

		flag = true;
		if (route.find(dHash) != route.end() && route[dHash].Depth > next.Depth)
		{
			route[dHash] = FRoute(&next, dest);
		}
		else
			route.insert(std::make_pair(dHash, FRoute(&next, dest)));
	}
	
	if (!flag) //이동 가능한 방향이 없는 경우 (되돌아가는 경우도 갈 수 없는 길로 판단함) 막힌길로 표시
	{
		int pH = CChapter::Coord2Hash(next.Coord);
		mDeadEnd[pH] = 5;
		if (nullptr != next.Parent && route.find(pH) != route.end() && route[pH].Depth > next.Parent->Depth)
		{			
			route[pH] = FRoute(*next.Parent);
		}
	}
	RouteCheck(route);
}

void CRouteMaker::RouteCheckBFS(std::list<FRoute>& route, std::map<int, int>& visited, const FVector2& target)
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

		if (mRoom.lock()->CheckCell(dest))
		{
			route.push_back(FRoute(&check, dest));
			visited[dHash] = 5;
		}
	}
	RouteCheckBFS(route, visited, target);
}