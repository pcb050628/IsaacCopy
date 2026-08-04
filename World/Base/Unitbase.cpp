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

	mHead = CreateComponent<CAnimation2DComponent>("MHead");
	mBody = CreateComponent<CAnimation2DComponent>("MBody");
	if (mHead.expired() || mBody.expired())
		return false;

	//몸 머리 오프셋 넣기 - SetRelativePos, SetRelativeScale

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

bool CUnitbase::SetAnim(const std::string& Name, const TCHAR* FilePath, bool Upper, float PlayTime, float PlayRate, bool Loop, bool Reverse, bool Symmetry)
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
