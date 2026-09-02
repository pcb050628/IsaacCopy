#include "ItemBelt.h"
#include "../Base/Character.h"

#include "LogManager.h"

#include "Asset/AssetManager.h"
#include "../Data/GameDataManager.h"
#include "../Data/AnimGData.h"

#include "../Manager/GameClassContainer.h"

REGISTER_GAMEDEFCLASS(CItemBelt, "The_Belt", EObjectType::Item)

CItemBelt::CItemBelt()
    :CItem("The_Belt", EItemType::Passive)
{
}

CItemBelt::~CItemBelt()
{
}

bool CItemBelt::Init(const std::weak_ptr<CGameClass>& Owner)
{
    if (!CItem::Init(Owner))
        return false;

    auto mgr = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData);
    if (!mgr->LoadDataFile<CAnimGData>("Item_Belt_Walk_V", EGDataType::Anim, TEXT("Item_Belt_Walk_V")))
        return false;
    if (!mgr->LoadDataFile<CAnimGData>("Item_Belt_Walk_H", EGDataType::Anim, TEXT("Item_Belt_Walk_H")))
        return false;

    std::shared_ptr<CAnimGData> animData = mgr->FindData<CAnimGData>("Item_Belt_Walk_V", EGDataType::Anim).lock();
    if (!animData || !animData->MakeAnim())
        return false;

    animData = mgr->FindData<CAnimGData>("Item_Belt_Walk_H", EGDataType::Anim).lock();
    if (!animData || !animData->MakeAnim())
        return false;

    mbHasBodyAnim = true;
    mbIsBodyAnimOverride = true;
    mbIsHasAttribute = true;
    mUnitAdditionalAttribute.Speed = 5.f;
    mBodyAnimName = "Item_Belt_Walk";

    mQuote = "SPEED UP";

    return true;
}
void CItemBelt::Reset(bool HardReset)
{
}

void CItemBelt::OnGetItem(const std::weak_ptr<class CCharacter>& character)
{
    //auto p = character.lock();
    //if(mbIsAttributeMagnification)
    //    p->SetAttribute(p->GetAttribute() * mUnitAdditionalAttribute);
    //else
    //    p->SetAttribute(p->GetAttribute() + mUnitAdditionalAttribute);
}
