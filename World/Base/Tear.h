#pragma once
#include "GameObject.h"
#include "../UnitData.h"

class CTear :
    public CGameObject
{
	GAMECLASS(CTear, 6)
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
	std::weak_ptr<class CRigidBodyComponent> mRigidBody;
	std::weak_ptr<class CColliderSphere2D> mHitBox;

	std::weak_ptr<class CMeshComponent> mMesh;
	std::weak_ptr<class CAnimation2DComponent> mAnimator;

	FUnitAttribute mUnitOwnerAttribute;
	float mMovedDistance = 0.f;

	bool mIsOwnerCharacter = false;

	std::string TexName;
	std::weak_ptr<class CUnitbase> mTarget;
	FVector2 mDirection;

	bool mHoming = false; //적 추적
	bool mPiercing = false; //관통(유닛만)
	bool mSpectral = false; //관통(장애물만)
	bool mOrbiting = false; //특정 객체 공전
	bool mBoomerang = false; //부메랑(절반 갔다가 캐릭터 방향으로 돌아옴)
	bool mShield = false; //눈물 파괴
	bool mBouncing = false; //튕김
	int mSplitStack = 0; //분열 횟수
	int mSplitCount = 0; //분열되는 갯수

	bool mSine = false;
	bool mSquareWave = false;
	bool mSpiral = false;

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

public:
	const bool GetIsHoming() const { return mHoming; }
	const bool GetIsPiercing() const { return mPiercing; }
	const bool GetIsSpectral() const { return mSpectral; }
	const bool GetIsOrbiting() const { return mOrbiting; }
	const bool GetIsBoomerang() const { return mBoomerang; }
	const bool GetIsShield() const { return mShield; }
	const bool GetIsBouncing() const { return mBouncing; }

	void SetHoming(const bool Val) { mHoming = Val; }
	void SetPiercing(const bool Val) { mPiercing = Val; }
	void SetSpectral(const bool Val) { mSpectral = Val; }
	void SetOrbiting(const bool Val) { mOrbiting = Val; }
	void SetBoomerang(const bool Val) { mBoomerang = Val; }
	void SetShield(const bool Val) { mShield = Val; }
	void SetBouncing(const bool Val) { mBouncing = Val; }
};

