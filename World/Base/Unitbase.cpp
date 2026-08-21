#include "Unitbase.h"

#include "Asset/AssetManager.h"
#include "Asset/AnimationManager.h"
#include "Asset/TextureManager.h"

#include "World/Animation2DComponent.h"
#include "World/MeshComponent.h"
#include "World/ColliderSphere2D.h"

#include "../Data/GameDataManager.h"
#include "../Data/AnimGData.h"

#include "../Component/RigidBodyComponent.h"
#include "../Component/TearShooter.h"

CUnitbase::CUnitbase(EObjectType Type)
	:CRoomMember(Type)
{
}

CUnitbase::CUnitbase(const CUnitbase& src)
	:CRoomMember(src)
{
}

CUnitbase::CUnitbase(CUnitbase&& src) noexcept
	:CRoomMember(std::move(src))
{
}

CUnitbase::~CUnitbase()
{
}

bool CUnitbase::Init()
{
	if (!CGameObject::Init())
		return false;

	mRigidBody = CreateComponent<CRigidBodyComponent>("Root");
	if (mRigidBody.expired())
		return false;

	mHeadMesh = CreateComponent<CMeshComponent>("MHead");
	mBodyMesh = CreateComponent<CMeshComponent>("MBody");
	if (mHeadMesh.expired() || mBodyMesh.expired())
		return false;

	mHead = CreateComponent<CAnimation2DComponent>("AHead");
	mBody = CreateComponent<CAnimation2DComponent>("ABody");
	if (mHead.expired() || mBody.expired())
		return false;

	mHead.lock()->SetUpdateComponent(mHeadMesh);
	mBody.lock()->SetUpdateComponent(mBodyMesh);

	mRigidBody.lock()->SetWorldScale(FVector2(10.f, 10.f));

	mHeadMesh.lock()->SetRenderLayer("Head");
	mBodyMesh.lock()->SetRenderLayer("Body");

	mHeadMesh.lock()->SetMesh("TexRect");
	mHeadMesh.lock()->SetShader("Animation2D");
	mBodyMesh.lock()->SetMesh("TexRect");
	mBodyMesh.lock()->SetShader("Animation2D");

	mHeadMesh.lock()->SetRelativePos(FVector2(0, 47.f));
	mBodyMesh.lock()->SetRelativePos(FVector2(0, 9.f));

	mHeadMesh.lock()->SetRelativeScale(FVector2(7.f, 7.f));
	mBodyMesh.lock()->SetRelativeScale(FVector2(5.5f, 5.5f));

	mHurtBox = CreateComponent<CColliderSphere2D>("HurtBox");
	if (mHurtBox.expired())
		return false;

	std::shared_ptr<CColliderSphere2D> hurtbox = mHurtBox.lock();
	hurtbox->SetBeginOverlapFunc(this, &CUnitbase::OnHurtOverlaps);
	hurtbox->SetEndOverlapFunc(this, &CUnitbase::ExitHurtOverlaps);

	//히트 박스 크기 조정

	mMagnification.ShotTerm = 1.f;
	mMagnification.Speed = 1.f;
	mMagnification.Luck = 1.f;
	mMagnification.Damage = 1.f;
	mMagnification.ShotSpeed = 1.f;
	mMagnification.Range = 1.f;
	mMagnification.Height = 1.f;
	mMagnification.knockback = 1.f;
	
	mActorTag = "Unit";
	return true;
}

void CUnitbase::Update(float DeltaTime)
{
	CActor::Update(DeltaTime);
}

void CUnitbase::Destroy()
{
	CActor::Destroy();
}

bool CUnitbase::AddAnim(const std::string& Name, const TCHAR* FilePath, bool Upper, float PlayTime, float PlayRate, bool Loop, bool Reverse, bool Symmetry)
{
	std::shared_ptr<CGameDataManager> dataMgr = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData);
	std::weak_ptr<CGameData> data = dataMgr->FindData("Anim_" + Name);
	if (data.expired())
	{
		std::wstring path = L"Anim\\";
		path += FilePath;
		if (!dataMgr->LoadDataFile<CAnimGData>("Anim_" + Name, path.c_str()))
			return false;
		data = dataMgr->FindData("Anim_" + Name);
	}
	std::shared_ptr<CAnimGData> d = std::dynamic_pointer_cast<CAnimGData>(data.lock());
	d->MakeAnim();

	std::shared_ptr<CAnimationManager> mgr = CAssetManager::GetInst()->GetSubManager<CAnimationManager>(EAssetType::Animation2D);
	std::weak_ptr<CAnimation2D> anim = mgr->FindAnimation(Name);

	if (Upper)
		mHead.lock()->AddAnimation(Name, PlayTime, PlayRate, Loop, Reverse, Symmetry);
	else
		mBody.lock()->AddAnimation(Name, PlayTime, PlayRate, Loop, Reverse, Symmetry);

	return true;
}

void CUnitbase::Move(const FVector3& Force) const
{
	mRigidBody.lock()->SetVelocity(Force);
}

void CUnitbase::Fire()
{
	if (mShooter.expired())
		return;

	std::shared_ptr<CTearShooter> shooter = mShooter.lock();

	//일단 해놓긴 했는데
	//발사 가능 상태를 어디서 검사하는게 맞는지 확신이 없네
	//일단은 슈터에서 확인하는게 맞는거같긴함
	//유닛쪽에서는 모든 역할을 슈터에 위임한거니까
	//아는건 아무것도 없고 능력치만 던져주고 발사하라고만 하는거니까
	//검사는 슈터에서 해야겟지?
	shooter->Fire();
}

void CUnitbase::Dead()
{
	mbIsDead = true;
}

void CUnitbase::SetBodyDirection(FVector2 Dir)
{
	if(mBodyDirection == Dir)
		return;

	if (mBodyDirection != Dir)
	{
		mBodyDirection = Dir;
		if (0 == Dir.x)
		{
			PlayBodyVerticalAnim();
		}
		else
		{
			PlayBodyHorizontalAnim();
		}
	}
}

void CUnitbase::SetHeadDirection(FVector2 Dir)
{
	if (mHeadDirection == Dir)
		return;

	if (mHeadDirection != Dir)
	{
		mHeadDirection = Dir;
		if (0 == Dir.x)
		{
			PlayHeadVerticalAnim();
		}
		else
		{
			PlayHeadHorizontalAnim();
		}
	}
}

void CUnitbase::PlayBodyVerticalAnim()
{
}

void CUnitbase::PlayBodyHorizontalAnim()
{
}

void CUnitbase::PlayHeadVerticalAnim()
{
}

void CUnitbase::PlayHeadHorizontalAnim()
{
}

const std::weak_ptr<CSceneComponent>& CUnitbase::GetHeadComp()
{
	if(mHeadMesh.expired())
		return std::weak_ptr<CSceneComponent>();
	return mHeadMesh.lock()->GetThisPtr<CSceneComponent>();
}

const std::weak_ptr<CSceneComponent>& CUnitbase::GetBodyComp()
{
	if (mBodyMesh.expired())
		return std::weak_ptr<CSceneComponent>();
	return mBodyMesh.lock()->GetThisPtr<CSceneComponent>();
}
