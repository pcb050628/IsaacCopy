#include "BossRoom.h"

#include "../Manager/GameClassContainer.h"
#include "../Chapter.h"

#include "Asset/PathManager.h"
#include "Asset/AssetManager.h"
#include "Asset/TextureManager.h"

#include "../Component/SpriteComponent.h"

#include "../Base/Boss.h"

REGISTER_GAMEOBJCLASS(CBossRoom, "BossRoom", EObjectType::Room)

CBossRoom::CBossRoom()
	:CRoombase(ERoomType::Boss)
{
}

CBossRoom::~CBossRoom()
{
}

bool CBossRoom::Init()
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

	//mOpenInfo.state = EOpenState::Closed;

	//보스 랜덤으로 가져와서 아이디 가지고 있기
	mBossID = 91;
	
	return true;
}

void CBossRoom::Update(float DeltaTime)
{
	CRoombase::Update(DeltaTime);
}

void CBossRoom::Destroy()
{
	CActor::Destroy();
}

void CBossRoom::OnEnterRoom()
{
	if (!mbSpawnCheck)
	{
		std::shared_ptr<CBoss> boss = std::dynamic_pointer_cast<CBoss>(CGameClassContainer::GetInst()->Instantiate(mBossID, FVector2(6, 5)).lock());
		assert(boss && "객체 생성 실패");
		mTargetBoss = boss;
		mbSpawnCheck = true;
	}
	else
	{

	}
}

void CBossRoom::WinRoom()
{
}

void CBossRoom::OnExitRoom()
{
	mTargetBoss.reset();
}

bool CBossRoom::WinCheck()
{
	if (mTargetBoss.expired())
		return false;

	return mTargetBoss.lock()->GetIsDead();
}
