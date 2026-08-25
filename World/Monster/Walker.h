#pragma once
#include "../Base/Monster.h"

class CWalker :
	public CMonster
{
public:
	CWalker();
	CWalker(const CWalker& src);
	CWalker(CWalker&& src) noexcept;
	virtual ~CWalker();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime) = 0;
	virtual void Destory();
	virtual void Dead() = 0;

	virtual void Reset(bool HardReset = false);

protected:
	virtual void OnHurtOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
	virtual void ExitHurtOverlaps(std::weak_ptr<CCollider> Collider);

	virtual void OnHitOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
	virtual void ExitHitOverlaps(std::weak_ptr<CCollider> Collider);

	bool UpdateNextMove();
	void MakeRoute();
	void MakeRouteBFS();
private:
	bool NextMoveSet(FVector2 Coord);
	void CheckRouteBFS(std::list<FRoute>& routes, std::map<int, int>& visited, const FVector2& target);
	bool CheckCellValid(const FVector2& Coord);
	int CoordDistance(FVector2 a, FVector2 b);

	void RouteCountCheck();

protected:
	std::weak_ptr<class CRouteMaker> mRouteMaker;
	std::weak_ptr<CUnitbase> mTarget;

	FVector3 mNextMoveDir = FVector3::Zero;
	std::list<FVector2> mRoute;
	float mMoveSpeed = 100.f;
	float mSpeedLimit = 500.f;
};

