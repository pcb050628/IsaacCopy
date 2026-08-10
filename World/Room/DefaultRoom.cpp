#include "DefaultRoom.h"
#include "../Base/Unitbase.h"

#include "../Manager/GameClassContainer.h"

#include "Asset/PathManager.h"
#include "Asset/AssetManager.h"
#include "Asset/TextureManager.h"
#include "Asset/AnimationManager.h"
#include "World/WorldManager.h"
#include "../Chapter.h"

#include "World/MeshComponent.h"
#include "World/Animation2DComponent.h"

REGISTER_GAMECLASS(CDefaultRoom, "Room_Default", EObjectType::Room);

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

	//메시에 텍스쳐 넣기

	mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundLT"));
	mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundRT"));
	mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundLB"));
	mBackgroundMesh.push_back(CreateComponent<CMeshComponent>("BackgroundRT"));

	mBackgroundAnim.push_back(CreateComponent<CAnimation2DComponent>("AnimLT"));
	mBackgroundAnim.push_back(CreateComponent<CAnimation2DComponent>("AnimRT"));
	mBackgroundAnim.push_back(CreateComponent<CAnimation2DComponent>("AnimLB"));
	mBackgroundAnim.push_back(CreateComponent<CAnimation2DComponent>("AnimRT"));

	//1300 x 700
	//14:8
	//3.5 2
	mBackgroundMesh[0].lock()->SetRelativePos(FVector2(-350.f, 200.f));
	mBackgroundMesh[1].lock()->SetRelativePos(FVector2(350.f, 200.f));
	mBackgroundMesh[2].lock()->SetRelativePos(FVector2(-350.f, -200.f));
	mBackgroundMesh[3].lock()->SetRelativePos(FVector2(350.f, -200.f));
	std::wstring path = CPathManager::FindPath("Texture");
	path += TEXT("Room\\Basement.png");
	CAssetManager::GetInst()->GetSubManager<CTextureManager>(EAssetType::Texture)->LoadTexture("Basement", path.c_str());

	//애니메이션으로 uv 설정중
	auto mgr = CAssetManager::GetInst()->GetSubManager<CAnimationManager>(EAssetType::Animation2D);
	mgr->CreateAnimation("Background_1");
	mgr->SetTexture("Background_1", "Basement");
	mgr->SetAnimationTextureType("Background_1", EAnimation2DTextureType::SpriteSheet);
	mgr->AddFrame("Background_1", FVector2::Zero, FVector2(234.f, 155.f));
	//======================================================================================================
	for (int i = 0; i < 4; ++i)
	{
		mBackgroundMesh[i].lock()->SetRelativeScale(FVector2(700.f, 400.f));
		mBackgroundMesh[i].lock()->SetMesh("TexRect"); mBackgroundMesh[i].lock()->SetShader("Animation2D");

		mBackgroundAnim[i].lock()->SetUpdateComponent(mBackgroundMesh[i]);
		mBackgroundAnim[i].lock()->AddAnimation("Background_1", 1.f, 1.f, true);
	}

	mBackgroundAnim[1].lock()->SetSymmetry("Background_1", true);
	mBackgroundAnim[2].lock()->SetFlip("Background_1", true);
	mBackgroundAnim[3].lock()->SetSymmetry("Background_1", true);
	mBackgroundAnim[3].lock()->SetFlip("Background_1", true);

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
	for (std::weak_ptr<CUnitbase> unit : mUnits)
	{
		if (unit.lock()->IsEnable())
		{
			return false;
		}
	}
	return true;
}