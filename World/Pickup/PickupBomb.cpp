#include "PickupBomb.h"

#include "TimeManager.h"

#include "../Manager/GameRuleManager.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "../Component/SpriteComponent.h"
#include "World/ColliderSphere2D.h"

REGISTER_GAMEOBJCLASS(CPickupBomb, "Bomb", EObjectType::Pickup)

CPickupBomb::CPickupBomb()
	:CPickup(EPickupType::Bomb, false, true)
{
}

CPickupBomb::~CPickupBomb()
{
}

bool CPickupBomb::Init()
{
	if (!CPickup::Init())
		return false;

	mSpriteRenderer = CreateComponent<CSpriteComponent>("Sprite");

	mSpriteRenderer.lock()->SetMesh("TexRect");mSpriteRenderer.lock()->SetShader("Sprite2D");mSpriteRenderer.lock()->SetRenderLayer("Pickup");
	mSpriteRenderer.lock()->SetSpriteData("Bomb");

	return true;
}

void CPickupBomb::Ignite()
{
	mbIsIgnited = true;
	CTimeManager::SetTimer(1.5f, false, this, &CPickupBomb::Explosion);
}

void CPickupBomb::Explosion()
{
	//범위내 적들 100 피해 주기
	ReturnToChapter();
}

bool CPickupBomb::TryGet(std::weak_ptr<class CCharacter> chara)
{
	if (mbIsIgnited)
		return false;

	CGameRuleManager::GetInst()->AddBomb(mAffectPickupCount);

	return true;
}
