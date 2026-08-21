#include "Tear.h"

#include "LogManager.h"

#include "Asset/AssetManager.h"
#include "World/MeshComponent.h"
#include "World/ColliderSphere2D.h"
#include "World/Animation2DComponent.h"

#include "../Data/GameDataManager.h"
#include "../Data/GameObjectStructure.h"
#include "../Data/AnimGData.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"
#include "../Component/RigidBodyComponent.h"

#include "../Component/TearShooter.h"

REGISTER_GAMEOBJCLASS(CTear, "Tear", EObjectType::Tear)

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

	mRigidBody.lock()->SetLimit(10000000.f);
	mRigidBody.lock()->SetMass(0.1f);
	mRigidBody.lock()->SetUseGravity(false);

	mMesh.lock()->SetMesh("TexRect");
	mMesh.lock()->SetShader("Animation2D");

	std::shared_ptr<CGameDataManager> mgr = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData);
	if(!mgr->LoadDataFile<CAnimGData>("Tear_Default", TEXT("Anim/Tear_Default")))
		return false;
	if (!mgr->LoadDataFile<CAnimGData>("Tear_Default_Pop", TEXT("Anim/Tear_Default_Pop")))
		return false;
	std::shared_ptr<CAnimGData> animData = std::dynamic_pointer_cast<CAnimGData>(mgr->FindData("Tear_Default").lock());
	animData->MakeAnim();

	std::shared_ptr<CAnimation2DComponent> animator = mAnimator.lock();
	animator->SetUpdateComponent(mMesh);
	animator->AddAnimation(animData->GetData().Name);
	animator->Stop();
	animator->SetFrame(3);

	animData = std::dynamic_pointer_cast<CAnimGData>(mgr->FindData("Tear_Default_Pop").lock());
	animData->MakeAnim();
	animator->AddAnimation(animData->GetData().Name);
	mAnimator.lock()->SetFinishFunction(animData->GetData().Name, this, &CTear::ReturnToChapter);
	
	mMesh.lock()->SetWorldScale(100.f, 100.f);
	mMesh.lock()->SetRenderLayer("Tear");
	//mHitBox.lock()->SetBeginOverlapFunc()

	mHitBox.lock()->SetCollisionProfile("Tear");
	mHitBox.lock()->SetRadius(15.f);
	mHitBox.lock()->SetDebugDraw(true);
	mHitBox.lock()->SetBeginOverlapFunc(this, &CTear::OnCollision);

	//히트 박스 스케일(radius) 조정
	//메시 스케일 조정
	//애니메이션 불러오고

	return true;
}

void CTear::Update(float DeltaTime)
{
	if (!mbIsDying)
	{
		//수명측정 방법
		//보니까 사거리가 6.5여도 속도가 0.1이면 한칸도 안가서 멈추더라
		//느리게 꾸역꾸역 사거리 만큼 가는게 아니라 시간누적해서 가는거

		//이부분도 전부 수정해야함 | 아닌가
		if (mTearAttribute.Homing)
		{
			HomingUpdate(DeltaTime);
		}
		if (mTearAttribute.Orbiting)
		{
			OrbitUpdate(DeltaTime);
		}
		if (mTearAttribute.Boomerang)
		{
			BoomerangUpdate(DeltaTime);
		}

		//그럼 시간누적은 어떻게 할까
		//mMoveDistance = DeltaTime * speed * RoomCellSize(85.f);
		//if(mMoveDistance > mUnitOwnerAttribute.Range) Destroy();
		mMovedDistance += DeltaTime * mRigidBody.lock()->GetVelocity().Length();
		if (mMovedDistance >= mTearAttribute.Range)
		{
			TearDying();
		}
	}
	else
	{
		if (mAnimator.lock()->GetCurrentAnimationName() != "Tear_Default_Pop")
		{
			mAnimator.lock()->ChangeAnimation("Tear_Default_Pop");
			mAnimator.lock()->Play();
		}
	}

	CGameObject::Update(DeltaTime);
}

void CTear::Destroy()
{
	if (!mShooter.expired())
		mShooter.lock()->OnDestroy(GetWorldPos());
	CGameObject::Destroy();
}

void CTear::Reset(bool HardReset)
{
	mIsOwnerCharacter = false;
	mbIsDying = false;

	mMovedDistance = 0.f;
	mTearAttribute = FTearAttribute();
	TexName = "Tear_Default";

	SetEnable(true);
	SetRenderEnable(true);
	mAnimator.lock()->ChangeAnimation("Tear_Default");
	mAnimator.lock()->SetFrame(3);
}

//생각해보니까 텍스쳐도 골라줘야함
//눈물이 너무 여러가지라서
void CTear::Set(bool IsPlayer, FVector3 StartPos, FVector2 Dir, FUnitAttribute Attribute
	, bool Homing, bool Piercing, bool Spectral, bool Orbiting, bool Boomerang, bool Shield, bool Bouncing
	, bool Sine, bool Square, bool Spiral, int SplitStack, int SplitCount)
{
	TexName;
	mIsOwnerCharacter = IsPlayer;
	SetWorldPos(StartPos);
	mTearAttribute.Direction = Dir;

	mTearAttribute.Damage = Attribute.Damage;
	mTearAttribute.Speed = Attribute.ShotSpeed;
	mTearAttribute.Range = Attribute.Range;
	mTearAttribute.Height = Attribute.Height;
	mTearAttribute.knockback = Attribute.knockback;

	mTearAttribute.Homing = Homing;
	mTearAttribute.Piercing = Piercing;
	mTearAttribute.Spectral = Spectral;
	mTearAttribute.Orbiting = Orbiting;
	mTearAttribute.Boomerang = Boomerang;
	mTearAttribute.Shield = Shield;
	mTearAttribute.Bouncing = Bouncing;

	mTearAttribute.Sine = Sine;
	mTearAttribute.SquareWave = Square;
	mTearAttribute.Spiral = Spiral;

	mTearAttribute.SplitStack = SplitStack;
	mTearAttribute.SplitCount = SplitCount;

	FVector2 velocity = mTearAttribute.Direction * mTearAttribute.Speed * 85.f;
	mRigidBody.lock()->SetVelocity(FVector3(velocity.x, velocity.y, 0));
}

void CTear::Set(bool IsPlayer, FVector3 StartPos, FTearAttribute Attribute, std::weak_ptr<CTearShooter> Shooter)
{
	mIsOwnerCharacter = IsPlayer;
	SetWorldPos(StartPos);
	mTearAttribute = Attribute;

	mTearAttribute.Direction.Normalize();
	FVector2 velocity = mTearAttribute.Direction * mTearAttribute.Speed * 85.f * 0.5f;
	mRigidBody.lock()->SetVelocity(FVector3(velocity.x, velocity.y, 0));

	mShooter = Shooter;
}

void CTear::ReturnToChapter()
{
	CGameObject::ReturnToChapter();
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

void CTear::TearDying()
{
	mbIsDying = true;
	//애니메이션 플레이하기
	//애니메이션 끝나는데 콜백걸어놓고 Destroy 하기
	mAnimator.lock()->ChangeAnimation("Tear_Default_Pop");
	mAnimator.lock()->Play(true);
	mRigidBody.lock()->SetVelocity(FVector3::Zero);
	mHitBox.lock()->SetEnable(false);
}

void CTear::OnCollision(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
	if (Collider.lock()->GetOwner().lock() == mShooter.lock()->GetOwner().lock())
		return;

	TearDying();
}

std::weak_ptr<CGameObject> CTear::GetShooterOwner()
{
	std::shared_ptr<CActor> act = mShooter.lock()->GetOwner().lock();
	return std::dynamic_pointer_cast<CGameObject>(act);
}
