#include "PedestalObstacle.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "World/ColliderBox2D.h"
#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"
#include "../Component/SpriteComponent.h"

#include "../Base/Character.h"

REGISTER_GAMEOBJCLASS(CPedestalObstacle, "Pedestal", EObjectType::Obstacle)

CPedestalObstacle::CPedestalObstacle()
    :CObstaclebase(EObstacleType::Pedestal)
{
}

CPedestalObstacle::~CPedestalObstacle()
{
}

bool CPedestalObstacle::Init()
{
    if (!CObstaclebase::Init())
        return false;

    mPedestalSpriteRenderer = CreateComponent<CSpriteComponent>("PedestalSprite");
    mItemSpriteRenderer = CreateComponent<CSpriteComponent>("ItemSprite");
    if (mPedestalSpriteRenderer.expired() || mItemSpriteRenderer.expired())
        return false;

    mMeshComp.lock()->SetEnable(false);
    mMeshComp.lock()->SetRenderEnable(false);
    mAnimator.lock()->SetEnable(false);

    FVector2 cellsize = mChapter.lock()->GetFocusedRoom().lock()->GetRoomCellSize();

    std::shared_ptr<CSpriteComponent> pedestalRenderer = mPedestalSpriteRenderer.lock();
    pedestalRenderer->SetMesh("TexRect"); pedestalRenderer->SetShader("Sprite2D");
    pedestalRenderer->SetRenderLayer("Obstacle");
    pedestalRenderer->SetWorldScale(cellsize * 0.8f);

    std::shared_ptr<CSpriteComponent> itemRenderer = mItemSpriteRenderer.lock();
    itemRenderer->SetMesh("TexRect"); itemRenderer->SetShader("Sprite2D");
    itemRenderer->SetRenderLayer("Obstacle");
    itemRenderer->SetWorldScale(cellsize);

    itemRenderer->SetRelativePos(0, 50.f);

    mHitBox.lock()->SetCollisionProfile("ContactHit_Obstacle");
    mHitBox.lock()->SetBeginOverlapFunc(this, &CPedestalObstacle::OnCollision);

    pedestalRenderer->SetSpriteData("Pedestal");

    SetItemID(101);

    return true;
}

void CPedestalObstacle::Update(float DeltaTime)
{
}

void CPedestalObstacle::Destroy()
{
}

void CPedestalObstacle::Reset(bool Hard)
{
}

void CPedestalObstacle::SetItemID(const int ID)
{
    mItemID = ID;
    mItemSpriteRenderer.lock()->SetSpriteData(CGameClassContainer::GetInst()->GetName(ID));
}

void CPedestalObstacle::OnCollision(const FVector3& HitPoint, const FVector3& Normal, std::weak_ptr<class CCollider> Collider)
{
    if (Collider.expired() || Collider.lock()->GetOwner().expired())
        return;
    std::shared_ptr<CCharacter> chara = std::dynamic_pointer_cast<CCharacter>(Collider.lock()->GetOwner().lock());
    if (!chara) 
        return;

    chara->ContainItem(mItemID);
    ReturnToChapter();
}
