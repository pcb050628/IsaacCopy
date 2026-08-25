#include "ItemSpoonBender.h"

#include "LogManager.h"

#include "Asset/AssetManager.h";
#include "../Data/GameDataManager.h"
#include "../Data/AnimGData.h"

#include "../Base/Character.h"
#include "../Component/TearShooter.h"

#include "../Manager/GameClassContainer.h"

REGISTER_GAMEDEFCLASS(CItemSpoonBender, "SpoonBender", EObjectType::Item)

CItemSpoonBender::CItemSpoonBender()
	:CItem(EItemType::Passive)
{
}

CItemSpoonBender::~CItemSpoonBender()
{
}

bool CItemSpoonBender::Init()
{
    if (!CItem::Init())
        return false;
    
    auto mgr = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData);
    if (!mgr->LoadDataFile<CAnimGData>("Item_Spoon_Bender_Front", EGDataType::Anim, TEXT("Item_Spoon_Bender_Front")))
        return false;
    if (!mgr->LoadDataFile<CAnimGData>("Item_Spoon_Bender_Back", EGDataType::Anim, TEXT("Item_Spoon_Bender_Back")))
        return false;
    if (!mgr->LoadDataFile<CAnimGData>("Item_Spoon_Bender_Right", EGDataType::Anim, TEXT("Item_Spoon_Bender_Right")))
        return false;
    if (!mgr->LoadDataFile<CAnimGData>("Item_Spoon_Bender_Left", EGDataType::Anim, TEXT("Item_Spoon_Bender_Left")))
        return false;

    std::shared_ptr<CAnimGData> animData = mgr->FindData<CAnimGData>("Item_Spoon_Bender_Front", EGDataType::Anim).lock();
    if (!animData || !animData->MakeAnim())
        return false;

    animData = mgr->FindData<CAnimGData>("Item_Spoon_Bender_Back", EGDataType::Anim).lock();
    if (!animData || !animData->MakeAnim())
        return false;
    animData = mgr->FindData<CAnimGData>("Item_Spoon_Bender_Right", EGDataType::Anim).lock();
    if (!animData || !animData->MakeAnim())
        return false;
    animData = mgr->FindData<CAnimGData>("Item_Spoon_Bender_Left", EGDataType::Anim).lock();
    if (!animData || !animData->MakeAnim())
        return false;

    mHeadAnimName = "Item_Spoon_Bender";

    return true;
}

void CItemSpoonBender::Destroy()
{
	CGameObject::Destroy();
}

void CItemSpoonBender::Reset(bool HardReset)
{
}

void CItemSpoonBender::OnGetItem(const std::weak_ptr<class CCharacter>& character)
{
    character.lock()->GetShooterComp().lock()->SetHoming(true);
}
