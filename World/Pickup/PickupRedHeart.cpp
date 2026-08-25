#include "PickupRedHeart.h"

#include "LogManager.h"

#include "World/ColliderSphere2D.h"
#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"
#include "../Component/RigidBodyComponent.h"

#include "Asset/AssetManager.h"
#include "../Data/GameDataManager.h"
#include "../Data/AnimGData.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

REGISTER_GAMEOBJCLASS(CPickupRedHeart, "Heart_Red", EObjectType::Pickup)

CPickupRedHeart::CPickupRedHeart()
    :CPickup(EPickupType::Heart, false, true)
{
}

CPickupRedHeart::~CPickupRedHeart()
{
}

bool CPickupRedHeart::Init()
{
    if (!CPickup::Init())
        return false;

    mMeshComp = CreateComponent<CMeshComponent>("Mesh");
    mAnimator = CreateComponent<CAnimation2DComponent>("Animator");
    if (mMeshComp.expired() || mAnimator.expired())
        return false;

    std::shared_ptr<CMeshComponent> mesh = mMeshComp.lock();
    std::shared_ptr<CAnimation2DComponent> animator = mAnimator.lock();

    mesh->SetMesh("TexRect"); mesh->SetShader("Animation2D"); mesh->SetRenderLayer("Pickup"); 
    mesh->SetRelativeScale(25.f, 25.f); mesh->SetSampler("Point_Unwrap");
    animator->SetUpdateComponent(mMeshComp);

    std::shared_ptr<CGameDataManager> mgr = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData);
    if (!mgr->LoadDataFile<CAnimGData>("Heart_Red_Pickup", EGDataType::Anim, TEXT("Heart_Red_Pickup")))
        return false;
    std::shared_ptr<CAnimGData> data = mgr->FindData<CAnimGData>("Heart_Red_Pickup", EGDataType::Anim).lock();
    if (!data || !data->MakeAnim())
        return false;

    animator->AddAnimation(data->GetData().Name, 1.f, 1.f, true);

    mCollider.lock()->SetRadius(10.f);
    mCollider.lock()->SetDebugDraw(true);

    //mRigidbody.lock()->SetWorldScale(100.f, 100.f);

    return true;
}

void CPickupRedHeart::Reset(bool Hard)
{

}
