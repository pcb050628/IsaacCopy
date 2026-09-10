#pragma once
#include "../Base/Unitbase.h"
#include "../Base/Monster.h"

class CWalker :
	public CUnitbase, public CMonster
{
public:
	CWalker();
	CWalker(const CWalker& src);
	CWalker(CWalker&& src) noexcept;
	virtual ~CWalker();

protected:
	std::weak_ptr<class CSound> mMumblingSound;
	uint64_t mMumblingTimerID;
	std::weak_ptr<class CRouteMaker> mRouteMaker;
	std::weak_ptr<CUnitbase> mTarget;

	FVector3 mNextMoveDir = FVector3::Zero;
	std::list<FVector2> mRoute;
	float mMoveSpeed = 100.f;
	float mSpeedLimit = 500.f;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime) = 0;
	virtual void Destory();
	virtual void Dead() = 0;

	virtual void Reset(bool HardReset = false) override;
	virtual void SetEnable(bool Enable) override;

	virtual void GetHit(std::weak_ptr<CGameObject> From);

protected:
	virtual void OnHurtOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
	virtual void ExitHurtOverlaps(std::weak_ptr<CCollider> Collider);

	virtual void OnHitOverlaps(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
	virtual void ExitHitOverlaps(std::weak_ptr<CCollider> Collider);

	void SetMumblingSound(const std::string& soundName, float Time, bool loop = false);
	void SetMumblingSound(float Time, bool loop = false);
	void PlayerMumbling();

};

