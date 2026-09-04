#include "ShopSlotObstacle.h"

#include "../Manager/GameRuleManager.h"
#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "World/ColliderBox2D.h"
#include "World/Animation2DComponent.h"

#include "../Component/SpriteComponent.h"

#include "../Base/Character.h"

REGISTER_GAMEOBJCLASS(CShopSlotObstacle, "SholSlot", EObjectType::Obstacle)

CShopSlotObstacle::CShopSlotObstacle()
    :CObstaclebase(EObstacleType::Pedestal)
{
}

CShopSlotObstacle::~CShopSlotObstacle()
{
}

bool CShopSlotObstacle::Init()
{
    if (!CObstaclebase::Init())
        return false;

    mItemSpriteRenderer = CreateComponent<CSpriteComponent>("ItemSprite");
    if (mItemSpriteRenderer.expired())
        return false;

    mMeshComp.lock()->SetEnable(false);
    mMeshComp.lock()->SetRenderEnable(false);
    mAnimator.lock()->SetEnable(false);

    FVector2 cellsize = mChapter.lock()->GetFocusedRoom().lock()->GetRoomCellSize();

    std::shared_ptr<CSpriteComponent> itemRenderer = mItemSpriteRenderer.lock();
    itemRenderer->SetMesh("TexRect"); itemRenderer->SetShader("Sprite2D");
    itemRenderer->SetRenderLayer("Obstacle");
    itemRenderer->SetWorldScale(cellsize);

    mHitBox.lock()->SetCollisionProfile("ContactHit_Obstacle");
    mHitBox.lock()->SetBeginOverlapFunc(this, &CShopSlotObstacle::OnCollision);

    SetItemID(101);
    return true;
}

void CShopSlotObstacle::Update(float DeltaTime)
{
    CObstaclebase::Update(DeltaTime);
}

void CShopSlotObstacle::Destroy()
{
    CObstaclebase::Destroy();
}

void CShopSlotObstacle::Reset(bool Hard)
{
}

void CShopSlotObstacle::SetItemID(const int ID)
{
    mItemID = ID;
    mItemSpriteRenderer.lock()->SetSpriteData(CGameClassContainer::GetInst()->GetName(ID));
}

FRoomMemberInfo CShopSlotObstacle::GetInfo()
{
    FRoomMemberInfo d;
    d.i_1 = mItemID;
    d.i_2 = mRequireCoin;
    return d;
}

void CShopSlotObstacle::SetInfo(const FRoomMemberInfo& info)
{
    SetItemID(info.i_1);
    //mRequireCoin = info.i_2;
    mRequireCoin = 5;
}

void CShopSlotObstacle::OnCollision(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
    if (Collider.expired() || Collider.lock()->GetOwner().expired())
        return;
    std::shared_ptr<CCharacter> chara = std::dynamic_pointer_cast<CCharacter>(Collider.lock()->GetOwner().lock());
    if (!chara)
        return;

    //플레이어인 경우에만 
    //게임 룰 매니저에서 골드 가져오기
    //부족한 경우 아무것도 하지않음
    if (CGameRuleManager::GetInst()->CanUseCoin(mRequireCoin))
    {
        chara->ContainItem(mItemID);
        CGameRuleManager::GetInst()->RemoveCoin(mRequireCoin);
        ReturnToChapter();
    }
}
