#include "Tear.h"

#include "World/MeshComponent.h"
#include "World/ColliderSphere2D.h"
#include "World/Animation2DComponent.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"
#include "../Component/RigidBodyComponent.h"

REGISTER_GAMECLASS(CTear, "Tear", EObjectType::Tear)

CTear::CTear()
	:CGameObject(EObjectType::Tear)
{
}

CTear::~CTear()
{
}

bool CTear::Init()
{
	if (!CGameObject::Init())
		return false;

	mRigidBody = CreateComponent<CRigidBodyComponent>("Root");
	mHitBox = CreateComponent<CColliderSphere2D>("Hit");
	mMesh = CreateComponent<CMeshComponent>("Mesh");
	mAnimator = CreateComponent<CAnimation2DComponent>("Animator");
	if (mRigidBody.expired() || mHitBox.expired() || mMesh.expired() || mAnimator.expired())
		return false;

	//히트 박스 스케일(radius) 조정
	//메시 스케일 조정
	//애니메이션 불러오고

	return true;
}

void CTear::Update(float DeltaTime)
{
	//수명측정 방법
	//보니까 사거리가 6.5여도 속도가 0.1이면 한칸도 안가서 멈추더라
	//느리게 꾸역꾸역 사거리 만큼 가는게 아니라 시간누적해서 가는거

	if (mHoming)
	{
		HomingUpdate(DeltaTime);
	}
	if (mOrbiting)
	{
		OrbitUpdate(DeltaTime);
	}
	if (mBoomerang)
	{
		BoomerangUpdate(DeltaTime);
	}

	//그럼 시간누적은 어떻게 할까
	//mMoveDistance = DeltaTime * speed * RoomCellSize(85.f);
	//if(mMoveDistance > mUnitOwnerAttribute.Range) Destroy();
	mMovedDistance += DeltaTime * mUnitOwnerAttribute.ShotSpeed * 85.f;
	if (mMovedDistance >= mUnitOwnerAttribute.Range)
	{
		Destroy();
	}

	CGameObject::Update(DeltaTime);
}

void CTear::Destroy()
{
	//여기서 파괴시 호출
	//충돌시와 중첩되지않게 만들것

	CGameObject::Destroy();
}

void CTear::Reset(bool HardReset)
{
	mIsOwnerCharacter = false;
	mDirection = FVector2::Zero;
	mUnitOwnerAttribute = FUnitAttribute();

	mHoming = false;
	mPiercing = false;
	mSpectral = false;
	mOrbiting = false;
	mBoomerang = false;
	mShield = false;
	mBouncing = false;
	mSine = false;
	mSquareWave = false;
	mSpiral = false;

	mSplitStack = 0;
	mSplitCount = 0;

	TexName = "Tear_Default";
}

//생각해보니까 텍스쳐도 골라줘야함
//눈물이 너무 여러가지라서
void CTear::Set(bool IsPlayer, FVector3 StartPos, FVector2 Dir, FUnitAttribute Attribute, bool Homing, bool Piercing, bool Spectral, bool Orbiting, bool Boomerang, bool Shield, bool Bouncing, bool Sine = false, bool Square = false, bool Spiral = false, int SplitStack, int SplitCount)
{
	TexName;
	mIsOwnerCharacter = IsPlayer;
	SetWorldPos(StartPos);
	mDirection = Dir;
	mUnitOwnerAttribute = Attribute;

	mHoming = Homing;
	mPiercing = Piercing;
	mSpectral = Spectral;
	mOrbiting = Orbiting;
	mBoomerang = Boomerang;
	mShield = Shield;
	mBouncing = Bouncing;

	mSine = Sine;
	mSquareWave = Square;
	mSpiral = Spiral;

	mSplitStack = SplitStack;
	mSplitCount = SplitCount;

	FVector2 velocity = mDirection * mUnitOwnerAttribute.ShotSpeed;
	mRigidBody.lock()->SetVelocity(FVector3(velocity.x, velocity.y, 0));
}

//할게 있나?
void CTear::BasicUpdate(float DeltaTime)
{
}

//아래는 나중에 다 만들기
void CTear::OrbitUpdate(float DeltaTime)
{
}

void CTear::HomingUpdate(float DeltaTime)
{
}

void CTear::BoomerangUpdate(float DeltaTime)
{
}

void CTear::SineUpdate(float DeltaTime)
{
}

void CTear::SquareWaveUpdate(float DeltaTime)
{
}

void CTear::SpiralUpdate(float DeltaTime)
{
}
