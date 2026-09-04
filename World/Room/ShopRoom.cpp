#include "ShopRoom.h"

#include "../Manager/GameRuleManager.h"
#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "Asset/PathManager.h"
#include "Asset/AssetManager.h"
#include "Asset/TextureManager.h"

#include "../Component/SpriteComponent.h"

#include "../Obstacle/ShopSlotObstacle.h"


REGISTER_GAMEOBJCLASS(CShopRoom, "ShopRoom", EObjectType::Room)

CShopRoom::CShopRoom()
	:CRoombase(ERoomType::Shop)
{
}

CShopRoom::~CShopRoom()
{
}

bool CShopRoom::Init()
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
	path += TEXT("Room\\Shop_.png");
	CAssetManager::GetInst()->GetSubManager<CTextureManager>(EAssetType::Texture)->LoadTexture("Shop", path.c_str());
	for (int i = 0; i < 4; ++i)
	{
		auto sprite = mBackgroundSprite[i].lock();
		sprite->SetRelativeScale(FVector2(700.f, 400.f));
		sprite->SetMesh("TexRect"); mBackgroundSprite[i].lock()->SetShader("Sprite2D");
		sprite->SetTexture("Shop");
		sprite->SetUV(FVector2::Zero, FVector2(234.f, 155.f));
		sprite->SetBaseColor(0, FVector4(4, 4, 4, 1));
		sprite->SetChromaKey(0, true, FVector3(255.f / 255, 200.f / 255, 210.f / 255), 1.f);
	}
	mBackgroundSprite[1].lock()->SetSymmetry(true);
	mBackgroundSprite[2].lock()->SetFlip(true);
	mBackgroundSprite[3].lock()->SetSymmetry(true);
	mBackgroundSprite[3].lock()->SetFlip(true);

	mRoomImageSize = FVector2(468.f, 310.f); //방 스프라이트의 *2

	CalculateSize();

	mOpenInfo.state = EOpenState::Open;

	//슬롯 데이터 초기화
	//최대 5개 - 아이템인지 픽업인지 - 랜덤 아이디 입력 - 아이템은 100번대 픽업은 80번대
	//방 입장시 false 인 위치는 비활성화하기

	int num = 100 + CGameRuleManager::GetInst()->GenerateRandomI() % 3 + 1;
	mSlots.push_back(std::make_pair(true, num));

	return true;
}

void CShopRoom::Update(float DeltaTime)
{
	CRoombase::Update(DeltaTime);
}

void CShopRoom::Destroy()
{
	CGameObject::Destroy();
}

void CShopRoom::OnEnterRoom()
{
	if (!mbFlag)
	{
		if (mSlotObstacles.empty())
		{
			for (std::pair<int, std::weak_ptr<CRoomMember>> pair : mObstacleMap)
			{
				assert(!pair.second.expired() && "유효하지 않은 객체를 참조중");
				if (402 == pair.second.lock()->GetGClassID())
				{
					mSlotObstacles.push_back(pair.second);
				}
			}
		}
		mbFlag = true;
	}

	for (int i = 0; i < mSlots.size(); ++i)
	{
		bool enable = mSlots[i].first;
		if (enable)
			continue;

		mSlotObstacles[i].lock()->SetEnable(enable);
		mSlotObstacles[i].lock()->SetRenderEnable(enable);
	}
}

void CShopRoom::WinRoom()
{
}

void CShopRoom::OnExitRoom()
{
}

bool CShopRoom::WinCheck()
{
	return true;
}
