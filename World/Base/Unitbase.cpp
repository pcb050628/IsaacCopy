#include "Unitbase.h"

#include "Asset/AssetManager.h"
#include "Asset/AnimationManager.h"
#include "Asset/TextureManager.h"

#include "../Data/GameDataManager.h"
#include "../Data/AnimGData.h"

#include "../Component/RigidBodyComponent.h"
#include "World/Animation2DComponent.h"
#include "World/MeshComponent.h"
#include "World/ColliderSphere2D.h"

CUnitbase::CUnitbase(EObjectType Type)
	:CGameObject(Type)
{
}

CUnitbase::CUnitbase(const CUnitbase& src)
	:CGameObject(src)
{
}

CUnitbase::CUnitbase(CUnitbase&& src) noexcept
	:CGameObject(std::move(src))
{
}

CUnitbase::~CUnitbase()
{
}

bool CUnitbase::Init()
{
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

	std::shared_ptr<CColliderSphere2D> hitbox = mHurtBox.lock();
	hitbox->SetBeginOverlapFunc(this, &CUnitbase::OnHurtOverlaps);
	hitbox->SetEndOverlapFunc(this, &CUnitbase::ExitHurtOverlaps);

	//히트 박스 크기 조정

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
		if (!dataMgr->LoadDataFile<CAnimGData>("Anim_" + Name, FilePath))
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
}

void CUnitbase::Dead()
{
	mbIsDead = true;
}
