#include "ItemBelt.h"
#include "../Base/Character.h"

#include "LogManager.h"

#include "Asset/AssetManager.h";
#include "../Data/GameDataManager.h"
#include "../Data/AnimGData.h"

#include "../Manager/GameClassContainer.h"

REGISTER_GAMEDEFCLASS(CItemBelt, "Belt", EObjectType::Item)

CItemBelt::CItemBelt()
    :CItem(EItemType::Passive)
{
}

CItemBelt::~CItemBelt()
{
}

bool CItemBelt::Init()
{
    if (!CItem::Init())
        return false;

    mbIsBodyAnimOverride = true;
    mUnitAdditionalAttribute.Speed = 20.f;

    auto mgr = CAssetManager::GetInst()->GetSubManager<CGameDataManager>(EAssetType::GameData);
    if (!mgr->LoadDataFile<CAnimGData>("Item_Belt_Walk_V", TEXT("Anim\\Item_Belt_Walk_V")))
        return false;
    if (!mgr->LoadDataFile<CAnimGData>("Item_Belt_Walk_H", TEXT("Anim\\Item_Belt_Walk_H")))
        return false;

    std::shared_ptr<CAnimGData> animData = std::dynamic_pointer_cast<CAnimGData>(mgr->FindData("Item_Belt_Walk_V").lock());
    if (!animData || !animData->MakeAnim())
        return false;

    animData = std::dynamic_pointer_cast<CAnimGData>(mgr->FindData("Item_Belt_Walk_H").lock());
    if (!animData || !animData->MakeAnim())
        return false;

    mBodyAnimName = "Item_Belt_Walk";

    return true;
}

void CItemBelt::Destroy()
{
    CItem::Destroy();
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
