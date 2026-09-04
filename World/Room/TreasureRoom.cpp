#include "TreasureRoom.h"

#include "Asset/PathManager.h"
#include "Asset/AssetManager.h"
#include "Asset/TextureManager.h"

#include "../Manager/GameRuleManager.h"
#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "../Base/RoomMember.h"
#include "../Obstacle/PedestalObstacle.h"

#include "../Component/SpriteComponent.h"

REGISTER_GAMEOBJCLASS(CTreasureRoom, "TreasureRoom", EObjectType::Room);

CTreasureRoom::CTreasureRoom()
	:CRoombase(ERoomType::Treasure, ERoomShape::Normal)
{
}

CTreasureRoom::~CTreasureRoom()
{
}

bool CTreasureRoom::Init()
{
	if (!CRoombase::Init())
		return false;

	mBackgroundSprite.push_back(CreateComponent<CSpriteComponent>("BackgroundLT"));
	mBackgroundSprite.push_back(CreateComponent<CSpriteComponent>("BackgroundRT"));
	mBackgroundSprite.push_back(CreateComponent<CSpriteComponent>("BackgroundLB"));
	mBackgroundSprite.push_back(CreateComponent<CSpriteComponent>("BackgroundRT"));

	mBackgroundSprite[0].lock()->SetRelativePos(FVector2(-350.f, 200.f));
	mBackgroundSprite[1].lock()->SetRelativePos(FVector2(350.f, 200.f));
	mBackgroundSprite[2].lock()->SetRelativePos(FVector2(-350.f, -200.f));
	mBackgroundSprite[3].lock()->SetRelativePos(FVector2(350.f, -200.f));
	std::wstring path = CPathManager::FindPath("Texture");
	path += TEXT("Room\\Basement.png");
	CAssetManager::GetInst()->GetSubManager<CTextureManager>(EAssetType::Texture)->LoadTexture("Basement", path.c_str());
	for (int i = 0; i < 4; ++i)
	{
		auto sprite = mBackgroundSprite[i].lock();
		sprite->SetRelativeScale(FVector2(700.f, 400.f));
		sprite->SetMesh("TexRect"); mBackgroundSprite[i].lock()->SetShader("Sprite2D");
		sprite->SetTexture("Basement");
		sprite->SetUV(FVector2::Zero, FVector2(234.f, 155.f));
	}
	mBackgroundSprite[1].lock()->SetSymmetry(true);
	mBackgroundSprite[2].lock()->SetFlip(true);
	mBackgroundSprite[3].lock()->SetSymmetry(true);
	mBackgroundSprite[3].lock()->SetFlip(true);

	mRoomImageSize = FVector2(468.f, 310.f); //방 스프라이트의 *2

	CalculateSize();

	mItemID = CGameRuleManager::GetInst()->GenerateRandomI() % 3 + 101;

	return true;
}

void CTreasureRoom::Update(float DeltaTime)
{
	CRoombase::Update(DeltaTime);
}

void CTreasureRoom::Destroy()
{
	CActor::Destroy();
}

void CTreasureRoom::OnEnterRoom()
{
	if (mbFlag)
		return;

	for (std::pair<int, std::weak_ptr<CRoomMember>> pair : mObstacleMap)
	{
		std::shared_ptr<CObstaclebase> ob = std::dynamic_pointer_cast<CObstaclebase>(pair.second.lock());
		if (ob->GetObstacleType() == EObstacleType::Pedestal)
		{
			std::shared_ptr<CPedestalObstacle> pedestal = std::dynamic_pointer_cast<CPedestalObstacle>(ob);
			pedestal->SetItemID(mItemID);
			mbFlag = true;
		}
	}

}

void CTreasureRoom::WinRoom()
{
}

void CTreasureRoom::OnExitRoom()
{
}

bool CTreasureRoom::WinCheck()
{
	return true;
}
