#include "ItemContainer.h"

#include "LogManager.h"

#include "Asset/AssetManager.h"
#include "Asset/AnimationManager.h"
#include "Asset/TextureManager.h"

#include "World/MeshComponent.h"
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

	mActiveItems.reserve(2);

	mHeadMesh.reserve(5);
	mBodyMesh.reserve(5);

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
	std::shared_ptr<CGameDefinition> gdef = CGameClassContainer::GetInst()->InstantiateDef(ID, mOwnerCharacter.lock()->GetThisPtr<CGameClass>());
	std::shared_ptr<CItem> item = std::dynamic_pointer_cast<CItem>(gdef);
	if (!item)
		return;

	switch (item->GetItemType())
	{
	case EItemType::Passive:
		ContainPassive(item);
		break;
	case EItemType::Active:
		if (mActiveContainLimit <= mActiveItems.size())
			return;
		mActiveItems.push_back(item);
		//ContainActive(item);
		break;
	}

	bool check = mContainedItems.find(ID) != mContainedItems.end() ? true : false;

	mContainedItems.insert(std::make_pair(ID, item));
	auto character = mOwnerCharacter.lock();
	if (!check) //만약 같은 아이템이 이미 있는 경우 | 아이템의 외형은 스킵한다.
	{
		//애니메이션 가져오기
		//지금 메시컴포넌트는 하나의 애니메이션 컴포넌트만 할당 가능하다.
		//내일 애니메이션 컴포넌트 여러개 할당하는 것을 테스트 해보고 어떻게 할지 생각하자
		auto animMgr = CAssetManager::GetInst()->GetSubManager<CAnimationManager>(EAssetType::Animation2D);
		if (item->HasHeadAnim()) //머리
		{
			std::shared_ptr<CAnimation2D> head = animMgr->FindAnimation(item->GetHeadAnimName() + "_Front").lock();
			if (item->GetIsHeadAnimOverride()) //이미지 변경
			{
				character->OverrideHeadAnim(item->GetHeadAnimName());
			}
			else //이미지 쌓기
			{
				std::weak_ptr<CMeshComponent> mesh = character->CreateComponent<CMeshComponent>(item->GetName() + "_MHead", "MHead");
				mesh.lock()->SetMesh("TexRect"); mesh.lock()->SetShader("Animation2D"); mesh.lock()->SetRenderLayer("Item");
				std::shared_ptr<CAnimation2DComponent> animator = character->CreateComponent<CAnimation2DComponent>(item->GetName() + "_Head").lock();
				animator->SetUpdateComponent(mesh);
				animator->AddAnimation(head, 1.f, 1.f, true);
				animator->AddAnimation(item->GetHeadAnimName() + "_Back", 1.f, 1.f, true);
				animator->AddAnimation(item->GetHeadAnimName() + "_Right", 1.f, 1.f, true);
				animator->AddAnimation(item->GetHeadAnimName() + "_Left", 1.f, 1.f, true);

				mHeadAnim.push_back(animator);
				mHeadMesh.push_back(mesh);
			}
		}
		if (item->HasBodyAnim()) //몸
		{
			std::shared_ptr<CAnimation2D> body = animMgr->FindAnimation(item->GetBodyAnimName() + "_V").lock();
			if (item->GetIsBodyAnimOverride())
			{
				character->OverrideBodyAnim(item->GetBodyAnimName());
			}
			else
			{
				std::weak_ptr<CMeshComponent> mesh = character->CreateComponent<CMeshComponent>(item->GetName() + "_MBody", "MBody");
				mesh.lock()->SetMesh("TexRect"); mesh.lock()->SetShader("Animation2D"); mesh.lock()->SetRenderLayer("Item");
				std::shared_ptr<CAnimation2DComponent> animator = character->CreateComponent<CAnimation2DComponent>(item->GetName() + "_Body").lock();
				animator->SetUpdateComponent(mesh);
				animator->AddAnimation(body);
				animator->AddAnimation(item->GetHeadAnimName() + "_H", 0.7f, 0.7f, true);

				mBodyAnim.push_back(animator);
				mBodyMesh.push_back(mesh);
			}
		}
	}

	{
		if (item->bIsOnHit)
			mItemCallbackMap["Hit"].push_back(item);
		if (item->bIsOnShot)
			mItemCallbackMap["Shot"].push_back(item);
		if (item->bIsOnKill)
			mItemCallbackMap["Kill"].push_back(item);
		if (item->bIsOnDead)
			mItemCallbackMap["Dead"].push_back(item);
		if (item->bIsEnterRoom)
			mItemCallbackMap["EnterRoom"].push_back(item);
		if (item->bIsOnWinRoom)
			mItemCallbackMap["WinRoom"].push_back(item);
		if (item->bIsExitRoom)
			mItemCallbackMap["ExitRoom"].push_back(item);
		if (item->bIsEnterChapter)
			mItemCallbackMap["EnterChapter"].push_back(item);
	}

	if (item->HasAttribute()) //능력치 적용
	{
		FUnitAttribute itemAttribute = item->GetAttribute();
		if (item->GetIsMagnification())
			character->AddMagnification(itemAttribute);
		else
			character->AddAttribute(itemAttribute);

		character->OnAttributeChanged();
	}

	item->OnGetItem(mOwnerCharacter);
}

bool CItemContainer::IsContained(int ID)
{
	return mContainedItems.find(ID) != mContainedItems.end() ? true : false;
}

void CItemContainer::ReleaseItem(int ID)
{
}

std::string CItemContainer::GetActiveItemName()
{
	if(mActiveItems.empty())
		return std::string();
	return mActiveItems[mActiveFocused]->GetName();
}

void CItemContainer::UseItem()
{
	if (mActiveItems.empty())
		return;

	assert(mActiveItems.size() > mActiveFocused && "액티브 아이템 인덱스가 범위를 벗어남");

	if (mActiveItems[mActiveFocused]->UseItem())
		LOG_DEBUG("아이템 사용됨");
	else
		LOG_DEBUG("아이템 사용 실패");
}

void CItemContainer::OnGetHit(std::weak_ptr<CGameObject> from, int& dmg)
{
	for (std::weak_ptr<CItem> item : mItemCallbackMap["Hit"])
	{
		item.lock()->OnGetHit();
	}
}

void CItemContainer::OnShot()
{
	for (std::weak_ptr<CItem> item : mItemCallbackMap["Shot"])
	{
		item.lock()->OnShot();
	}
}

void CItemContainer::OnDead()
{
	for (std::weak_ptr<CItem> item : mItemCallbackMap["Dead"])
	{
		item.lock()->OnDead();
	}
}

void CItemContainer::OnKill()
{
	for (std::weak_ptr<CItem> item : mItemCallbackMap["Kill"])
	{
		item.lock()->OnKill();
	}
}

void CItemContainer::EnterRoom()
{
	for (std::weak_ptr<CItem> item : mItemCallbackMap["EnterRoom"])
	{
		item.lock()->OnEnterRoom();
	}
}

void CItemContainer::OnWinRoom()
{
	for (std::weak_ptr<CItem> item : mItemCallbackMap["WinRoom"])
	{
		item.lock()->OnWinRoom();
	}
}

void CItemContainer::ExitRoom()
{
	for (std::weak_ptr<CItem> item : mItemCallbackMap["ExitRoom"])
	{
		item.lock()->OnExitRoom();
	}
}

void CItemContainer::EnterChapter()
{
	for (std::weak_ptr<CItem> item : mItemCallbackMap["EnterChapter"])
	{
		item.lock()->OnEnterChapter();
	}
}

void CItemContainer::PlayHeadAnim(bool Stop, int Frame)
{
	for (std::weak_ptr<CAnimation2DComponent> animator : mHeadAnim)
	{
		if (Stop)
			animator.lock()->Stop();
		else
			animator.lock()->Play();
		animator.lock()->SetFrame(Frame);
	}
}

void CItemContainer::PlayBodyAnim(bool Stop, int Frame)
{
	for (std::weak_ptr<CAnimation2DComponent> animator : mBodyAnim)
	{
		if (Stop)
			animator.lock()->Stop();
		else
			animator.lock()->Play();
		animator.lock()->SetFrame(Frame);
	}
}

//원본 애니메이션 이름에 V로 수직 H로 수평 애니메이션 바꾸기
void CItemContainer::SetHeadDirection(FVector2 Dir)
{
	for (std::weak_ptr<CAnimation2DComponent> animator : mHeadAnim)
	{
		std::string animName = animator.lock()->GetCurrentAnimationName();
		if (animName == "")
			continue;

		animName.assign(animName.begin(), animName.begin() + animName.rfind('_'));
		if (Dir.x == 0)
		{
			if (Dir.y > 0)
				animName += "_Back";
			else
				animName += "_Front";
		}
		else
		{
			if (Dir.x > 0)
				animName += "_Right";
			else
				animName += "_Left";
		}

		animator.lock()->ChangeAnimation(animName);
	}
}

void CItemContainer::SetBodyDirection(FVector2 Dir)
{
	bool symmetry = Dir.y > 0 ? true : false;
	for (std::weak_ptr<CAnimation2DComponent> animator : mBodyAnim)
	{
		std::string animName = animator.lock()->GetCurrentAnimationName();
		if (animName == "")
			continue;

		animName.assign(animName.begin(), animName.end() - 2);
		if (Dir.x == 0)
			animName += "_V";
		else
			animName += "_H";

		animator.lock()->SetSymmetry(animName, symmetry);
		animator.lock()->ChangeAnimation(animName);
	}
}

void CItemContainer::SetHeadRenderEnable(bool Enable)
{
	for (std::weak_ptr<CMeshComponent> mesh : mHeadMesh)
	{
		mesh.lock()->SetRenderEnable(Enable);
	}
}

void CItemContainer::SetBodyRenderEnable(bool Enable)
{
	for (std::weak_ptr<CMeshComponent> mesh : mBodyMesh)
	{
		mesh.lock()->SetRenderEnable(Enable);
	}
}

void CItemContainer::ContainPassive(const std::shared_ptr<CItem>& item)
{
}

void CItemContainer::ContainActive(const std::shared_ptr<CItem>& item)
{
}
