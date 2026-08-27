#include "ItemCroockedPenny.h"

#include "LogManager.h"

#include "Asset/AssetManager.h"
#include "Asset/TextureManager.h"

#include "../Manager/GameRuleManager.h"

#include "../Chapter.h"
#include "../Base/Character.h"
#include "../Component/TearShooter.h"

#include "../Manager/GameClassContainer.h"

REGISTER_GAMEDEFCLASS(CItemCroockedPenny, "Croocked Penny", EObjectType::Item)

CItemCroockedPenny::CItemCroockedPenny()
    :CItem("Crooked_Penny", EItemType::Active, false, false, false, false, false, true)
{
    mEnergyMax = 4;
    mCurrentEnergy = mEnergyMax;
}

CItemCroockedPenny::~CItemCroockedPenny()
{
}

bool CItemCroockedPenny::Init(const std::weak_ptr<CGameClass>& Owner)
{
    if (!CItem::Init(Owner))
        return false;

    //이 아이템은 애니메이션 없이 스프라이트만 있음
    auto mgr = CAssetManager::GetInst()->GetSubManager<CTextureManager>(EAssetType::Texture);
    if (mgr->FindTexture("ItemSprite_").expired())
        return false;

    mItemSpriteName = "ItemSprite_";
    mItemSpriteStart = FVector2(0, 768);
    mItemSpriteSize = FVector2(32, 32);

    return true;
}

void CItemCroockedPenny::Reset(bool HardReset)
{
}

bool CItemCroockedPenny::UseItem()
{
    std::shared_ptr<CRoomMember> obj = std::dynamic_pointer_cast<CRoomMember>(mOwner.lock());
    if (!obj)
        return false;

    std::shared_ptr<CRoombase> room = obj->GetRoom().lock();
    if (!room)
        return false;
    if (CGameRuleManager::GetInst()->GenerateRandomF() >= 0.5f)
    {
        std::vector<std::pair<int, FVector2>> pickups;
        room->GetIdOfPickup(pickups);
        for (std::pair<int, FVector2> info : pickups)
        {
            CGameClassContainer::GetInst()->Instantiate(info.first, info.second);
        }
        LOG_DEBUG("50% 확률 성공: 2배");
    }
    else
    {
        std::vector<std::weak_ptr<CRoomMember>> pickups;
        room->GetPickups(pickups);
        for (std::weak_ptr<CRoomMember> rm : pickups)
        {
            rm.lock()->ReturnToChapter();
        }
        LOG_DEBUG("50% 확률 실패: 제거");
    }

    return true;
}

void CItemCroockedPenny::OnGetItem(const std::weak_ptr<class CCharacter>& character)
{
}
