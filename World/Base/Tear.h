#pragma once
#include "GameObject.h"
#include "../../GameObjectStructs.h"

class CTear :
    public CGameObject
{
	GAMEOBJCLASS(CTear, 6)
public:
	CTear();
	virtual ~CTear();
	//눈물(객체(게임 오브젝트))
	//무엇이 필요한가요
	//1. 리지드 바디 컴포넌트
	//2. 애니메이션 컴포넌트
	//3. 콜라이더
	// 
	//1. 생성을 요청한 객체
	//2. 여러 효과들 내장해야함(독이나 화상같은)
	//2. 여러 기능들 내장해야함(충돌시 분열하는 등)

protected:
	bool mbIsDying = false;
	std::weak_ptr<class CRigidBodyComponent> mRigidBody;
	std::weak_ptr<class CColliderSphere2D> mHitBox;
	std::weak_ptr<class CSoundComponent> mSoundPlayer;

	std::weak_ptr<class CMeshComponent> mMesh;
	std::weak_ptr<class CAnimation2DComponent> mAnimator;

	//혹시라도 나중에 너무 무거워지고 느려지면
	//이 변수 먼저 RawPointer 로 바꾸기
	std::weak_ptr<class CTearShooter> mShooter; //눈물을 발사한 객체
	bool mIsOwnerCharacter = false;
	float mMovedDistance = 0.f;

	std::string TexName;
	std::weak_ptr<class CUnitbase> mTarget;
	FTearAttribute mTearAttribute;

	//나중에 어느정도 만든 후에
	//레이저를 다시 해보고싶을때 만들자
	//레이저는 box콜라이더를 사용해서 하면되기 때문에 다른 효과가 없을때 간단하지만
	//다른 효과가 있을때는 렌더가 어렵다.

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Destroy();

	virtual void Reset(bool HardReset = false);
public:
	//눈물을 설정할때 넣어줘야할것
	//플레이어, 시작위치, 진행방향, FUnitAttribute, 다른 모든 효과들
	void Set(bool IsPlayer, FVector3 StartPos, FVector2 Dir, FUnitAttribute Attribute
		, bool Homing = false, bool Piercing = false, bool Spectral = false, bool Orbiting = false
		, bool Boomerang = false, bool Shield = false, bool Bouncing = false, bool Sine = false, bool Square = false, bool Spiral = false
		, int SplitStack = 0, int SplitCount = 0);
	void Set(bool IsPlayer, FVector3 StartPos, FTearAttribute Attribute, std::weak_ptr<CTearShooter> Shooter, std::weak_ptr<class CSound> Sound);

	virtual void ReturnToChapter() override;

private:
	//눈물 궤적 함수들
	void BasicUpdate(float DeltaTime);
	//1. 공전
	void OrbitUpdate(float DeltaTime);
	//2. 호밍
	void HomingUpdate(float DeltaTime);
	//3. 부메랑
	void BoomerangUpdate(float DeltaTime);
	//4. s
	void SineUpdate(float DeltaTime);
	//5. 직각
	void SquareWaveUpdate(float DeltaTime);
	//6. 나선
	void SpiralUpdate(float DeltaTime);

	//눈물 파괴시
	void TearDying();

	//충돌시
	void OnCollision(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider);
public:
	FTearAttribute GetTearAttribute() { return mTearAttribute; }

	const bool GetIsHoming() const { return mTearAttribute.Homing; }
	const bool GetIsPiercing() const { return mTearAttribute.Piercing; }
	const bool GetIsSpectral() const { return mTearAttribute.Spectral; }
	const bool GetIsOrbiting() const { return mTearAttribute.Orbiting; }
	const bool GetIsBoomerang() const { return mTearAttribute.Boomerang; }
	const bool GetIsShield() const { return mTearAttribute.Shield; }
	const bool GetIsBouncing() const { return mTearAttribute.Bouncing; }

	void SetHoming(const bool Val) { mTearAttribute.Homing = Val; }
	void SetPiercing(const bool Val) { mTearAttribute.Piercing = Val; }
	void SetSpectral(const bool Val) { mTearAttribute.Spectral = Val; }
	void SetOrbiting(const bool Val) { mTearAttribute.Orbiting = Val; }
	void SetBoomerang(const bool Val) { mTearAttribute.Boomerang = Val; }
	void SetShield(const bool Val) { mTearAttribute.Shield = Val; }
	void SetBouncing(const bool Val) { mTearAttribute.Bouncing = Val; }

	std::weak_ptr<CGameObject> GetShooterOwner();
	EObjectType GetOwnerType();
};

