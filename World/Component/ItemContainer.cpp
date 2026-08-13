#include "ItemContainer.h"

#include "Asset/AssetManager.h"
#include "Asset/AnimationManager.h"
#include "Asset/TextureManager.h"

#include "World/Animation2DComponent.h"

#include "../Manager/GameClassContainer.h"
#include "../Base/Character.h"
#include "../Base/Item.h"

CItemContainer::CItemContainer()
{
}

CItemContainer::~CItemContainer()
{
}

bool CItemContainer::Init()
{
    if (!CActorComponent::Init())
        return false;

    mItems.reserve(20);
    mActiveItems.reserve(2);

    mHeadAnim.reserve(5);
    mBodyAnim.reserve(5);

    mOwnerCharacter = std::dynamic_pointer_cast<CCharacter>(mOwner.lock());
    if (mOwnerCharacter.expired())
        return false;

    mCharacterTearShooter = mOwnerCharacter.lock()->GetShooterComp();
    if (mCharacterTearShooter.expired())
        return false;

    return true;
}

void CItemContainer::Update(float DeltaTime)
{
    CActorComponent::Update(DeltaTime);
}

void CItemContainer::Destroy()
{
    CActorComponent::Destroy();
}

void CItemContainer::ContainItem(int ID)
{
	std::shared_ptr<CItem> item = std::dynamic_pointer_cast<CItem>(CGameClassContainer::GetInst()->Instantiate(ID, FVector2(0, 0)).lock());
    if (!item || !item->Init())
        return;

    mItems.push_back(item);

    //애니메이션 가져오기
    //지금 메시컴포넌트는 하나의 애니메이션 컴포넌트만 할당 가능하다.
    //내일 애니메이션 컴포넌트 여러개 할당하는 것을 테스트 해보고 어떻게 할지 생각하자
    auto animMgr = CAssetManager::GetInst()->GetSubManager<CAnimationManager>(EAssetType::Animation2D);
    std::shared_ptr<CAnimation2D> head = animMgr->FindAnimation(item->GetHeadAnimName()).lock();
    if (head)
    {
        std::shared_ptr<CAnimation2DComponent> animator = mOwner.lock()->CreateComponent<CAnimation2DComponent>(item->GetName() + "_Head", "MHead").lock();

        mHeadAnim.push_back(animator);
    }
    std::shared_ptr<CAnimation2D> body = animMgr->FindAnimation(item->GetBodyAnimName()).lock();
    if (body)
    {
        std::shared_ptr<CAnimation2DComponent> animator = mOwner.lock()->CreateComponent<CAnimation2DComponent>(item->GetName() + "_Body", "MBody").lock();

        mHeadAnim.push_back(animator);
    }

    if (item->bIsOnHit)
        mItemMap["Hit"].push_back(item);
    if (item->bIsOnShot)
        mItemMap["Shot"].push_back(item);
    if (item->bIsOnKill)
        mItemMap["Kill"].push_back(item);
    if (item->bIsOnDead)
        mItemMap["Dead"].push_back(item);
    if (item->bIsEnterRoom)
        mItemMap["EnterRoom"].push_back(item);
    if (item->bIsOnWinRoom)
        mItemMap["WinRoom"].push_back(item);
    if (item->bIsExitRoom)
        mItemMap["ExitRoom"].push_back(item);
    if (item->bIsEnterChapter)
        mItemMap["EnterChapter"].push_back(item);

}

void CItemContainer::OnHit()
{
    for (std::weak_ptr<CItem> item : mItemMap["Hit"])
    {
        item.lock()->OnHit();
    }
}

void CItemContainer::OnShot()
{
    for (std::weak_ptr<CItem> item : mItemMap["Shot"])
    {
        item.lock()->OnShot();
    }
}

void CItemContainer::OnDead()
{
    for (std::weak_ptr<CItem> item : mItemMap["Dead"])
    {
        item.lock()->OnDead();
    }
}

void CItemContainer::OnKill()
{
    for (std::weak_ptr<CItem> item : mItemMap["Kill"])
    {
        item.lock()->OnKill();
    }
}

void CItemContainer::EnterRoom()
{
    for (std::weak_ptr<CItem> item : mItemMap["EnterRoom"])
    {
        item.lock()->OnEnterRoom();
    }
}

void CItemContainer::OnWinRoom()
{
    for (std::weak_ptr<CItem> item : mItemMap["WinRoom"])
    {
        item.lock()->OnWinRoom();
    }
}

void CItemContainer::ExitRoom()
{
    for (std::weak_ptr<CItem> item : mItemMap["ExitRoom"])
    {
        item.lock()->OnExitRoom();
    }
}

void CItemContainer::EnterChapter()
{
    for (std::weak_ptr<CItem> item : mItemMap["EnterChapter"])
    {
        item.lock()->OnEnterChapter();
    }
}

void CItemContainer::PlayHeadAnim(bool Play, int Frame)
{
    for (std::weak_ptr<CAnimation2DComponent> animator : mHeadAnim)
    {
        animator.lock()->Play(Play);
        animator.lock()->SetFrame(Frame);
    }
}

void CItemContainer::PlayBodyAnim(bool Play, int Frame)
{
    for (std::weak_ptr<CAnimation2DComponent> animator : mBodyAnim)
    {
        animator.lock()->Play(Play);
        animator.lock()->SetFrame(Frame);
    }
}

//원본 애니메이션 이름에 V로 수직 H로 수평 애니메이션 바꾸기
void CItemContainer::SetHeadDirection(FVector2 Dir)
{
    for (std::weak_ptr<CAnimation2DComponent> animator : mHeadAnim)
    {
        std::string animName = animator.lock()->GetCurrentAnimationName();
        animName.assign(animName.begin(), animName.begin() + animName.rfind('_'));
        if (Dir.x == 0)
        {
            if (Dir.y > 0)
                animName += "Back";
            else
                animName += "Front";
        }
        else
        {
            if (Dir.x > 0)
                animName += "Right";
            else
                animName += "Left";
        }

        animator.lock()->ChangeAnimation(animName);
    }
}

void CItemContainer::SetBodyDirection(FVector2 Dir)
{
    for (std::weak_ptr<CAnimation2DComponent> animator : mBodyAnim)
    {
        std::string animName = animator.lock()->GetCurrentAnimationName();
        animName.assign(animName.begin(), animName.end() - 2);
        if (Dir.x == 0)
            animName += "_V";
        else
            animName += "_H";

        animator.lock()->ChangeAnimation(animName);
    }
}

void CItemContainer::SetMeshComponent(const std::weak_ptr<class CMeshComponent>& Head, const std::weak_ptr<class CMeshComponent>& Body)
{
    mHeadMesh = Head;
    mBodyMesh = Body;
}
