#include "RockObstacle.h"

#include "LogManager.h"

#include "Asset/AssetManager.h"

#include "World/Animation2DComponent.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "../Data/GameDataManager.h"
#include "../Data/AnimGData.h"
#include "../Data/GameObjectStructure.h"

REGISTER_GAMEOBJCLASS(CRockObstacle, "Rock", EObjectType::Obstacle)

CRockObstacle::CRockObstacle()
    :CObstaclebase(EObstacleType::Rock)
{
}

CRockObstacle::~CRockObstacle()
{
}

bool CRockObstacle::Init()
{
    if(!CObstaclebase::Init())
        return false;

    //이미지 가져오기
    std::shared_ptr<CGameDataManager> dataMgr = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData);
    if (!dataMgr->LoadDataFile<CAnimGData>("Rock_1", EGDataType::Anim, TEXT("Rock_1")))
        return false;

    std::shared_ptr<CAnimGData> animData = dataMgr->FindData<CAnimGData>("Rock_1", EGDataType::Anim).lock();
    if (!animData || !animData->MakeAnim())
        return false;
    
    std::shared_ptr<CAnimation2DComponent> animator = mAnimator.lock();
    animator->AddAnimation(animData->GetData().Name);

    return true;
}

void CRockObstacle::Update(float DeltaTime)
{
    CObstaclebase::Update(DeltaTime);
}

void CRockObstacle::Destroy()
{
    CObstaclebase::Destroy();
}

void CRockObstacle::Reset(bool Hard)
{
    //상태 돌려주기
}
