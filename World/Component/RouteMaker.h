#pragma once
#include "World\ActorComponent.h"
#include "../UnitData.h"

class CRouteMaker :
    public CActorComponent
{
public:
	CRouteMaker();
	virtual ~CRouteMaker();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

protected:
	std::weak_ptr<class CChapter> mChapter;
	std::weak_ptr<class CRoombase> mRoom;

	std::weak_ptr<class CGameObject> mTargetObj;
	FVector2 mTargetCoord = -FVector2::One;
	FVector2 mMyCoord;

	std::map<int, int> mDeadEnd;
	std::vector<int> mPoint;
	int mFocused = 0;

public:
	bool CanGetToTarget();
	FVector2 MakeRoute();
	FVector2 MakeRouteBFS();

	void SetRoom(std::weak_ptr<class CRoombase> room) { mRoom = room; mDeadEnd.clear(); }
	void SetChapter(std::weak_ptr<class CChapter> chptr) { mChapter = chptr; }

	void SetTarget(std::weak_ptr<class CGameObject> obj) { mTargetObj = obj; }

private:
	bool CheckStraightRoute(FVector2 to, FVector2 from);

	void RouteCheck(std::map<int, FRoute>& route);
	void RouteCheckBFS(std::list<FRoute>& route, std::map<int, int>& visited, const FVector2& target);

	int CoordDistance(FVector2 to, FVector2 from)
	{
		return abs(to.x - from.x) + abs(to.y - from.y);
	}
	FVector2 GetDir(FVector2 to, FVector2 from)
	{
		FVector2 dir = to - from;
		if (fabs(dir.x) > fabs(dir.y))
			dir.y = 0;
		else
			dir.x = 0;
		dir.Normalize();
		return dir;
	}
};

