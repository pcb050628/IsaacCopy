#include "DefaultRoom.h"
#include "../Base/Unitbase.h"

#include "LogManager.h"
#include "../Manager/GameClassContainer.h"

#include "Asset/PathManager.h"
#include "Asset/AssetManager.h"
#include "Asset/TextureManager.h"
#include "Asset/AnimationManager.h"
#include "World/WorldManager.h"
#include "../Chapter.h"

#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"

#include "../Component/SpriteComponent.h"

REGISTER_GAMEOBJCLASS(CDefaultRoom, "Room_Default", EObjectType::Room);

CDefaultRoom::CDefaultRoom()
	:CRoombase(ERoomType::Normal, ERoomShape::Normal)
{
	
}

CDefaultRoom::~CDefaultRoom()
{
}

bool CDefaultRoom::Init()
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
	
	mRoomImageSize = FVector2(468.f, 310.f);

	CalculateSize();
	return true;
}

void CDefaultRoom::Update(float DeltaTime)
{
	CRoombase::Update(DeltaTime);
}

void CDefaultRoom::Destroy()
{
	CActor::Destroy();
}

void CDefaultRoom::OnEnterRoom()
{
}

void CDefaultRoom::WinRoom()
{
	mbIsRoomWin = true;
	//보상 스폰하기
}

void CDefaultRoom::OnExitRoom()
{
}

bool CDefaultRoom::WinCheck()
{
	bool flag = false;
	return mMonsterMap.empty();
}